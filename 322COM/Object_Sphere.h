#pragma once
#include "Object.h"


class Object_Sphere : public Object
{
private:
	float radius;

public:
	Object_Sphere(vec3 location = vec3(0, 0, 0), float radius = 1);

	/**
	* Does this ray interesect with this object?
	* @param ray			The ray in question
	* @param outDistance	The distance from ray origin to intersection point
	* @returns True if ray hits, false if misses
	*/
	virtual bool IntersectsRay(Ray ray, float& outDistance);


	/**
	* Getters & Setters
	*/
public:

	inline void SetRadius(float rad) { radius = rad; }
	inline float GetRadius() { return radius; }
};

