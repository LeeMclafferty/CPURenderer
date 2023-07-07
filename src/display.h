#pragma once

#include "SDL.h"
#include <stdbool.h>
#include <stdint.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern int window_width;
extern int window_height;

bool initialize_window(void);
void destroy_window(void);

void render_color_buffer(void);
void clear_color_buffer(uint32_t color);

void draw_grid(uint32_t color);
void draw_rect(int x_pos, int y_pos, int width, int height, uint32_t color);
void draw_pixel(int x_pos, int y_pos, uint32_t color);
