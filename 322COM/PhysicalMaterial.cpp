#include "PhysicalMaterial.h"


PhysicalMaterial::~PhysicalMaterial() 
{
}

Colour PhysicalMaterial::FetchBaseColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount)
{
	Colour colour;
	if(m_texture == nullptr)
		return GetColour();
	else
		return m_texture->GetColour(hit.uvs.x, hit.uvs.y) * GetColour();
}

Colour PhysicalMaterial::FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount)
{
	Colour baseColour = FetchBaseColour(scene, ray, hit, recursionCount);;

	// Don't do complex checks if only simple rendering
	if (scene->GetRenderingQualityLevel() < 2)
		return baseColour;
	else if (scene->GetRenderingQualityLevel() < 3)
		return ResolveTransparency(baseColour, scene, ray, hit, recursionCount);


	// Get reflection colour
	if (scene->GetRenderingQualityLevel() >= 6 && m_reflectivity != 0.0f)
	{
		vec3 reflRay = reflect(ray.direction, hit.normal);
		Colour reflColour = scene->CalculateRayColour(Ray(hit.location + reflRay * 0.01f, reflRay), recursionCount + 1, scene->GetSkyColour());
		reflColour.a = baseColour.a;
		baseColour = baseColour * (1.0f - m_reflectivity) + (baseColour * reflColour) * m_reflectivity;
	}


	// Calc lighting
	if (scene->GetRenderingQualityLevel() >= 3)
	{
		const std::vector<Light*>& lights = scene->GetLights();
		Colour totalDiffuse;
		Colour totalSpecular;

		for (Light* light : lights)
		{
			Colour colour;
			float specularFactor;
			light->CalculateLighting(scene, ray, hit, recursionCount, colour, specularFactor);

			totalDiffuse += colour;
			totalSpecular += colour * pow(specularFactor, m_shininess) * m_smoothness;
		}


		// Apply specular
		Colour colour = baseColour * (1.0f - baseColour.a) + (baseColour * totalDiffuse * (baseColour.a));
		colour += totalSpecular;
		baseColour = colour;
	}


	if (m_refractionIndex != 1.0f)
	{
		Ray refractedRay = Ray(ray.origin, refract(ray.direction, hit.normal, m_refractionRatio));
		return ResolveTransparency(baseColour, scene, refractedRay, hit, recursionCount);
	}
	else
		return ResolveTransparency(baseColour, scene, ray, hit, recursionCount);
}