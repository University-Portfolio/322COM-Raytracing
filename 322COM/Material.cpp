#include "Material.h"
#include "Scene.h"


Colour Material::FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount)
{
	if(scene->IsSimpleRenderingEnabled())
		return colour;
	else
		return ResolveTransparency(colour, scene, ray, hit, recursionCount);
}

Colour Material::ResolveTransparency(Colour baseColour, const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount) 
{
	if (colour.a >= 1.0f)
		return baseColour;

	const float alpha = clamp(0.0f, 1.0f, colour.a);
	const Colour behindColour = scene->CalculateRayColour(Ray(hit.location + ray.direction * 0.001f, ray.direction), recursionCount, Colour(1.0f, 1.0f, 1.0f, 1.0f));

	Colour mixed = baseColour * (1.0f - alpha) + behindColour * alpha;
	mixed.a = 1.0f;
	return mixed;
}