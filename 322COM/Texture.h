#pragma once
#include <SDL.h>
#include <string>
#include "Colour.h"


/**
* How a texture should query normalized position lookups
*/
enum FilterMode
{
	Nearest,
	Linear
};

/**
* How a texture should treat out of bounds lookups
*/
enum WrapMode
{
	None,
	Wrap
};


/**
* Holds information about about any texture that can be rendered to the screen
*/
class Texture
{
private:
	SDL_Surface* m_image;
	FilterMode m_filterMode;
	WrapMode m_wrapMode;

	int width;
	int height;

public:
	Texture(std::string path);
	~Texture();

	/**
	* Retreive a colour at a specific pixel
	* @param x,y	The position in pixels to check
	* @returns The colour at that pixel or wrap if out of bounds
	*/
	Colour GetColour(int x, int y) const;

	/**
	* Retreive a colour at a specific position in the texture
	* @param x,y	The normalized position on the texture to check for
	* @returns The colour at that pixel or wrap if out of bounds
	*/
	Colour GetColour(float x, float y) const;

	/**
	* Getters and Setters
	*/
public:

	inline SDL_Surface* GetSDLSurface() const { return m_image; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	inline FilterMode GetFilterMode() const { return m_filterMode; }
	inline void SetFilterMode(FilterMode mode) { m_filterMode = mode; }

	inline WrapMode GetWrapMode() const { return m_wrapMode; }
	inline void SetWrapMode(WrapMode mode) { m_wrapMode = mode; }
};

