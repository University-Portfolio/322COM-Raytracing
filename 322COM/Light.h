#pragma once
#include <glm.hpp>
#include "Colour.h"


using namespace glm;


/**
* Base class for any lights which can be placed in the scene
*/
class Light
{
private:
	vec3 location;
	Colour colour;

public:
	/**
	* Does this ray interesect with this object?
	* @param ray			The ray in question
	* @param outDistance	The distance from ray origin to intersection point
	* @returns True if ray hits, false if misses
	*/
	virtual bool AffectObject(class Object* object) = 0;

	/**
	* Getters & Setters
	*/
public:

	inline void SetLocation(vec3 loc) { location = loc; }
	inline vec3 GetLocation() const { return location; }

	inline void SetColour(Colour col) { colour = col; }
	inline Colour GetColour() const { return colour; }
};

