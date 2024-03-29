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
	* @param hitInfo		Information about the ray intersection
	* @returns True if ray hits, false if misses
	*/
	virtual bool IntersectsRay(Ray ray, PixelHitInfo& hitInfo);

	/**
	* Get the AABB for this specific object
	* @return Object centred bounding box
	*/
	virtual BoundingBox GetAABB() const;

	/**
	* Getters & Setters
	*/
public:

	inline void SetRadius(float rad) { radius = rad; }
	inline float GetRadius() const { return radius; }
};

