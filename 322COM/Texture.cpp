#include "Texture.h"
#include "Logger.h"
#include "glm.hpp"


using namespace glm;


Texture::Texture(std::string path)
{
	m_image = SDL_LoadBMP(path.c_str());
	
	if (m_image == nullptr)
	{
		LOG_ERROR("Unabled to load texture from path '%s'", path.c_str());
		return;
	}

	width = m_image->clip_rect.w;
	height = m_image->clip_rect.h;
	LOG("Loaded texture from path '%s'", path.c_str());
}


Texture::~Texture()
{
	if (m_image == nullptr)
		SDL_FreeSurface(m_image);
}

Colour Texture::GetColour(int x, int y) const
{
	// Send black, if invalid request
	if (m_image == nullptr)
		return Colour(0, 0, 0);


	// Convert normalized position to pixel coord
	if(m_wrapMode == WrapMode::Wrap)
	{
		x %= width;
		y %= height;

		if (x < 0)
			x += width;
		if (y < 0)
			y += height;
	}

	// Out of bounds so return black
	else if(x < 0 || x >= width || y < 0 || y >= height)
		return Colour(0, 0, 0);



	// Get pointer to start of pixel info (Pitch = width * bytesPerPixel)
	Uint8* pixelPtr = (Uint8*)m_image->pixels + y * m_image->pitch + x;
	Uint32 pixel = 0;

	// Base on bbp fetch the correct value for this pixel
	switch (m_image->format->BytesPerPixel)
	{
		case 1:
			pixel = *pixelPtr;
			break;

		case 2:
			pixel = *(Uint16*)pixelPtr;
			break;

		case 3:
			// Work out which way rgb is stored and offset it correctly
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				pixel = pixelPtr[0] << 16 | pixelPtr[1] << 8 | pixelPtr[2];
			else
				pixel = pixelPtr[0] | pixelPtr[1] << 8 | pixelPtr[2] << 16;
			break;

		case 4:
			pixel = *(Uint32*)pixelPtr;
			break;
	}


	// Split colour into rgb based on format
	Colour c;
	SDL_GetRGB(pixel, m_image->format, &c.r, &c.g, &c.b);
	return c;
}

Colour Texture::GetColour(float x, float y) const 
{
	if (m_image == nullptr)
		return Colour(0, 0, 0);


	switch (m_filterMode)
	{
		case Nearest:
		{
			int px = (int)(round(x * width)) % width;
			int py = (int)(round(y * height)) % height;
			return GetColour(px, py);
		}

		case Linear:
		{
			float px = x * width;
			float py = y * height;

			int xWhole = floor(px);
			int yWhole = floor(py);
			float xFrac = px - xWhole;
			float yFrac = py - yWhole;

			// Check adjacent pixels and blur together based on query distance from pixels
			const vec3 v00 = GetColour(xWhole, yWhole).ToVector();
			const vec3 v10 = GetColour(xWhole + 1, yWhole).ToVector();
			const vec3 v01 = GetColour(xWhole, yWhole + 1).ToVector();
			const vec3 v11 = GetColour(xWhole + 1, yWhole + 1).ToVector();

			// Check blend x small and x big for y small and y big
			const vec3 vx0 = v00 * (1.0f - xFrac) + v10 * xFrac;
			const vec3 vx1 = v01 * (1.0f - xFrac) + v11 * xFrac;

			// Blend x blended vales for y small and y big
			const vec3 v = vx0 * (1.0f - yFrac) + vx1 * yFrac;
			return Colour(v);
		}

		default:
			break;
	}

	return Colour(0, 0, 0);
}