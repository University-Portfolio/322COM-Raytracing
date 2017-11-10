#include "Object_Triangle.h"
#include "Scene.h"


Object_Triangle::Object_Triangle()
{
	SetCullingMode(CullingMode::Backface);
}

std::vector<Object_Triangle*> Object_Triangle::BreakMesh(const Mesh& mesh, const vec3& offset)
{
	std::vector<Object_Triangle*> output;

	const std::vector<vec3>& vertices =		mesh.GetPositions();
	const std::vector<vec3>& normals =		mesh.GetNormals();
	const std::vector<vec2>& uvs =			mesh.GetUVs();
	const std::vector<uint>& triangles =	mesh.GetTriangles();

	
	for (uint i = 0; i < triangles.size(); i += 3)
	{
		ivec3 indices(
			triangles[i],
			triangles[i + 1],
			triangles[i + 2]
			);

		// Populate vertices
		Object_Triangle* tri = new Object_Triangle;

		tri->A.position =	offset + vertices[indices.x];
		tri->A.uv =			uvs[indices.x];
		tri->A.normal =		normals[indices.x];

		tri->B.position =	offset + vertices[indices.y];
		tri->B.uv =			uvs[indices.y];
		tri->B.normal =		normals[indices.y];
		
		tri->C.position =	offset + vertices[indices.z];
		tri->C.uv =			uvs[indices.z];
		tri->C.normal =		normals[indices.z];

		// Cache triangle normal
		tri->m_normal = cross(tri->B.position - tri->A.position, tri->C.position - tri->A.position); 

		// Cache AABB
		vec3 min = tri->A.position;
		vec3 max = tri->A.position;

		// Get min and max to build AABB
		if (tri->B.position.x < min.x) min.x = tri->B.position.x;
		if (tri->B.position.y < min.y) min.y = tri->B.position.y;
		if (tri->B.position.z < min.z) min.z = tri->B.position.z;

		if (tri->B.position.x > max.x) max.x = tri->B.position.x;
		if (tri->B.position.y > max.y) max.y = tri->B.position.y;
		if (tri->B.position.z > max.z) max.z = tri->B.position.z;

		if (tri->C.position.x < min.x) min.x = tri->C.position.x;
		if (tri->C.position.y < min.y) min.y = tri->C.position.y;
		if (tri->C.position.z < min.z) min.z = tri->C.position.z;

		if (tri->C.position.x > max.x) max.x = tri->C.position.x;
		if (tri->C.position.y > max.y) max.y = tri->C.position.y;
		if (tri->C.position.z > max.z) max.z = tri->C.position.z;

		tri->SetLocation(offset);
		tri->m_aabb.SetMinMax(min, max);
		output.emplace_back(tri);
	}

	return output;
}

bool Object_Triangle::IntersectsRay(Ray ray, PixelHitInfo& hitInfo) 
{
	// Box out at low quality
	if (GetScene()->GetRenderingQualityLevel() == 0)
	{
		hitInfo.object = this;
		hitInfo.normal = vec3(0, 0, 0);
		hitInfo.uvs = vec2(0, 0);
		return true;
	}
	
	// Only render AABB, at lower quality
	if (GetScene()->GetRenderingQualityLevel() < 4)
	{
		float aabbDistance;
		if (!m_aabb.Intersects(ray, aabbDistance))
			return false;

		hitInfo.object = this;
		hitInfo.distance = aabbDistance;
		hitInfo.normal = vec3(0, 0, 0);
		hitInfo.uvs = vec2(0, 0);
		hitInfo.location = ray.origin + ray.direction * aabbDistance;
		return true;
	}


	// Ignore face if it should be culled
	if (GetCullingMode() == CullingMode::Backface && dot(ray.direction, m_normal) > 0)
			return false;
	else if (GetCullingMode() == CullingMode::Frontface && dot(ray.direction, m_normal) < 0)
			return false;


	// Calculate actual collision
	const vec3 eAB = B.position - A.position;
	const vec3 eAC = C.position - A.position;
	const vec3 origin = ray.origin - A.position;
	const vec3 dir_c_eAC = cross(ray.direction, eAC);
	const float eAB_d_dir_c_AC = dot(eAB, dir_c_eAC);

	// Due to cross product containing sin() it ends up cancelling out to form equation for area of triangle
	const float u = dot(origin, dir_c_eAC) / eAB_d_dir_c_AC;
	const float v = dot(ray.direction, cross(origin, eAB)) / eAB_d_dir_c_AC;

	// If any area is incorrect then the point must lay outside the triangle
	if (u < 0 || u > 1 || v < 0 || u + v > 1)
		return false;

	const float w = 1.0f - u - v;
	const float t = dot(eAC, cross(origin, eAB)) / eAB_d_dir_c_AC;



	hitInfo.object = this;
	hitInfo.distance = t;
	hitInfo.location = ray.origin + ray.direction * t;
	// Work out normal,uv based on barycentric coords as weightings
	hitInfo.normal = normalize(
		A.normal * w +
		B.normal * u +
		C.normal * v
		);
	hitInfo.uvs = 
		A.uv * w +
		B.uv * u +
		C.uv * v;
	return true;
}

BoundingBox Object_Triangle::GetAABB() const 
{
	return m_aabb;
}