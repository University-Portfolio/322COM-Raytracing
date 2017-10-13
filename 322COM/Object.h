#pragma once
#include <glm.hpp>
#include "Colour.h"
#include "Ray.h"


using namespace glm;


/**
* Container for all the information about a pixel hit 
*/
struct PixelHitInfo 
{
	class Object* object;
	float distance;

	vec3 location;
	vec3 normal;
	vec2 uvs;
};


/**
* Abstract Object represents something which can be placed in the scene
*/
class Object
{
private:
	vec3 location;
	Colour colour;

public:
	virtual ~Object() {}

	/**
	* Does this ray interesect with this object?
	* @param ray			The ray in question
	* @param hitInfo		Information about the ray intersection
	* @returns True if ray hits, false if misses
	*/
	virtual bool IntersectsRay(Ray ray, PixelHitInfo& hitInfo) = 0;

	/**
	* Getters & Setters
	*/
public:

	inline void SetLocation(vec3 loc) { location = loc; }
	inline vec3 GetLocation() const { return location; }

	inline void SetColour(Colour col) { colour = col; }
	inline Colour GetColour() const { return colour; }
};