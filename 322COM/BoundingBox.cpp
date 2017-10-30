#include "BoundingBox.h"



BoundingBox::BoundingBox(vec3 centre, vec3 size) : centre(centre), size(size)
{
}

void BoundingBox::SetCentreSize(vec3 centre, vec3 size) 
{
	this->centre = centre;
	this->size = size;
}

void BoundingBox::SetMinMax(vec3 min, vec3 max) 
{
	centre = (min + max) * 0.5f;
	size = max - min;
}

bool BoundingBox::Intersects(Ray ray, float& outDistance) const
{
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	const vec3 min = GetMin();
	const vec3 max = GetMax();
	const vec3 invDir = 1.0f / ray.direction;


	// Check y
	float t0 = (min.x - ray.origin.x) * invDir.x;
	float t1 = (max.x - ray.origin.x) * invDir.x;
	
	// Swap
	if (t0 > t1)
	{
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}
	
	
	// Calc y
	float t0y = (min.y - ray.origin.y) * invDir.y;
	float t1y = (max.y - ray.origin.y) * invDir.y;

	// Swap
	if (t0y > t1y)
	{
		float temp = t0y;
		t0y = t1y;
		t1y = temp;
	}


	// Check x on y
	if (t0 > t1y || t0y > t1)
		return false;

	// Swap to get correct min/max
	if (t0y > t0)
		t0 = t0y;
	if (t1y < t1)
		t1 = t1y;


	// Calc y
	float t0z = (min.z - ray.origin.z) * invDir.z;
	float t1z = (max.z - ray.origin.z) * invDir.z;

	// Swap
	if (t0z > t1z)
	{
		float temp = t0z;
		t0z = t1z;
		t1z = temp;
	}


	// Check result on z
	if (t0 > t1z || t0z > t1)
		return false;


	// Swap to get correct min/max
	if (t0z > t0)
		t0 = t0z;
	if (t1z < t1)
		t1 = t1z;

	outDistance = t0 < 0.0f ? t1 : t0;
	return true;
}

bool BoundingBox::Intersects(const BoundingBox& other) const
{
	const vec3 min = GetMin();
	const vec3 max = GetMax();

	const vec3 otherMin = other.GetMin();
	const vec3 otherMax = other.GetMax();

	return !(
		otherMax.x < min.x || otherMax.y < min.y || otherMax.z < min.z || 
		otherMin.x > max.x || otherMin.y > max.y || otherMin.z > max.z
	);
}