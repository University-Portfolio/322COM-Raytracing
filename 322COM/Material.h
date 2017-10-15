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
	Colour colour = Colour(1.0f, 1.0f, 1.0f, 1.0f);

public:
	virtual ~Material() {}

	/**
	* What to colour objects which have no material assigned
	* @returns Default draw colour (Pink)
	*/
	static Colour GetDefaultColour() { return Colour(255, 0, 249); }

	/**
	* Fetch the desired colour for an object's pixel in a scene
	* @param scene			The scene that is currently being tested with
	* @param ray			The ray that was previously used to fetch the object
	* @param hit			The hit information about the object/pixel to colour
	* @param recursionCount	How many recursive lookups have occured
	* @returns The correct colour that this pixel should be
	*/
	virtual Colour FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount);

protected:
	/**
	* Resolve any transparency for a given pixel
	* @param baseColour		The basic colour for this pixel
	* @param scene			The scene that is currently being tested with
	* @param ray			The ray that was previously used to fetch the object
	* @param hit			The hit information about the object/pixel to colour
	* @param recursionCount	How many recursive lookups have occured
	* @returns The correct colour that this pixel should be
	*/
	Colour ResolveTransparency(Colour baseColour, const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount);


	/**
	* Getters & Setters
	*/
public:

	inline void SetColour(Colour col) { colour = col; }
	inline const Colour& GetColour() const { return colour; }
};

