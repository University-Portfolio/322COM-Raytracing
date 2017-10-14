#include "Object_Sphere.h"
#include "Logger.h"


Object_Sphere::Object_Sphere(vec3 location, float radius)
{
	SetLocation(location);
	this->radius = radius;
}

bool Object_Sphere::IntersectsRay(Ray ray, PixelHitInfo& hitInfo)
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
		

	// Get closest (Ignore if too close)
	float t = 0;

	switch (GetCullingMode())
	{
	// Get closest of 2 intersections
	case Nothing:
		t = t0;
		if (t < 0)
			t = t1;
		break;

	case Backface:
		t = t0;
		break;

	case Frontface:
		t = t1;
		break;
	}

	// If too close, ignore hit
	if (t < 0)
		return false;


	hitInfo.object = this;
	hitInfo.distance = t;
	hitInfo.location = ray.origin + ray.direction * t;
	hitInfo.normal = normalize(hitInfo.location - centre);
	
	// Project uvs
	const float pi = 3.141592f;
	const float tau = 3.141592f * 2.0f;
	hitInfo.uvs = vec2(
		atan2f(-hitInfo.normal.x, hitInfo.normal.z) / tau + 0.5f,
		0.5f + hitInfo.normal.y * -0.5f
	);

	return true;
}
