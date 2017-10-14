#pragma once
#include "Material.h"
#include "Scene.h"
#include "Object.h"


/**
* Material that will respond to physical properties e.g. Shadows, lighting, reflections etc.
*/
class PhysicalMaterial : public Material
{
public:
	/**
	* Fetch the desired base colour (Not affected by physics) for an object's pixel in a scene
	* @param scene			The scene that is currently being tested with
	* @param ray			The ray that was previously used to fetch the object
	* @param hit			The hit information about the object/pixel to colour
	* @returns The correct colour that this pixel should be
	*/
	virtual Colour FetchBaseColour(const Scene* scene, Ray ray, PixelHitInfo& hit);

	/**
	* Fetch the desired colour for an object's pixel in a scene
	* @param scene			The scene that is currently being tested with
	* @param ray			The ray that was previously used to fetch the object
	* @param hit			The hit information about the object/pixel to colour
	* @returns The correct colour that this pixel should be
	*/
	virtual Colour FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit) override;
};

