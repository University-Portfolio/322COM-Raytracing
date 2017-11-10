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
	Texture* m_texture;

public:
	TexturedMaterial();
	virtual ~TexturedMaterial();

	/**
	* Fetch the desired colour for an object's pixel in a scene
	* @param scene			The scene that is currently being tested with
	* @param ray			The ray that was previously used to fetch the object
	* @param hit			The hit information about the object/pixel to colour
	* @param recursionCount	How many recursive lookups have occured
	* @returns The correct colour that this pixel should be
	*/
	virtual Colour FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount) override;

	/**
	* Getters & Setters
	*/
public:
	inline void SetTexture(Texture* tex) { m_texture = tex; }
	inline Texture* GetTexture() const { return m_texture; }
};

