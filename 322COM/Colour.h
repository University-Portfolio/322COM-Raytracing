#pragma once
#include <glm.hpp>


typedef unsigned __int8 byte;


struct Colour
{
public:
	byte r;
	byte g;
	byte b;


	Colour(byte r = 0, byte g = 0, byte b = 0)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
	
	Colour(vec3 vector)
	{
		this->r = min(vector.x * 255.0f, 255.0f);
		this->g = min(vector.y * 255.0f, 255.0f);
		this->b = min(vector.z * 255.0f, 255.0f);
	}

	void Invert() 
	{
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
	}

	inline vec3 ToVector()
	{
		return vec3(r / 255.0, g / 255.0, b / 255.0);
	}
};