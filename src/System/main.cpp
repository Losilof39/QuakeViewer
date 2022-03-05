#include "quakedef.h"
#include "PAK.h"
#include "QBSP.h"
#include "Frustum.h"
#include "Window.h"
#include "Draw.h"
#include <iostream>
#include <algorithm>

uint32_t* backbuffer;

int main(int argc, char* argv[])
{

	mesh meshCube;
	meshCube.LoadFromObjectFile("axis.obj");

	//meshCube.tris = {

	//	// SOUTH
	//	{ 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f},
	//	{ 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f},

	//	// EAST           																
	//	{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f},
	//	{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f},

	//	// NORTH           																
	//	{ 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f},
	//	{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f},

	//	// WEST            																
	//	{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f},
	//	{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f},

	//	// TOP             																
	//	{ 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f},
	//	{ 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f},

	//	// BOTTOM          																
	//	{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f},
	//	{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f}

	//};

	///////////////////////
	//	WINDOW INIT
	///////////////////////

	Window window("Quake Renderer", WIDTH, HEIGHT);

	backbuffer = window.GetBackBuffer();

	///////////////////////
	//	LOAD PAK AND BSP
	///////////////////////

	PAK pak;
	pak.LoadPak(".\\assets\\PAK0.PAK");

	QBSP bsp;
	bsp.LoadBSPFromPak(&pak);

	
	///////////////////////
	//	MAIN LOOP
	///////////////////////

	float dt = 0.0f;
	float angle = 0.0f;
	float yaw = -90.0f;

	// set up main matrices needed
	mat4 scale = ScaleMatrix(0.3f);
	mat4 proj = MakeProjMatrix(90.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	mat4 screen = ScreenSpaceMatrix(WIDTH / 2, HEIGHT / 2);

	// camera stuff
	vec3_t pos = { 0.0f, 0.0f, 5.0f};
	vec3_t at = { 0.0f, 0.0f, -1.0f };
	vec3_t up = { 0.0f, 1.0f, 0.0f };
	vec3_t orientation = AddVectors(pos, at);
	mat4 view = LookAt(pos, orientation, up);

	SDL_Event event;
	while (1)
	{
		Uint64 now = SDL_GetTicks();

		// get all input states
		const Uint8* keystates = SDL_GetKeyboardState(NULL);

		// get OS and Window events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				exit(0);
			}break;
			default:
				break;
			}
		}

		// update
		angle += dt;

		if (keystates[SDL_SCANCODE_W])
			pos.z -= dt;
		if (keystates[SDL_SCANCODE_S])
			pos.z += dt;
		if (keystates[SDL_SCANCODE_A])
			pos.x -= dt;
		if (keystates[SDL_SCANCODE_D])
			pos.x += dt;

		if (keystates[SDL_SCANCODE_SPACE])
			pos.y += dt;
		if (keystates[SDL_SCANCODE_LSHIFT])
			pos.y -= dt;
			

		// render game
		Fill(WIDTH, HEIGHT, 0x00);
	
		// update matrices
		mat4 Rot = YRotationMatrix(angle);
		vec3_t orientation = AddVectors(pos, at);
		mat4 view = LookAt(pos, orientation, up);

		// draw the cube!
		// this is the full pipeline
		for (auto tri : meshCube.tris)
		{
			for (int i = 0; i < 3; i++)
			{
				tri.p[i] = MultiplyMatrixVector( tri.p[i], scale );
				tri.p[i] = MultiplyMatrixVector(tri.p[i], Rot);
			}

			vec4_t normal, line1, line2, pos4;

			// Get lines either side of triangle
			line1 = SubVectors(tri.p[1], tri.p[0]);
			line2 = SubVectors(tri.p[2], tri.p[0]);

			// Take cross product of lines to get normal to triangle surface
			normal = CrossProduct(line1, line2);

			// You normally need to normalise a normal!
			normal = NormalizeVector(normal);

			pos4 = { pos.x, pos.y, pos.z, 1.0f };

			// Get Ray from triangle to camera
			pos4 = SubVectors(tri.p[0], pos4);

			// backface-culling - render only triangle facing the camera
			if (DotProduct(normal, pos4) < 0.0f)
			{
				for (int i = 0; i < 3; i++)
				{
					tri.p[i] = MultiplyMatrixVector(tri.p[i], view);
					tri.p[i] = MultiplyMatrixVector(tri.p[i], proj);
					tri.p[i] = DivVector(tri.p[i], tri.p[i].w);
					tri.p[i] = MultiplyMatrixVector(tri.p[i], screen);
				}

				DrawTriangle(tri.p[0].x, tri.p[0].y,
					tri.p[1].x, tri.p[1].y,
					tri.p[2].x, tri.p[2].y, 0xffffff);
			}
			
		}

		SDL_UpdateWindowSurface(window.GetWindow());

		Uint64 end = SDL_GetTicks64();
		
		dt = ((float)end - (float)now) / 1000.0f;
	}

	return 0;
}