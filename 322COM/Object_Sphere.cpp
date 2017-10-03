#include "Object_Sphere.h"



Object_Sphere::Object_Sphere(vec3 location, float radius) : Object(location)
{
	this->radius = radius;
}

bool Object_Sphere::IntersectsRay(Ray ray, float& outDistance) 
{
	// At time t0 and t1 the ray's distance from 
	// the centre of the sphere will be equal to the radius

	vec3 centre = GetLocation();
	vec3 originToCentre = centre - ray.origin;

	// Calculate hypotenuse of triangle where originToCentre is base and ray direction indicates hypotenuse direction 
	float mainHyp = glm::dot(originToCentre, ray.direction);


	// Ray is not facing towards sphere
	if (mainHyp < 0)
		return false;


	// Pythagorous: Work out side from ray to center (side at right angle)
	// p0 and p1 lie at equal distance from this point in opposite directions along the ray
	float oppositeDistanceSqrd = glm::dot(originToCentre, originToCentre) - mainHyp * mainHyp;


	float radSqrd = radius * radius;

	// Ray passes by outside of sphere
	if (oppositeDistanceSqrd > radSqrd)
		return false;


	// Hit distance from opposite and hyp intersection
	float hitDistance = sqrtf(radSqrd - oppositeDistanceSqrd);


	float t0 = mainHyp - hitDistance;
	float t1 = mainHyp + hitDistance;
	
	// Swap
	if (t1 < t0)
	{
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}
	

	// t0 too close
	if (t0 < 0)
	{
		// t1 not too close
		if (t1 >= 0)
		{
			outDistance = t1;
			return true;
		}
	}
	else
	{
		outDistance = t0;
		return true;
	}
	return false;
}
