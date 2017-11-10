#include "Material.h"
#include "Scene.h"


Colour Material::FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount)
{
	if(scene->GetRenderingQualityLevel() < 2)
		return colour;
	else
		return ResolveTransparency(colour, scene, ray, hit, recursionCount);
}

Colour Material::ResolveTransparency(Colour baseColour, const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount) 
{
	if (colour.a >= 1.0f)
		return baseColour;

	const float alpha = clamp(colour.a, 0.0f, 1.0f);
	const Colour behindColour = scene->CalculateRayColour(Ray(hit.location + ray.direction * 0.001f, ray.direction), recursionCount, scene->GetSkyColour());

	Colour mixed = baseColour * (alpha) + behindColour * (1.0f - alpha);
	mixed.a = 1.0f;
	return mixed;
}