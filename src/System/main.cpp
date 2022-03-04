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

	PAK pak;
	pak.LoadPak(".\\assets\\PAK0.PAK");

	QBSP bsp;
	bsp.LoadBSPFromPak(&pak);

	
	///////////////////////
	//	MAIN LOOP
	///////////////////////

	float dt = 0.0f;
	float angle = 0.0f;

	mat4 proj = MakeProjMatrix(75.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	mat4 scale = ScaleMatrix(1.5f);

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
		// update
		angle += dt;

		// render game
		Draw::GetInstance()->Fill(WIDTH, HEIGHT, 0x00);
		
		mat4 screen = ScreenSpaceMatrix(WIDTH / 2, HEIGHT / 2);
		mat4 Rot = YRotationMatrix(angle);

		vec4_t a = { 0.0f, 0.5f, 0.0f, 1.0};
		vec4_t b = { -0.5f, -0.5f, 0.0f, 1.0 };
		vec4_t c = { 0.5f, -0.5f, 0.0f, 1.0 };

		vec4_t translate = { 0.0f, 0.0f, -1.5f, 0.0f };

		/*a = MultiplyMatrixVector(a, scale);
		b = MultiplyMatrixVector(b, scale);
		c = MultiplyMatrixVector(c, scale);*/

		a = MultiplyMatrixVector(a, Rot);
		b = MultiplyMatrixVector(b, Rot);
		c = MultiplyMatrixVector(c, Rot);

		a = AddVectors(a, translate);
		b = AddVectors(b, translate);
		c = AddVectors(c, translate);

		a = MultiplyMatrixVector(a, proj);
		b = MultiplyMatrixVector(b, proj);
		c = MultiplyMatrixVector(c, proj);

		a =  MultiplyMatrixVector(a, screen);
		b = MultiplyMatrixVector(b, screen);
		c = MultiplyMatrixVector(c, screen);

		a = DivVector(a, a.w);
		b = DivVector(b, b.w);
		c = DivVector(c, c.w);

		Draw::GetInstance()->FillTriangle(a.x, a.y, b.x, b.y, c.x, c.y, 0xff0000);

		SDL_UpdateWindowSurface(window.GetWindow());

		Uint64 end = SDL_GetTicks64();
		
		dt = ((float)end - (float)now) / 1000.0f;
	}

	return 0;
}