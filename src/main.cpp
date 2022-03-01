#include "quakedef.h"
#include "PAK.h"
#include "QBSP.h"
#include "Frustum.h"

void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, SDL_Renderer* renderer)
{
	SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
	SDL_RenderDrawLineF(renderer, x2, y2, x3, y3);
	SDL_RenderDrawLineF(renderer, x3, y3, x1, y1);
}

int main(int argc, char* argv[])
{
	mesh meshCube;

	meshCube.tris = {

		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

	};

	///////////////////////
	//	SDL STUFF INIT
	///////////////////////

	const int width = 640;
	const int height = 480;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to load SDL library!");
		return -1;
	}

	SDL_Window* pWindow = SDL_CreateWindow("Quake 3D Renderer", 100, 100, width, height, SDL_WINDOW_SHOWN);
	SDL_ShowWindow(pWindow);

	SDL_Renderer* pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE);

	///////////////////////
	//	LOAD PAK AND BSP
	///////////////////////

	PAK pak;
	pak.LoadPak(".\\assets\\PAK0.PAK");

	QBSP bsp;
	bsp.LoadBSPFromPak(&pak);

	//Camera cam;
	Frustum frustum;


	// Projection Matrix
	mat4 proj = MakeProjMatrix(90.0f, (float)height / (float)width, 0.1f, 1000.0f);

	vec3_t cam = { 0.0f, 0.0f, 0.0f };
	vec3_t target = { 0.0f, 0.0f, 1.0f };
	vec3_t up = { 0.0f, 1.0f, 0.0f };

	mat4 view = MakeViewMatrix(cam, target, up);

	//glm::vec3 pos = {544.0f, 288.0f, 32.0f};

	
	///////////////////////
	//	MAIN LOOP
	///////////////////////

	float dt = 0.0f;

	SDL_Event event;
	while (1)
	{
		Uint64 now = SDL_GetTicks();

		const Uint8* keystates = SDL_GetKeyboardState(NULL);

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

		// update game

		if (keystates[SDL_SCANCODE_W])
			cam.z += 1.0f * dt;
		if (keystates[SDL_SCANCODE_S])
			cam.z -= 1.0f * dt;
		if (keystates[SDL_SCANCODE_SPACE])
			cam.y += 1.0f * dt;
		if (keystates[SDL_SCANCODE_LSHIFT])
			cam.y -= 1.0f * dt;

		// render game

		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

		SDL_RenderClear(pRenderer);

		SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
		
		view = MakeViewMatrix(cam, target, up);

		for (auto tri : meshCube.tris)
		{
			//triangle triProjected;

			tri.point[0].z += 1.5f;
			tri.point[1].z += 1.5f;
			tri.point[2].z += 1.5f;

			for (int i = 0; i < 3; i++)
			{
				tri.point[i] = MultiplyMatrixVector(tri.point[i], view);
				tri.point[i] = MultiplyMatrixVector(tri.point[i], proj);
			}

			AdjustToView(tri, width, height);

			DrawTriangle(tri.point[0].x, tri.point[0].y,
				tri.point[1].x, tri.point[1].y,
				tri.point[2].x, tri.point[2].y,
				pRenderer);
		}

		SDL_RenderPresent(pRenderer);

		Uint64 end = SDL_GetTicks64();
		
		dt = ((float)end - (float)now) / 1000.0f;
	}

	return 0;
}