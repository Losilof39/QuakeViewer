#pragma once

#include "quakedef.h"

class Window
{
public:
	Window(std::string title, int width, int height);
	SDL_Window* GetWindow();
	uint32_t* GetBackBuffer();

private:
	int m_width, m_height;
	SDL_Window* m_pWindow;
	uint32_t* backbuffer;
};