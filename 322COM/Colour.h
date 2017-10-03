#pragma once


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

	void Invert() 
	{
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
	}
};