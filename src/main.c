#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>

bool is_running = false;

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
		return false;	}

	// Create SDL Window
	window = SDL_CreateWindow("CPU Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		fprintf(stderr, "Error Creating SDL Window. \n");
		return false;
	}
	
	//TOD0: CREATE SDL RENDERER
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error Creating SDL Renderer. \n");
		return false;
	}

	return true;
}

void setup(void)
{
	// Allocate memory for the array based on the window size with each element bing sized for uint32_t
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_height * window_width);

	//Create SDL_Texture that will display the color buffer.
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

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

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	render_color_buffer();

	clear_color_buffer(0xFFFFFF00);

	SDL_RenderPresent(renderer);
}

void destroy_window(void)
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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