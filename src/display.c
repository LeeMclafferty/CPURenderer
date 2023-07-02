#include "display.h"
#include <stdio.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL. \n");
		return false;
	}

	// Use SDL to query what is the full screen max width/height
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	//window_width = display_mode.w;
	//window_height = display_mode.h;

	// Create SDL Window
	window = SDL_CreateWindow("CPU Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		fprintf(stderr, "Error Creating SDL Window. \n");
		return false;
	}

	//CREATE SDL RENDERER
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error Creating SDL Renderer. \n");
		return false;
	}

	return true;
}

void destroy_window(void)
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void render_color_buffer(void)
{
	// Copy the color buffer into the SDL_Texture to be displayed.
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)window_width * sizeof(uint32_t));
	// Render SDL_Texture.
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

/*Color buffer is not actually displayed, it will be moved to an SDL_Texture to be displayed.*/
void clear_color_buffer(uint32_t color)
{
	for (int y = 0; y < window_height; y++) //rows
	{
		for (int x = 0; x < window_width; x++) //columns
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void draw_grid(uint32_t color)
{
	for (int y = 0; y < window_height; y += 10) //rows
	{
		for (int x = 0; x < window_width; x += 10) //columns
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void draw_rect(int x_pos, int y_pos, int width, int height, uint32_t color)
{
	for (int i = 0; i < height; i++) // rows
	{
		for (int j = 0; j < width; j++) // columns
		{
			//Use passed in position as an offset to draw rectangle.
			int x = x_pos + i;
			int y = y_pos + j;
			draw_pixel(x, y, color);
		}
	}
}

void draw_pixel(int x_pos, int y_pos, uint32_t color)
{
	if (x_pos >= 0 && y_pos >= 0 && x_pos < window_width && y_pos < window_height)
	{
		color_buffer[(window_width * y_pos) + x_pos] = color;
	}
}
