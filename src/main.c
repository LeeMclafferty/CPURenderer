#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

triangle_t triangles_to_render[N_MESH_FACES];

vec3_t camera_pos = { 0, 0 , -5};
vec3_t cube_rotation = { 0, 0, 0 };

float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0; // Track time for FPS

void setup(void)
{
	// Allocate memory for the array based on the window size with each element bing sized for uint32_t
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_height * window_width);

	//Create SDL_Texture that will display the color buffer.
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
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
	// Stop execution for consistent FPS
// 	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
// 
// 	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
// 	{
// 		SDL_Delay(time_to_wait);
// 	}
	SDL_Delay(FRAME_TARGET_TIME);

	previous_frame_time = SDL_GetTicks();

	// Add rotation manually for testing
	cube_rotation.y += (float)0.01;
	cube_rotation.z += (float)0.01;
	cube_rotation.x += (float)0.01;

	for (int i = 0; i < N_MESH_FACES; i++)
	{
		face_t mesh_face = mesh_faces[i];

		vec3_t face_verticies[3];
		face_verticies[0] = mesh_vertices[mesh_face.a - 1];
		face_verticies[1] = mesh_vertices[mesh_face.b - 1];
		face_verticies[2] = mesh_vertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		//Loop vertices of current face and apply transformations and project in to 2D space
		for (int j = 0; j < 3; j++)
		{
			vec3_t transformed_vertex = face_verticies[j];

			transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
			transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

			// Translate vertex away from camera
			transformed_vertex.z -= camera_pos.z;

			// project current vertex
			vec2_t projected_point = project(transformed_vertex);

			//scale and translate projected point to middle of screen.
			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);
			projected_triangle.points[j] = projected_point;
		}

		// Array of triangles to render
		triangles_to_render[i] = projected_triangle;
	}
}

void render(void)
{
	draw_grid(0xFF333333);
	
	// Loop over projected triangle and render them
 	for (int i = 0; i < N_MESH_FACES; i++)
 	{
 		triangle_t triangle = triangles_to_render[i];
		// draw vertex points
		draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFF00FF);
		draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFF00FF);
		draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFF00FF);

		printf("point[0]: %f, %f\n", triangle.points[0].x, triangle.points[0].y);
		printf("point[1]: %f, %f\n", triangle.points[1].x, triangle.points[1].y);
		printf("point[2]: %f, %f\n", triangle.points[2].x, triangle.points[2].y);

		// Freezes execution
//         draw_triangle(
//             triangle.points[0].x, triangle.points[0].y,
//             triangle.points[1].x, triangle.points[1].y,
//             triangle.points[2].x, triangle.points[2].y,
//             0xFF00FF00
//         );
 	}
		//works
	draw_line(628.939087, 267.439209, 435.483582, 362.301422, 0xFFFF0000);
	draw_line(435.483582, 362.301422, 513.708618, 179.648712, 0xFFFF0000);
	draw_line(513.708618, 179.648712, 628.939087, 267.439209, 0xFFFF0000);

	// does not work
	draw_triangle(
		628.939087, 267.439209,
		435.483582, 362.301422,
		513.708618, 179.648712,
		0xFFFFFFFF);

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