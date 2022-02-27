#include "quakedef.h"
#include "PAK.h"
#include "QBSP.h"
#include "Frustum.h"
#include "Camera.h"

int main(int argc, char* argv[])
{
	///////////////////////
	//	SDL STUFF INIT
	///////////////////////

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to load SDL library!");
		return -1;
	}

	SDL_Window* pWindow = SDL_CreateWindow("Quake 3D Renderer", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	SDL_ShowWindow(pWindow);

	///////////////////////
	//	LOAD PAK AND BSP
	///////////////////////

	PAK pak;
	pak.LoadPak(".\\assets\\PAK0.PAK");

	QBSP bsp;
	bsp.LoadBSPFromPak(&pak);

	glm::vec3 pos = {544.0f, 288.0f, 32.0f};

	int camleaf = bsp.FindCamLeaf(pos);
	printf("Current leaf: %d", camleaf);


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
		/*if (keystates[SDL_SCANCODE_W])
			pos.y += 100.0f * dt;
		if (keystates[SDL_SCANCODE_S])
			pos.y -= 100.0f * dt;
		if (keystates[SDL_SCANCODE_A])
			pos.x -= 100.0f * dt;
		if (keystates[SDL_SCANCODE_D])
			pos.x += 100.0f * dt;*/

		// render game

		Uint64 end = SDL_GetTicks64();
		
		dt = ((float)end - (float)now) / 1000.0f;
	}

	return 0;
}