#pragma once
#include "Object.h"
#include "Mesh.h"


class Object_Mesh : public Object
{
private:
	Mesh* m_mesh = nullptr;

public:
	Object_Mesh(vec3 location = vec3(0, 0, 0));
	virtual ~Object_Mesh();

	/**
	* Does this ray interesect with this object?
	* @param ray			The ray in question
	* @param hitInfo		Information about the ray intersection
	* @returns True if ray hits, false if misses
	*/
	virtual bool IntersectsRay(Ray ray, PixelHitInfo& hitInfo);

	/**
	* Get the AABB for this specific object
	* @return Object centred bounding box
	*/
	virtual BoundingBox GetAABB() const;

	/**
	* Getters and Setters
	*/
public:
	inline void SetMesh(Mesh* mesh) { m_mesh = mesh; }
	inline Mesh* GetMesh() { return m_mesh; }
};

