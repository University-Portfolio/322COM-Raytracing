#include "Object_Plane.h"



Object_Plane::Object_Plane(vec3 location, vec3 normal)
{
	SetLocation(location);
	bIsOctreeSafe = false;
	this->normal = normalize(normal);
}

BoundingBox Object_Plane::GetAABB() const
{
	return BoundingBox(GetLocation(), vec3(extent, extent, extent));
}

bool Object_Plane::IntersectsRay(Ray ray, PixelHitInfo& hitInfo) 
{
	vec3 origin = GetLocation() - ray.origin;
	float den = dot(ray.direction, normal);

	// Ray is parralel with the plane
	if (den == 0.0f)
		return false;
	
	// Culled desired side
	switch (GetCullingMode())
	{
	case Nothing:
		break;

	case Backface:
		if (dot(ray.direction, normal) > 0)
			return false;
		break;

	case Frontface:
		if (dot(ray.direction, normal) < 0)
			return false;
		break;
	}


	// Work out distance from intersection
	float t = dot(origin, normal) / den;


	hitInfo.object = this;
	hitInfo.distance = t;
	hitInfo.location = ray.origin + ray.direction * t;
	hitInfo.normal = normal;

	// Source https://stackoverflow.com/questions/18663755/how-to-convert-a-3d-point-on-a-plane-to-uv-coordinates
	vec3 u = normalize(vec3(normal.y, -normal.x, 0));
	vec3 v = cross(normal, u);

	hitInfo.uvs = vec2(
		dot(u, hitInfo.location) * uvScale.x,
		dot(v, hitInfo.location) * uvScale.y
		);
	return true;
}
