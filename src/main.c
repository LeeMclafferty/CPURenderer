#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"

#define N_POINTS 9 * 9 * 9
vec3_t cube_points[N_POINTS];

bool is_running = false;

void setup(void)
{
	// Allocate memory for the array based on the window size with each element bing sized for uint32_t
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_height * window_width);

	//Create SDL_Texture that will display the color buffer.
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

	int point_count = 0;
	for (float x = -1; x <= 1; x += 0.25)
	{
		for (float y = -1; y <= 1; y += 0.25)
		{
			for (float z = -1; z <= 1; z += 0.25)
			{
				vec3_t new_point = { .x = x, .y = y, .z = z };
				cube_points[point_count] = new_point;
				point_count++;
			}
		}
	}
}

void process_input(void)
{
	// TODO:
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		break;
	default:
		break;
	}
}

void update(void)
{
	// TODO:
}

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_grid(0xFF333333);

	draw_pixel(20, 20, 0xFFFF0000);
	draw_rect(300, 200, 150, 100, 0xFF234400);
	render_color_buffer();

	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

int main(int argc, char* args[])
{
	is_running = initialize_window();
	setup();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	return 0;
}