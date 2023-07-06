#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"

#define N_POINTS 9 * 9 * 9
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_pos = { 0, 0 , -5};
vec3_t cube_rotation = { 0, 0, 0 };

float fov_factor = 640;

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

// Transform 3D vector to 2D vector/point to render
vec2_t project(vec3_t point)
{
	float x = (point.x * fov_factor) / point.z; // divide by z to mock scaling points
	float y = (point.y * fov_factor) / point.z;
	vec2_t projected_point = { .x = x, .y = y};
	return projected_point;
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
	cube_rotation.y += (float)0.0005;
	cube_rotation.z += (float)0.0005;
	cube_rotation.x += (float)0.0005;

	for (int i = 0; i < N_POINTS; i++)
	{
		vec3_t point = cube_points[i];

		vec3_t transformed_point = vec3_rotate_y(point, cube_rotation.y);
		transformed_point = vec3_rotate_x(transformed_point, cube_rotation.x);
		transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

		// Translate point away from camera to avoid the view port being in the center of our render.
		transformed_point.z -= camera_pos.z;

		// Project current point
		vec2_t projected_point = project(transformed_point);
		projected_points[i] = projected_point;
	}
}

void render(void)
{
	draw_grid(0xFF333333);
	
	// Loop over projected points and render them
	for (int i = 0; i < N_POINTS; i++)
	{
		vec2_t projected_point = projected_points[i];
		draw_rect(projected_point.x + (float)(window_width/2), projected_point.y + (float)(window_height / 2), 4, 4, 0xFFFF00FF);
	}

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