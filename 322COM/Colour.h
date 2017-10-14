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
		// Clamp each component to 0-1
		vector.x = glm::max(0.0f, glm::min(1.0f, vector.x));
		vector.y = glm::max(0.0f, glm::min(1.0f, vector.y));
		vector.z = glm::max(0.0f, glm::min(1.0f, vector.z));

		this->r = vector.x * 255.0f;
		this->g = vector.y * 255.0f;
		this->b = vector.z * 255.0f;
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

	inline Colour operator*(float scale)
	{
		return Colour
		(
			(byte)glm::max(0.0f, glm::min(255.0f, r * scale)),
			(byte)glm::max(0.0f, glm::min(255.0f, g * scale)),
			(byte)glm::max(0.0f, glm::min(255.0f, b * scale))
		);
	}
	inline void operator*=(float scale)
	{
		r = glm::max(0.0f, glm::min(1.0f, r * scale));
		g = glm::max(0.0f, glm::min(1.0f, g * scale));
		b = glm::max(0.0f, glm::min(1.0f, b * scale));
	}
	
	inline Colour operator+(Colour other)
	{
		return Colour
		(
			(byte)glm::clamp(0, 255, (int)r + (int)other.r),
			(byte)glm::clamp(0, 255, (int)g + (int)other.g),
			(byte)glm::clamp(0, 255, (int)b + (int)other.b)
		);
	}
	inline void operator+=(Colour other)
	{
		r = (byte)glm::clamp(0, 255, (int)r + (int)other.r);
		g = (byte)glm::clamp(0, 255, (int)g + (int)other.g);
		b = (byte)glm::clamp(0, 255, (int)b + (int)other.b);
	}
};