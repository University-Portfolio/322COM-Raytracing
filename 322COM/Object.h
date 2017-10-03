#pragma once
#include <glm.hpp>
#include "Colour.h"
#include "Ray.h"


using namespace glm;


/**
* Abstract Object represents something which can be placed in the scene
*/
class Object
{
private:
	vec3 location;
	Colour colour;

public:
	Object(vec3 location = vec3(0,0,0));
	virtual ~Object() {}

	/**
	* Does this ray interesect with this object?
	* @param ray			The ray in question
	* @param outDistance	The distance from ray origin to intersection point
	* @returns True if ray hits, false if misses
	*/
	virtual bool IntersectsRay(Ray ray, float& outDistance) = 0;

	/**
	* Getters & Setters
	*/
public:

	inline void SetLocation(vec3 loc) { location = loc; }
	inline vec3 GetLocation() const { return location; }

	inline void SetColour(Colour col) { colour = col; }
	inline Colour GetColour() const { return colour; }
};

