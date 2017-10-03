#include "RenderSurface.h"



RenderSurface::RenderSurface(int width, int height, Uint32 format)
{
	this->width = width;
	this->height = height;

	m_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		width, height,
		0,
		format
	);
}

RenderSurface::~RenderSurface()
{
	if (m_surface != nullptr)
		SDL_FreeSurface(m_surface);
}

void RenderSurface::Clear(Colour colour) 
{
	SDL_FillRect(m_surface, nullptr, SDL_MapRGB(m_surface->format, colour.r, colour.g, colour.b));
}

void RenderSurface::SetPixel(int x, int y, Colour colour) 
{
	Uint32* pixel = (Uint32*)m_surface->pixels + y * width + x;
	*pixel = SDL_MapRGB(m_surface->format, colour.r, colour.g, colour.b);
}