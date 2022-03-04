#include "quakedef.h"
#include "PAK.h"
#include "QBSP.h"
#include "Frustum.h"
#include "Window.h"
#include "Draw.h"

uint32_t* backbuffer;

int main(int argc, char* argv[])
{
	mesh meshCube;

	meshCube.tris = {

		// SOUTH
		{ 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f},
		{ 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f},

		// EAST           																
		{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f},
		{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f},

		// NORTH           																
		{ 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f},
		{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f},

		// WEST            																
		{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f},
		{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f},

		// TOP             																
		{ 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f},
		{ 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f},

		// BOTTOM          																
		{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f},
		{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f}

	};

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

	// set up main matrices needed
	mat4 scale = ScaleMatrix(0.5f);
	mat4 proj = MakeProjMatrix(75.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	mat4 screen = ScreenSpaceMatrix(WIDTH / 2, HEIGHT / 2);

	// camera stuff
	vec3_t pos = { 0.0f, 0.0f, 2.0f};
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
		for (auto tri : meshCube.tris)
		{
			for (int i = 0; i < 3; i++)
			{
				tri.p[i] = MultiplyMatrixVector( tri.p[i], scale );
				tri.p[i] = MultiplyMatrixVector( tri.p[i], Rot );
				tri.p[i] = MultiplyMatrixVector( tri.p[i], view );
				tri.p[i] = MultiplyMatrixVector( tri.p[i], proj );
				tri.p[i] = MultiplyMatrixVector( tri.p[i], screen );
				tri.p[i] = DivVector( tri.p[i], tri.p[i].w) ;
			}

			DrawTriangle(tri.p[0].x, tri.p[0].y,
				tri.p[1].x, tri.p[1].y,
				tri.p[2].x, tri.p[2].y, 0xffffff);
		}

		SDL_UpdateWindowSurface(window.GetWindow());

		Uint64 end = SDL_GetTicks64();
		
		dt = ((float)end - (float)now) / 1000.0f;
	}

	return 0;
}