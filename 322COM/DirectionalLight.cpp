#include "DirectionalLight.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx\rotate_vector.hpp"


DirectionalLight::DirectionalLight(vec3 direction)
{
	this->direction = normalize(direction);
}

void DirectionalLight::CalculateLighting(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount, Colour& outColour, float& outSpecular) const
{
	outColour = Colour(0.0f, 0.0f, 0.0f);

	float intensity = clamp(dot(hit.normal, -direction), scene->GetMinimumBrightness(), 1.0f);



	// Check to see if obscured by any object (Should be in shadow)
	if(scene->GetRenderingQualityLevel() >= 5)
	{
		PixelHitInfo shadowHit;

		Colour totalShadowColour;
		uint totalShadowHits = 0;

		const int checkHalfSize = scene->GetRenderingQualityLevel() >= 7 ? 2 : 0;
		const int totalChecks = (checkHalfSize * checkHalfSize + 1) * (checkHalfSize * checkHalfSize + 1);
		const float checkSize = 1.0f / (float)totalChecks;


		// Apply soft shadows
		for (int dx = -checkHalfSize; dx <= checkHalfSize; ++dx)
			for (int dy = -checkHalfSize; dy <= checkHalfSize; ++dy)
			{
				// Do check by offsetting as point light
				vec3 dir = dx == 0 && dy == 0 ? -direction : rotateX(rotateZ(-direction, dy * checkSize), dx * checkSize);

				Ray shadowRay = Ray(hit.location + dir * 0.01f, dir);
				if (scene->CastRay(shadowRay, shadowHit, recursionCount))
				{
					Material* mat = shadowHit.object->GetMaterial();
					Colour shadowColour = (mat == nullptr ? Material::GetDefaultColour() : mat->GetColour());

					// Use alpha as weights for shadow colour blending
					float alpha = shadowColour.a;
					shadowColour = shadowColour * (1.0f - alpha);
					shadowColour.a = alpha;

					totalShadowColour += shadowColour;
					++totalShadowHits;
				}
			}

		// Blend the shadow colours
		if (totalShadowHits != 0)
		{
			totalShadowColour /= totalShadowHits;
			totalShadowColour.r *= intensity;
			totalShadowColour.g *= intensity;
			totalShadowColour.b *= intensity;
			totalShadowColour = totalShadowColour.Clamp();
			totalShadowColour *= GetColour();

			const Colour defaultColour = GetColour() * intensity;

			// Resolve transparency
			totalShadowColour = totalShadowColour * (totalShadowColour.a) + defaultColour * (1.0f - totalShadowColour.a);

			// Blur shadow edges
			const float shadowLevel = clamp((float)totalShadowHits / (float)(totalChecks), 0.0f, 1.0f - scene->GetMinimumBrightness());
			outColour = totalShadowColour * shadowLevel + defaultColour * (1.0f - shadowLevel);
		}
		else
			outColour = GetColour() * intensity;
	}
	else
		outColour = GetColour() * intensity;


	// Calculate specular
	vec3 reflectedLight = reflect(direction, hit.normal);
	outSpecular = max(0.0f, dot(reflectedLight, -ray.direction));
	outColour.a = 1.0f;
}
