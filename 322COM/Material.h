#pragma once
#include "Colour.h"
#include "Ray.h"


class Scene;
struct PixelHitInfo;

/**
* Represents some form of visual/physical material with any amount of settings
*/
class Material
{
private:
	Colour colour = Colour(255, 255, 255);

public:
	virtual ~Material() {}


	/**
	* Fetch the desired colour for an object's pixel in a scene
	* @param scene			The scene that is currently being tested with
	* @param ray			The ray that was previously used to fetch the object
	* @param hit			The hit information about the object/pixel to colour
	* @returns The correct colour that this pixel should be
	*/
	virtual Colour FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit) const;


	/**
	* Getters & Setters
	*/
public:

	inline void SetColour(Colour col) { colour = col; }
	inline Colour GetColour() const { return colour; }
};

