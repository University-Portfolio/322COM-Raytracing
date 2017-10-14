#include "PhysicalMaterial.h"


PhysicalMaterial::~PhysicalMaterial() 
{
	if (m_texture != nullptr)
		delete m_texture;
}

Colour PhysicalMaterial::FetchBaseColour(const Scene* scene, Ray ray, PixelHitInfo& hit)
{
	if(m_texture == nullptr)
		return GetColour();

	Colour c = m_texture->GetColour(hit.uvs.x, hit.uvs.y);
	c.Filter(GetColour());
	return c;
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