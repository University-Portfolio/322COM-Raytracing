#pragma once
#include <vector>
#include "Object.h"


/**
* Represents a node that is used in tree branches
*/
class OctreeNode
{
private:
	const uint32			m_depth;
	const uint32			maxDepth;
	const BoundingBox		m_aabb;
	std::vector<Object*>	m_objects;

protected:
	// Allocate from array for easier looping
	OctreeNode* m_nodes[8]{ nullptr };

	OctreeNode*& m_node000; // back bottom left
	OctreeNode*& m_node001; // back bottom right
	OctreeNode*& m_node010; // back top left
	OctreeNode*& m_node011; // back top right
	OctreeNode*& m_node100; // front bottom left
	OctreeNode*& m_node101; // front bottom right
	OctreeNode*& m_node110; // front top left
	OctreeNode*& m_node111; // front top right

public:
	OctreeNode(const uint32& depth, const int& maxTreeDepth, const vec3& centre, const vec3& size);
	~OctreeNode();

	/**
	* Insert this object into the tree
	* @param obj				Object we would like to put into the tree
	*/
	bool Insert(Object* obj);

	/**
	* Detect whether the ray hits anything or not
	* @param ray				The ray to cast
	* @param closestHit			The information about the closest hit
	* @param closestDistance	The distance of the current closest hit (-1 if not currently hit)
	* @param renderQuality		The current quality level of the render
	* @returns Whether the ray hit anything or not
	*/
	bool CastRay(const Ray& ray, PixelHitInfo& closestHit, float& closestDistance, const int& renderQuality) const;

	/** Count how many nodes are connected to this one */
	uint32 Count() const;
	inline const BoundingBox& GetAABB() const { return m_aabb; }
}; 


/**
* Represents the entire oct-tree structure
* Allows dev to insert objects into a tree for partition world for faster lookup scalability
*/
class Octree
{
private:
	class Scene* m_scene;
	const int m_treeDepth;

	// All objects which cannot be safely put into an octree
	std::vector<Object*> m_unsafeList;
	OctreeNode* m_root;

public:
	Octree(const int& maxTreeDepth, Scene* scene);
	~Octree();

	/**
	* Detect whether the ray hits anything or not
	* @param ray				The ray to cast
	* @param closestHit			The information about the closest hit
	* @returns Whether the ray hit anything or not
	*/
	bool CastRay(const Ray& ray, PixelHitInfo& closestHit) const;
};

