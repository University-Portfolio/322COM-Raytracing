#pragma once
#include <SDL.h>
#include "Colour.h"


/**
* Specific surface/target that can be drawn to
*/
class RenderSurface
{
private:
	SDL_Surface* m_surface = nullptr;

	int width;
	int height;

public:
	RenderSurface(int width, int height, Uint32 format);
	~RenderSurface();

	/**
	* Clear this render target with a specific colour
	*/
	void Clear(Colour colour);

	/**
	* Set the colour of a specific pixel
	*/
	void SetPixel(int x, int y, Colour colour);

	/**
	* Draw a rectangle of a given colour
	*/
	void SetRect(int x, int y, int width, int height, Colour colour);


	/**
	* Getters and Setters
	*/
public:

	inline SDL_Surface* GetSDLSurface() const { return m_surface; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};

