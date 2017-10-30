#pragma once
#include <glm.hpp>
#include "Ray.h"


using namespace glm;


/**
* Simple mathematical shape for fast box-ray & box-box collision checks
*/
class BoundingBox
{
private:
	vec3 centre;
	vec3 size;

public:
	BoundingBox(vec3 centre = vec3(0, 0, 0), vec3 size = vec3(1, 1, 1));

	/**
	* Set the bounding box transform from centre and size
	* @param centre			The centre of the box
	* @param size			How much the box should expand from the centre
	*/
	void SetCentreSize(vec3 centre, vec3 size);
	
	/**
	* Set the bounding box transform from min and max
	* @param min			The minimum point of the bounding box i.e. bottom back left
	* @param max			The maximum point of the bounding box i.e. top front right
	*/
	void SetMinMax(vec3 min, vec3 max);

	/**
	* Does the ray intersect with this box?
	* @param ray			The ray to test against
	* @param outDistance	The closest distance hit
	*/
	bool Intersects(Ray ray, float& outDistance) const;

	/**
	* Does the ray intersect with this box?
	* @param ray		The ray to test against
	*/
	inline bool Intersects(Ray ray) const { float temp; return Intersects(ray, temp); }

	/**
	* Do both bounding boxes intersect with each other?
	* @param other		The other bounding box in questions
	*/
	bool Intersects(const BoundingBox& other) const;

	/**
	* Getters & Setters
	*/
public:
	inline vec3 GetCentre() const { return centre; }
	inline vec3 GetSize() const { return size; }

	inline vec3 GetMin() const { return centre - size * 0.5f; }
	inline vec3 GetMax() const { return centre + size * 0.5f; }
};

