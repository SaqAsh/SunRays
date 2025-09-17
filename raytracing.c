#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x00000000

typedef struct Circle {
  double x;
  double y;
  double r;
} Circle;
void FillCircle(SDL_Surface *surface, Circle circle, Uint32 color) {

  double radius_squared = pow(circle.r, 2);
  // Iterate over the cicle, the way we do this is the following
  for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++) {
    for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++) {
      // we need to save computation, via avoiding the sqrt
      double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
      if (distance_squared < radius_squared) {
        SDL_Rect pixel = (SDL_Rect){x, y, 1, 1};
        SDL_FillRect(surface, &pixel, color);
      }
    }
  }
}
int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  Circle circle = (Circle){200, 200, 80};
  SDL_Rect erase_rect = (SDL_Rect){0, 0, WIDTH, HEIGHT};

  // Event loop
  SDL_Event event;
  int running = 1;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
      if (event.type == SDL_MOUSEMOTION && event.motion.state != 0) {
        circle.x = event.motion.x;
        circle.y = event.motion.y;
      }
    }
    SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
    FillCircle(surface, circle, COLOR_WHITE);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(16);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}