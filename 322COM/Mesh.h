#pragma once
#include <string>
#include <glm.hpp>
#include <vector>

#include "BoundingBox.h"


using namespace glm;


class Mesh
{
private:
	std::vector<vec3> m_positions;
	std::vector<vec3> m_normals;
	std::vector<vec2> m_uvs;
	std::vector<uint> m_triangles;
	BoundingBox m_aabb;

public:
	Mesh();

	/**
	* Attempt to import an obj model at the given path
	* @param path			The URL for the obj model
	* @param outTarget		The mesh to import the data onto
	* @param scale			How much to scale the mesh by
	* @returns True if import is successful
	*/
	static bool ImportObj(std::string path, Mesh* outTarget, float scale = 1.0f);


	/**
	* Getters and Setters
	*/
public:
	inline void Clear() 
	{
		m_positions.clear();
		m_normals.clear();
		m_uvs.clear();
		m_triangles.clear();
	}

	inline void SetPositions(std::vector<vec3> positions) { m_positions = positions; }
	inline const std::vector<vec3>& GetPositions() const { return m_positions; }

	inline void SetNormals(std::vector<vec3> normals) { m_normals = normals; }
	inline const std::vector<vec3>& GetNormals() const { return m_normals; }

	inline void SetUVs(std::vector<vec2> uvs) { m_uvs = uvs; }
	inline const std::vector<vec2>& GetUVs() const { return m_uvs; }

	inline void SetTriangles(std::vector<uint> triangles) { m_triangles = triangles; }
	inline const std::vector<uint>& GetTriangles() const { return m_triangles; }

	inline const BoundingBox& GetAABB() { return m_aabb; }
};

