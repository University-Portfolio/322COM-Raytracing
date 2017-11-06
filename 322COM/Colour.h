#pragma once
#include <glm.hpp>


using namespace glm;
typedef uint8 byte;
typedef tvec4<byte> byte4;
typedef tvec3<byte> byte3;


class Colour : public vec4
{
public:
	Colour(float r = 0, float g = 0, float b = 0, float a = 1.0f) : vec4(r, g, b, a) {}
	Colour(int r, int g = 0, int b = 0, int a = 255) : vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f) {}
	Colour(const vec3& vec) : vec4(vec.x, vec.y, vec.z, 1.0f) {}
	Colour(const vec4& vec) : vec4(vec) {}
	Colour(const Colour& other) : vec4(other) {}

	inline Colour Inverted() const
	{
		return Colour(
			1.0f - r,
			1.0f - g,
			1.0f - b,
			1.0f - a
			);
	}

	inline Colour Clamp(const float min = 0.0f, const float max = 1.0f) const
	{
		return Colour(
			clamp(min, max, r),
			clamp(min, max, g),
			clamp(min, max, b),
			clamp(min, max, a)
			);
	}

	inline byte4 ToRGBA() const
	{
		return byte4(
			255 * clamp(0.0f, 1.0f, r),
			255 * clamp(0.0f, 1.0f, g),
			255 * clamp(0.0f, 1.0f, b),
			255 * clamp(0.0f, 1.0f, a)
			);
	}

	inline byte3 ToRGB() const
	{
		return byte3(
			255 * clamp(0.0f, 1.0f, r),
			255 * clamp(0.0f, 1.0f, g),
			255 * clamp(0.0f, 1.0f, b)
			);
	}
	
	inline Colour& operator*=(vec3 col)
	{
		r *= col.r;
		g *= col.g;
		b *= col.b;
		return *this;
	}
	inline Colour operator*(vec3 col)
	{
		return Colour(
			r * col.r,
			g * col.g,
			b * col.b,
			a
			);
	}

	inline Colour& operator*=(vec4 col)
	{
		r *= col.r;
		g *= col.g;
		b *= col.b;
		a *= col.a;
		return *this;
	}
	inline Colour operator*(vec4 col)
	{
		return Colour(
			r * col.r,
			g * col.g,
			b * col.b,
			a * col.a
			);
	}
};