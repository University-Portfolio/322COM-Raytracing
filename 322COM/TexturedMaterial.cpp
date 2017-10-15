#include "TexturedMaterial.h"
#include "Scene.h"



TexturedMaterial::TexturedMaterial(std::string texturePath) : m_texture(texturePath)
{
}


TexturedMaterial::~TexturedMaterial()
{
	
}

Colour TexturedMaterial::FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount)
{
	return m_texture.GetColour(hit.uvs.x, hit.uvs.y) * GetColour();
}