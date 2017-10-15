#pragma once
#include <glm.hpp>
#include "Colour.h"
#include "Ray.h"


using namespace glm;

class Scene;
class PixelHitInfo;



/**
* Base class for any lights which can be placed in the scene
*/
class Light
{
private:
	vec3 location;
	Colour colour = Colour(1.0f, 1.0f, 1.0f);

public:
	Light() {}
	virtual ~Light() {}

	/**
	* Caclulate the lighting for a specific pixel in a scene
	* @param scene			The scene containing all the objects
	* @param ray			The camera ray used to pick the pixel
	* @param hit			The information retrieved about the pixel
	* @param recursionCount	How many recursive lookups have occured
	* @param outColour		The light colour to used for this pixel
	* @param outSpecular	The specular factor for this pixel
	*/
	virtual void CalculateLighting(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount, Colour& outColour, float& outSpecular) const = 0;

	/**
	* Getters & Setters
	*/
public:

	inline void SetLocation(vec3 loc) { location = loc; }
	inline const vec3& GetLocation() const { return location; }

	inline void SetColour(Colour col) { colour = col; }
	inline const Colour& GetColour() const { return colour; }
};

