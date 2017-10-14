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
	
	Colour(glm::vec3 vector)
	{
		this->r = glm::min(vector.x * 255.0f, 255.0f);
		this->g = glm::min(vector.y * 255.0f, 255.0f);
		this->b = glm::min(vector.z * 255.0f, 255.0f);
	}

	void Invert() 
	{
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
	}

	inline glm::vec3 ToVector()
	{
		return glm::vec3(r / 255.0, g / 255.0, b / 255.0);
	}

	/**
	* Apply colour filter to this colour
	*/
	inline void Filter(Colour c) 
	{
		glm::vec3 f = c.ToVector();
		r = f.x * r;
		g = f.y * g;
		b = f.z * b;
	}
};