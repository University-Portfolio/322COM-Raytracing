#include "TexturedMaterial.h"



TexturedMaterial::TexturedMaterial(std::string texturePath) : m_texture(texturePath)
{
}


TexturedMaterial::~TexturedMaterial()
{
	
}

Colour TexturedMaterial::FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit) 
{
	//return Colour(vec3(hit.uvs.x, hit.uvs.y, 0));

	Colour c = m_texture.GetColour(hit.uvs.x, hit.uvs.y);
	//c.Filter(GetColour());
	return c;
}