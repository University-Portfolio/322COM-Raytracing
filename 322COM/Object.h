#pragma once
#include <glm.hpp>
#include "Colour.h"
#include "Ray.h"
#include "Material.h"
#include "BoundingBox.h"


using namespace glm;


/**
* Container for all the information about a pixel hit 
*/
struct PixelHitInfo 
{
	class Object* object = nullptr;
	float distance = 0.0f;

	vec3 location;
	vec3 normal;
	vec2 uvs;
};


enum CullingMode
{
	Nothing,
	Frontface,
	Backface
};


/**
* Abstract Object represents something which can be placed in the scene
*/
class Object
{
private:
	class Scene* scene;

	vec3 location;
	Material* material;

	CullingMode m_cullingMode = CullingMode::Backface;

public:
	virtual ~Object() {}

	/**
	* Does this ray interesect with this object?
	* @param ray			The ray in question
	* @param hitInfo		Information about the ray intersection
	* @returns True if ray hits, false if misses
	*/
	virtual bool IntersectsRay(Ray ray, PixelHitInfo& hitInfo) = 0;

	/**
	* Get the AABB for this specific object
	* @return Object centred bounding box
	*/
	virtual BoundingBox GetAABB() const = 0;

	/**
	* Getters & Setters
	*/
public:
	inline void SetScene(Scene* s) { scene = s; }
	inline Scene* GetScene() const { return scene; }

	inline void SetLocation(vec3 loc) { location = loc; }
	inline vec3 GetLocation() const { return location; }

	inline void SetMaterial(Material* mat) { material = mat; }
	inline Material* GetMaterial() const { return material; }

	inline void SetCullingMode(CullingMode mode) { m_cullingMode = mode; }
	inline CullingMode GetCullingMode() const { return m_cullingMode; }
};