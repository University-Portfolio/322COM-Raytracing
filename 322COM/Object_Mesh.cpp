#include "Object_Mesh.h"
#include "Logger.h"


Object_Mesh::Object_Mesh(vec3 location)
{
	SetLocation(location);
}


Object_Mesh::~Object_Mesh()
{
}

bool Object_Mesh::IntersectsRay(Ray ray, PixelHitInfo& hitInfo) 
{
	if (m_mesh == nullptr)
		return false;


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


		vec3 eAB = b - a;
		vec3 eAC = c - a;

		float u = dot(ray.origin - a, cross(ray.direction, eAC)) / dot(eAB, cross(ray.direction, eAC));
		float v = dot(ray.direction, cross(ray.origin - a, eAB)) / dot(eAB, cross(ray.direction, eAC));
		float w = 1.0f - u - v;

		if (u < 0 || u > 1 || v < 0 || u + v > 1)
			continue;


		float t = dot(eAC, cross(ray.origin - a, eAB)) / dot(eAB, cross(ray.direction, eAC));


		if (closestHitDistance != -1 && t >= closestHitDistance)
			continue;

		closestHitDistance = t;
		closestTriangle = tri;
		closestUVW = vec3(u, v, w);
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