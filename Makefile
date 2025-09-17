# Makefile for Raytracing project with SDL2

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lSDL2 -lm

# Target executable
TARGET = raytracing

# Source files
SRC = raytracing.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up compiled files
clean:
	rm -f $(OBJ) $(TARGET)

# Install SDL2 dependencies (macOS)
install-deps:
	@echo "Installing SDL2 on macOS..."
	brew install sdl2

# Show help
help:
	@echo "Available targets:"
	@echo "  all        - Build the raytracing executable (default)"
	@echo "  run        - Build and run the program"
	@echo "  clean      - Remove compiled files"
	@echo "  install-deps - Install SDL2 using Homebrew"
	@echo "  help       - Show this help message"

# Declare phony targets
.PHONY: all run clean install-deps help
