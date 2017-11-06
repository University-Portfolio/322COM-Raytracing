#include "Object_Mesh.h"
#include "Scene.h"
#include "Logger.h"



Object_Mesh::Object_Mesh(vec3 location)
{
	SetLocation(location);
}

Object_Mesh::~Object_Mesh()
{
}

BoundingBox Object_Mesh::GetAABB() const
{
	if (m_mesh == nullptr)
		return BoundingBox(GetLocation(), vec3(0, 0, 0));

	BoundingBox box = m_mesh->GetAABB();
	box.SetCentreSize(box.GetCentre() + GetLocation(), box.GetSize());
	return box;
}

bool Object_Mesh::IntersectsRay(Ray ray, PixelHitInfo& hitInfo) 
{
	if (m_mesh == nullptr)
		return false;
	
	float aabbDistance;
	if (!GetAABB().Intersects(ray, aabbDistance))
		return false;

	// Don't render full mesh at low quality
	if (GetScene()->GetRenderingQualityLevel() < 4)
	{
		hitInfo.object = this;
		hitInfo.distance = aabbDistance;
		hitInfo.normal = vec3(0, 0, 0);
		hitInfo.uvs = vec2(0, 0);
		hitInfo.location = ray.origin + ray.direction * aabbDistance;
		return true;
	}


	// Information about closest triangle hit
	ivec3 closestTriangle;
	vec3 closestUVW;
	float closestHitDistance = -1;


	// Cache some values (Not really, but at least it makes it more readable I guess)
	vec3 location = GetLocation();
	const std::vector<vec3>& vertices = m_mesh->GetPositions();
	const std::vector<vec3>& normals = m_mesh->GetNormals();
	const std::vector<vec2>& uvs = m_mesh->GetUVs();
	const std::vector<uint>& triangles = m_mesh->GetTriangles();


	// Check all triangles
	for (uint i = 0; i < triangles.size(); i += 3)
	{
		ivec3 tri(
			triangles[i],
			triangles[i + 1],
			triangles[i + 2]
		);

		vec3 a = location + vertices[tri.x];
		vec3 b = location + vertices[tri.y];
		vec3 c = location + vertices[tri.z];


		// Ignore face if it should be culled
		if (GetCullingMode() == CullingMode::Backface)
		{
			vec3 normal = cross(b - a, c - a);
			if (dot(ray.direction, normal) > 0)
				continue;
		}
		else if (GetCullingMode() == CullingMode::Frontface)
		{
			vec3 normal = cross(b - a, c - a);
			if (dot(ray.direction, normal) < 0)
				continue;
		}


		const vec3 eAB = b - a;
		const vec3 eAC = c - a;
		const vec3 origin = ray.origin - a;
		const vec3 dir_c_eAC = cross(ray.direction, eAC);
		const float eAB_d_dir_c_AC = dot(eAB, dir_c_eAC);

		// Due to cross product containing sin() it ends up cancelling out to form equation for area of triangle
		const float u = dot(origin, dir_c_eAC) / eAB_d_dir_c_AC;
		const float v = dot(ray.direction, cross(origin, eAB)) / eAB_d_dir_c_AC;

		// If any area is incorrect then the point must lay outside the triangle
		if (u < 0 || u > 1 || v < 0 || u + v > 1)
			continue;


		const float t = dot(eAC, cross(origin, eAB)) / eAB_d_dir_c_AC;


		if (closestHitDistance != -1 && t >= closestHitDistance)
			continue;

		closestHitDistance = t;
		closestTriangle = tri;
		closestUVW = vec3(u, v, 1.0f - u - v);
	}

	// Didn't hit any triangles
	if(closestHitDistance == -1)
		return false;



	hitInfo.object = this;
	hitInfo.distance = closestHitDistance;
	hitInfo.location = ray.origin + ray.direction * closestHitDistance;

	// Calc normal and uv for this pixel
	{
		vec3 a = location + vertices[closestTriangle.x];
		vec3 b = location + vertices[closestTriangle.y];
		vec3 c = location + vertices[closestTriangle.z];

		hitInfo.normal = normalize(
			normals[closestTriangle.x] * closestUVW.z +
			normals[closestTriangle.y] * closestUVW.x +
			normals[closestTriangle.z] * closestUVW.y
			);

		hitInfo.uvs =
			uvs[closestTriangle.x] * closestUVW.z +
			uvs[closestTriangle.y] * closestUVW.x +
			uvs[closestTriangle.z] * closestUVW.y;
	}
	
	return true;
}