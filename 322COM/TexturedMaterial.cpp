#include "TexturedMaterial.h"
#include "Scene.h"



TexturedMaterial::TexturedMaterial() : m_texture(nullptr)
{
}


TexturedMaterial::~TexturedMaterial()
{
	
}

Colour TexturedMaterial::FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount)
{
	if(m_texture == nullptr)
		return GetColour();
	else
		return m_texture->GetColour(hit.uvs.x, hit.uvs.y) * GetColour();
}