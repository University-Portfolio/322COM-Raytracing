#pragma once
#include "Object.h"


class Object_Plane : public Object
{
private:
	vec3 normal;
	float extent = 1000000.0f;
	vec2 uvScale = vec2(1, 1);

public:
	Object_Plane(vec3 location = vec3(0, 0, 0), vec3 normal = vec3(0,1,0));

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
	inline void SetExtent(float e) { extent = e; }
	inline float GetExtent() const { return extent; }

	inline void SetNormal(vec3 n) { normal = normalize(n); }
	inline vec3 GetNormal() const { return normal; }

	inline void SetUVScale(vec2 s) { uvScale = s; }
	inline vec2 GetUVScale() const { return uvScale; }
};

