#include "quakedef.h"
#include "PAK.h"
#include "QBSP.h"
#include "Frustum.h"
#include "Window.h"
#include "Draw.h"
#include <algorithm>
#include <list>
#include <iostream>

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
	float yaw = -90.0f;

	// set up main matrices needed
	mat4 scale = ScaleMatrix(0.5f);
	mat4 proj = MakeProjMatrix(90.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	mat4 screen = ScreenSpaceMatrix(WIDTH / 2, HEIGHT / 2);

	// camera stuff
	vec3_t pos = { -1.8f, 0.0f, 2.0f};
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
	
		// update matrices
		vec3_t orientation = AddVectors(pos, at);
		mat4 view = LookAt(pos, orientation, up);

		std::vector<triangle> triToRaster;

		// Transform all vertices and clip the triangles
		// this is the full pipeline
		for (auto tri : meshCube.tris)
		{
			for (int i = 0; i < 3; i++)
			{
				tri.p[i] = MultiplyMatrixVector( tri.p[i], scale );
			}

			vec4_t normal, line1, line2, pos4;

			// Get lines either side of triangle
			line1 = SubVectors(tri.p[1], tri.p[0]);
			line2 = SubVectors(tri.p[2], tri.p[0]);

			// normal to the triangle
			normal = CrossProduct(line1, line2);

			// You normally need to normalise a normal!
			normal = NormalizeVector(normal);

			pos4 = { pos.x, pos.y, pos.z, 1.0f };

			// Get Ray from triangle to camera
			pos4 = SubVectors(tri.p[0], pos4);

			// backface-culling - render only triangle facing the camera
			if (DotProduct(normal, pos4) < 0.0f)
			{

				tri.p[0] = MultiplyMatrixVector(tri.p[0], view);
				tri.p[1] = MultiplyMatrixVector(tri.p[1], view);
				tri.p[2] = MultiplyMatrixVector(tri.p[2], view);

				// clip triangle against near_z plane
				triangle tri_clipped[2];
				int clipped_count = 0;

				clipped_count = ClipTriangleToPlane({ 0.0f, 0.0f, -0.1f }, { 0.0f, 0.0f, -1.0f },
					tri, tri_clipped[0], tri_clipped[1]);

				for (int n = 0; n < clipped_count; n++)
				{
					tri_clipped[n].p[0] = MultiplyMatrixVector(tri_clipped[n].p[0], proj);
					tri_clipped[n].p[1] = MultiplyMatrixVector(tri_clipped[n].p[1], proj);
					tri_clipped[n].p[2] = MultiplyMatrixVector(tri_clipped[n].p[2], proj);

					tri_clipped[n].p[0] = DivVector(tri_clipped[n].p[0], tri_clipped[n].p[0].w);
					tri_clipped[n].p[1] = DivVector(tri_clipped[n].p[1], tri_clipped[n].p[1].w);
					tri_clipped[n].p[2] = DivVector(tri_clipped[n].p[2], tri_clipped[n].p[2].w);

					tri_clipped[n].p[0] = MultiplyMatrixVector(tri_clipped[n].p[0], screen);
					tri_clipped[n].p[1] = MultiplyMatrixVector(tri_clipped[n].p[1], screen);
					tri_clipped[n].p[2] = MultiplyMatrixVector(tri_clipped[n].p[2], screen);
					
					triToRaster.push_back(tri_clipped[n]);
				}

			}
			
		}

		// render game
		Fill(WIDTH, HEIGHT, 0x00);

		// clip triangles to screen edges
		for (auto& tri : triToRaster)
		{
			// Clip triangles against all four screen edges, this could yield
			// a bunch of triangles, so create a queue that we traverse to 
			//  ensure we only test new triangles generated against planes
			triangle clipped[2];
			std::list<triangle> listTriangles;

			// Add initial triangle
			listTriangles.push_back(tri);
			int nNewTriangles = 1;

			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					// Take triangle from front of queue
					triangle test = listTriangles.front();
					listTriangles.pop_front();
					nNewTriangles--;

					// Clip it against a plane. We only need to test each 
					// subsequent plane, against subsequent new triangles
					// as all triangles after a plane clip are guaranteed
					// to lie on the inside of the plane. I like how this
					// comment is almost completely and utterly justified
					switch (p)
					{
					// top
					case 0:	nTrisToAdd = ClipTriangleToPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					// bottom
					case 1:	nTrisToAdd = ClipTriangleToPlane({ 0.0f, (float)HEIGHT - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					// left
					case 2:	nTrisToAdd = ClipTriangleToPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					// right
					case 3:	nTrisToAdd = ClipTriangleToPlane({ (float)WIDTH - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					// Clipping may yield a variable number of triangles, so
					// add these new ones to the back of the queue for subsequent
					// clipping against next planes
					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}


			// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
			for (auto& t : listTriangles)
			{
				FillTriangle(t.p[0].x, t.p[0].y,
					t.p[1].x, t.p[1].y,
					t.p[2].x, t.p[2].y, 0xffffff, true);
			}
		}

		SDL_UpdateWindowSurface(window.GetWindow());

		Uint64 end = SDL_GetTicks64();
		
		dt = ((float)end - (float)now) / 1000.0f;
	}

	return 0;
}