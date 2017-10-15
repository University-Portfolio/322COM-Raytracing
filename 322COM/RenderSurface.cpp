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
	byte3 col = colour.ToRGB();
	*pixel = SDL_MapRGB(m_surface->format, col.r, col.g, col.b);
}

void RenderSurface::SetRect(int x, int y, int width, int height, Colour colour) 
{
	byte3 tcol = colour.ToRGB();
	Uint32 col = SDL_MapRGB(m_surface->format, tcol.r, tcol.g, tcol.b);


	for (int dx = 0; dx < width; ++dx)
	{
		// Ignore if out of range
		int xc = x + dx;
		if (xc < 0 || xc >= this->width)
			continue;

		for (int dy = 0; dy < height; ++dy)
		{
			// Ignore if out of range
			int yc = y + dy;
			if (yc < 0 || yc >= this->height)
				continue;

			Uint32* pixel = (Uint32*)m_surface->pixels + yc * this->width + xc;
			*pixel = col;
		}
	}
}