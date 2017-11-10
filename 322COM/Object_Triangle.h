#pragma once
#include "Object.h"
#include "Mesh.h"
#include <vector>


/**
* Contains all data pertaining to a vertex (Used with triangles)
*/
struct VertexData
{
	vec3 position;
	vec2 uv;
	vec3 normal;
};


class Object_Triangle : public Object
{
private:
	VertexData A;
	VertexData B;
	VertexData C;
	vec3 m_normal;
	BoundingBox m_aabb;

	Object_Triangle();
public:
	/**
	* Break apart a mesh into many triangle objects
	* @param mesh			The mesh to break up
	* @param offset			The offset to apply to the triangles
	* @returns List of all newly create triangles
	*/
	static std::vector<Object_Triangle*> BreakMesh(const Mesh& mesh, const vec3& offset);


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
	* Getters & Setters
	*/
public:
};

