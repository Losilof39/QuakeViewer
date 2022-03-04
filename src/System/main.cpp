#include "quakedef.h"
#include "PAK.h"
#include "QBSP.h"
#include "Frustum.h"
#include "Window.h"
#include "Draw.h"

int main(int argc, char* argv[])
{

	///////////////////////
	//	WINDOW INIT
	///////////////////////

	Window window("Quake Renderer", WIDTH, HEIGHT);
	Draw::GetInstance()->Init(window.GetBackBuffer());


	///////////////////////
	//	LOAD PAK AND BSP
	///////////////////////

	/*PAK pak;
	pak.LoadPak(".\\assets\\PAK0.PAK");

	QBSP bsp;
	bsp.LoadBSPFromPak(&pak);*/

	
	///////////////////////
	//	MAIN LOOP
	///////////////////////

	float dt = 0.0f;
	float angle = 0.0f;

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
		// render game
		Draw::GetInstance()->Fill(WIDTH, HEIGHT, 0x00);

		angle += 1.0f * dt;
		
		mat4 screen = ScreenSpaceMatrix(WIDTH / 2, HEIGHT / 2);
		mat4 RotX = XRotationMatrix(angle);

		vec3_t point = { 0.0f, 0.0f, 0.0f, 1.0};

		point = MultiplyMatrixVector(point, RotX);
		point = MultiplyMatrixVector(point, screen);

		Draw::GetInstance()->Pixel(point.x, point.y, 0xff0000);

		SDL_UpdateWindowSurface(window.GetWindow());

		Uint64 end = SDL_GetTicks64();
		
		dt = ((float)end - (float)now) / 1000.0f;
	}

	return 0;
}