#include "Material.h"


Colour Material::FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount)
{
	// Default impl. just return material colour
	return colour;
}