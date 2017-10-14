#include "TexturedMaterial.h"
#include "Scene.h"



TexturedMaterial::TexturedMaterial(std::string texturePath) : m_texture(texturePath)
{
}


TexturedMaterial::~TexturedMaterial()
{
	
}

Colour TexturedMaterial::FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit)
{
	Colour c = m_texture.GetColour(hit.uvs.x, hit.uvs.y);
	c.Filter(GetColour());
	return c;
}