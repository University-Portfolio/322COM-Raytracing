#include "PhysicalMaterial.h"


Colour PhysicalMaterial::FetchBaseColour(const Scene* scene, Ray ray, PixelHitInfo& hit)
{
	return GetColour();
}

Colour PhysicalMaterial::FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit)
{
	Colour colour = FetchBaseColour(scene, ray, hit);

	// Don't do complex checks if only simple rendering
	if (scene->IsSimpleRenderingEnabled())
		return colour;


	const std::vector<Light*>& lights = scene->GetLights();
	Colour totalDiffuse;
	Colour totalSpecular;

	for (Light* light : lights)
	{
		Colour colour;
		float specularFactor;
		light->CalculateLighting(scene, ray, hit, colour, specularFactor);

		totalDiffuse += colour;
		totalSpecular += colour * pow(specularFactor, m_shininess) * m_smoothness;
	}


	colour.Filter(totalDiffuse);
	colour += totalSpecular;
	return colour;
}