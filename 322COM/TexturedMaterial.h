#pragma once
#include "Material.h"
#include "Object.h"
#include "Texture.h"


/**
* Material for applying a single material to an object
*/
class TexturedMaterial : public Material
{
private:
	Texture m_texture;

public:
	TexturedMaterial(std::string texturePath);
	virtual ~TexturedMaterial();


	/**
	* Fetch the desired colour for an object's pixel in a scene
	* @param scene			The scene that is currently being tested with
	* @param ray			The ray that was previously used to fetch the object
	* @param hit			The hit information about the object/pixel to colour
	* @returns The correct colour that this pixel should be
	*/
	virtual Colour FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit) override;

	/**
	* Getters & Setters
	*/
public:
	inline Texture* GetTexture() { return &m_texture; }
};

