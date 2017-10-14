#pragma once
#include "Light.h"
#include "Scene.h"
#include "Object.h"


class DirectionalLight : public Light
{
private:
	vec3 direction;

public:
	DirectionalLight(vec3 direction = vec3(0, -1, 0));

	/**
	* Caclulate the lighting for a specific pixel in a scene
	* @param scene			The scene containing all the objects
	* @param ray			The camera ray used to pick the pixel
	* @param hit			The information retrieved about the pixel
	* @param outColour		The light colour to used for this pixel
	* @param outSpecular	The specular factor for this pixel
	*/
	virtual void CalculateLighting(const Scene* scene, Ray ray, PixelHitInfo& hit, Colour& outColour, float& outSpecular) const;

	/**
	* Getters & Setters
	*/
public:
	inline void SetDirection(vec3 dir) { direction = normalize(dir); }
	inline vec3 GetDirection() const { return direction; }
};

