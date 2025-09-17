#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x00000000
#define RAYS_NUMBER 100
#define RAY_COLOR 0xffd43b
#define COLOR_GRAY 0x80808080

typedef struct Circle {
  double x;
  double y;
  double r;
} Circle;

typedef struct Ray {
  double x_start, y_start;
  double angle;
} Ray;

void FillRays(SDL_Surface *surface, Ray rays[RAYS_NUMBER], Uint32 color,
              Circle object) {

  double radius_squared = pow(object.r, 2);
  for (int i = 0; i < RAYS_NUMBER; i++) {
    Ray ray = rays[i];
    int end_of_screen = 0;
    int object_hit = 0;
    double step = 1;

    double x_draw = ray.x_start;
    double y_draw = ray.y_start;
    while (!end_of_screen && !object_hit) {
      x_draw += cos(ray.angle) * step;
      y_draw += sin(ray.angle) * step;
      SDL_Rect pixel = (SDL_Rect){x_draw, y_draw, 1, 1};
      SDL_FillRect(surface, &pixel, color);
      if (x_draw < 0 || x_draw > WIDTH || y_draw < 0 || y_draw > HEIGHT) {
        end_of_screen = 1;
      }
      double distance_squared =
          pow(x_draw - object.x, 2) + pow(y_draw - object.y, 2);
      if (distance_squared < radius_squared) {
        object_hit = 1;
        break;
      }
    }
  }
}
void generate_rays(Circle circle, Ray rays[RAYS_NUMBER]) {
  for (int i = 0; i < RAYS_NUMBER; i++) {
    double angle = (double)(i)*2 * M_PI / RAYS_NUMBER;
    rays[i] = (Ray){circle.x, circle.y, angle};
  }
}
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
  Circle shadow = (Circle){650, 300, 140};
  SDL_Rect erase_rect = (SDL_Rect){0, 0, WIDTH, HEIGHT};

  Ray rays[RAYS_NUMBER];
  generate_rays(circle, rays);
  double obstacle_speed_y = 1;
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
        generate_rays(circle, rays);
      }
    }
    SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
    FillRays(surface, rays, RAY_COLOR, shadow);
    FillCircle(surface, circle, COLOR_WHITE);
    FillCircle(surface, shadow, COLOR_WHITE);

    shadow.y += obstacle_speed_y;
    if (shadow.y + shadow.r > HEIGHT || shadow.y - shadow.r < 0) {
      obstacle_speed_y = -obstacle_speed_y;
    }
    SDL_UpdateWindowSurface(window);
    SDL_Delay(16);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}