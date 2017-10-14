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


	// TODO - REMOVE TEST

	// Directional light
	Colour lightColour(255, 255, 255);
	vec3 lightDir = normalize(vec3(1, -1, 0));


	// Diffuse
	Colour diffuse;
	{
		float intensity = max(0.0f, dot(hit.normal, -lightDir));

		if (intensity != 0.0f)
		{

			// Check for shadow
			Colour temp;
			if (scene->CastRay(Ray(hit.location, -lightDir), temp))
				return Colour(0, 0, 0);

			diffuse = lightColour * intensity;
		}
	}

	// Specular
	vec3 specular;
	{
		vec3 reflectedLight = reflect(lightDir, hit.normal);
		float specularFactor = max(0.0f, dot(reflectedLight, -ray.direction));

		specular = lightColour.ToVector() * pow(specularFactor, 10.0f);
	}


	colour.Filter(diffuse);
	colour += specular;
	return colour;
}