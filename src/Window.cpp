#include "Window.h"

Window::Window(std::string title, int width, int height) : m_width(width), m_height(height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to load SDL library!");
		exit(-1);
	}

	m_pWindow = SDL_CreateWindow(title.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN);
	SDL_ShowWindow(m_pWindow);
}

SDL_Window* Window::GetWindow()
{
	return m_pWindow;
}

uint32_t* Window::GetBackBuffer()
{
	SDL_Surface* window_surf = SDL_GetWindowSurface(m_pWindow);
	return (uint32_t*)window_surf->pixels;
}
