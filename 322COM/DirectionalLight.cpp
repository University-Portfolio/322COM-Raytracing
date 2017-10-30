#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(vec3 direction)
{
	this->direction = normalize(direction);
}

void DirectionalLight::CalculateLighting(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount, Colour& outColour, float& outSpecular) const
{
	outColour = Colour(0.0f, 0.0f, 0.0f);


	float intensity = clamp(scene->GetMinimumBrightness(), 1.0f, dot(hit.normal, -direction));


	// Don't bother with lighting attempts if too dim anyway
	if (intensity <= scene->GetMinimumBrightness())
		return;


	// Check to see if obscured by any object (Should be in shadow)
	PixelHitInfo shadowHit;
	Ray shadowRay = Ray(hit.location - direction * 0.01f, -direction);
	if (scene->CastRay(shadowRay, shadowHit, recursionCount))
	{
		Material* mat = shadowHit.object->GetMaterial();
		Colour shadowObjectColour = (mat == nullptr ? Material::GetDefaultColour() : mat->FetchColour(scene, shadowRay, shadowHit, recursionCount));
		outColour = (GetColour() * intensity * (shadowObjectColour.a)) * shadowObjectColour;
	}
	else
		outColour = GetColour() * intensity;


	// Calculate specular
	vec3 reflectedLight = reflect(direction, hit.normal);
	outSpecular = max(0.0f, dot(reflectedLight, -ray.direction));
	outColour.a = 1.0f;
}