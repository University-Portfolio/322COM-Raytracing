#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(vec3 direction)
{
	this->direction = normalize(direction);
}

void DirectionalLight::CalculateLighting(const Scene* scene, Ray ray, PixelHitInfo& hit, Colour& outColour, float& outSpecular) const
{
	outColour = Colour(0, 0, 0);


	float intensity = dot(hit.normal, -direction);


	// Don't bother with lighting attempts if too dim anyway
	if (intensity <= 0.0f)
		return;


	// Check to see if obscured by any object (Should be in shadow)
	Colour temp;
	if (scene->CastRay(Ray(hit.location, -direction), temp))
		return; // TODO - Transparency checks

	outColour = GetColour() * intensity;

	// Calculate specular
	vec3 reflectedLight = reflect(direction, hit.normal);
	outSpecular = max(0.0f, dot(reflectedLight, -ray.direction));
}