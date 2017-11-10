#include "Octree.h"
#include "Logger.h"
#include "Scene.h"


OctreeNode::OctreeNode(const uint32& depth, const int& maxTreeDepth, const vec3& centre, const vec3& size)  :
	m_depth(depth),
	maxDepth(maxTreeDepth),
	m_aabb(centre, size),

	m_node000(m_nodes[0]),
	m_node001(m_nodes[1]),
	m_node010(m_nodes[2]),
	m_node011(m_nodes[3]),
	m_node100(m_nodes[4]),
	m_node101(m_nodes[5]),
	m_node110(m_nodes[6]),
	m_node111(m_nodes[7])
{
}

OctreeNode::~OctreeNode() 
{
	for (OctreeNode*& node : m_nodes)
		delete node;
}

uint32 OctreeNode::Count() const 
{
	uint32 c = 1;
	for (const OctreeNode* node : m_nodes)
		if (node != nullptr)
			c += node->Count();
	return c;
}

bool OctreeNode::Insert(Object* obj) 
{
	BoundingBox objAABB = obj->GetAABB();

	// Doesn't intersect, so ignore
	if (!m_aabb.Intersects(objAABB))
		return false;


	// Is bottom of tree, so add to list
	if (m_depth == maxDepth)
	{
		m_objects.emplace_back(obj);
		return true;
	}


	// Add to child nodes (Until reach bottom node)
	else
	{
		const vec3 centre = m_aabb.GetCentre();
		const vec3 halfSize = m_aabb.GetSize() * 0.5f;
		const vec3 Step = m_aabb.GetSize() * 0.25f;

		// Create child nodes
		for (int i = 0; i < 8; ++i)
		{
			vec3 offset;
			switch (i)
			{
			case 0:	// 000
				offset = vec3(-Step.x, -Step.y, -Step.z);
				break;
			case 1:	// 001
				offset = vec3(-Step.x, -Step.y, Step.z);
				break;
			case 2:	// 010
				offset = vec3(-Step.x, Step.y, -Step.z);
				break;
			case 3:	// 011
				offset = vec3(-Step.x, Step.y, Step.z);
				break;
			case 4:	// 100
				offset = vec3(Step.x, -Step.y, -Step.z);
				break;
			case 5:	// 101
				offset = vec3(Step.x, -Step.y, Step.z);
				break;
			case 6:	// 110
				offset = vec3(Step.x, Step.y, -Step.z);
				break;
			case 7:	// 111
				offset = vec3(Step.x, Step.y, Step.z);
				break;
			}

			if (m_nodes[i] == nullptr)
			{
				m_nodes[i] = new OctreeNode(m_depth + 1, maxDepth, centre + offset, halfSize);
				if (!m_nodes[i]->Insert(obj))
				{
					delete m_nodes[i];
					m_nodes[i] = nullptr;
				}
			}
			else
				m_nodes[i]->Insert(obj);
		}

		// If intersected parent, at least 1 child will also intersect it
		return true;
	}
}

bool OctreeNode::CastRay(const Ray& ray, PixelHitInfo& closestHit, float& closestDistance, const int& renderQuality) const
{
	float aabbDistance;
	if (!m_aabb.Intersects(ray, aabbDistance))
		return false;

	bool hasHit = false;

	// Check objects
	if (m_depth == maxDepth)
	{
		for (Object* obj : m_objects)
		{
			PixelHitInfo hit;
			// Not rendering geometry, so use nodes distance
			if (renderQuality == 0) 
			{
				if (obj->IntersectsRay(ray, hit))
				{
					hit.distance = aabbDistance;
					if ((hit.distance < closestDistance || closestDistance == -1) && hit.distance > 0)
					{
						closestHit = hit;
						closestDistance = hit.distance;
						hasHit = true;
					}
				}
			}

			// Normal render
			else if (obj->IntersectsRay(ray, hit) && (hit.distance < closestDistance || closestDistance == -1) && hit.distance > 0)
			{
				closestHit = hit;
				closestDistance = hit.distance;
				hasHit = true;
			}
		}
	}

	// Check against child nodes
	else
	{
		for (const OctreeNode* node : m_nodes)
			if (node != nullptr)
				hasHit |= node->CastRay(ray, closestHit, closestDistance, renderQuality);
	}

	return hasHit;
	
}


Octree::Octree(const int& maxTreeDepth, Scene* scene)
	: m_treeDepth(maxTreeDepth)
{
	m_scene = scene;
	const std::vector<Object*>& objects = scene->GetObjects();

	LOG("Building Octree for %i geometry", objects.size());

	// Work out world size from objects
	{
		vec3 min(0,0,0);
		vec3 max(0, 0, 0);
		for (Object* obj : objects)
		{
			// Skip unsafe octree objects
			if (!obj->IsOctreeSafe()) 
				continue;

			BoundingBox aabb = obj->GetAABB();
			vec3 obj_min = aabb.GetMin();
			vec3 obj_max = aabb.GetMax();

			if (obj_min.x < min.x) min.x = obj_min.x;
			if (obj_min.y < min.y) min.y = obj_min.y;
			if (obj_min.z < min.z) min.z = obj_min.z;

			if (obj_max.x > max.x) max.x = obj_max.x;
			if (obj_max.y > max.y) max.y = obj_max.y;
			if (obj_max.z > max.z) max.z = obj_max.z;
		}

		const vec3 centre = (min + max) * 0.5f;
		const vec3 size = max - min;

		m_root = new OctreeNode(0, maxTreeDepth, centre, size);
		LOG("\t-World Centre(%f,%f,%f)", centre.x, centre.y, centre.z);
		LOG("\t-World Size(%f,%f,%f)", size.x, size.y, size.z);
	}


	// Build octree from objects
	for (Object* obj : objects)
	{
		if (!obj->IsOctreeSafe())
		{
			m_unsafeList.emplace_back(obj);
			continue;
		}
		else
			m_root->Insert(obj);
	}

	LOG("\t-Node count (%i)", m_root->Count());
	LOG("\t-Tree depth (%i)", maxTreeDepth);
	LOG("\t-NonOctree Geometry (%i)", m_unsafeList.size());
	LOG("Octree Built!");
}

Octree::~Octree() 
{
	delete m_root;
}

bool Octree::CastRay(const Ray& ray, PixelHitInfo& closestHit) const 
{
	bool hasHit = false;
	float closestDistance = -1;
	
	// Check against tree
	hasHit = m_root->CastRay(ray, closestHit, closestDistance, m_scene->GetRenderingQualityLevel());

	// Check against objects not in the tree
	for (Object* obj : m_unsafeList)
	{
		PixelHitInfo hit;
		if (obj->IntersectsRay(ray, hit) && (hit.distance < closestDistance || closestDistance == -1) && hit.distance > 0)
		{
			closestHit = hit;
			closestDistance = hit.distance;
			hasHit = true;
		}
	}

	return hasHit;
}