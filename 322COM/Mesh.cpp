#include "Mesh.h"

#include <fstream>
#include <cstring>
#include <unordered_map>

#include "Logger.h"


/**
* Define required functions to use glm ivec3 as map key
*/
struct MapKeyFunctions
{
	size_t operator()(const ivec3& k)const
	{
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
	}

	bool operator()(const ivec3& a, const ivec3& b)const
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};


Mesh::Mesh()
{
}

bool Mesh::ImportObj(std::string path, Mesh* outTarget, float scale)
{
	std::vector<vec3> rawPositions;
	std::vector<vec3> rawNormals;
	std::vector<vec2> rawUvs;
	std::vector<ivec3> rawTriangles;


	std::ifstream file(path);
	if (!file.good())
		return false;


	// Read through, line by line
	std::string line;
	while (std::getline(file, line)) 
	{
		//Ignore comments and objects (All objects are merged into 1)
		if (line.length() == 0 || line[0] == '#' || line[0] == 'g')
			continue;

		std::string identifier = line.substr(0, 2);


		// Vertex
		if (identifier == "v ")
		{
			vec3 v(0, 0, 0);
			sscanf_s(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);
			rawPositions.emplace_back(v);
		}
		// UV
		else if (identifier == "vt")
		{
			vec2 v(0, 0);
			sscanf_s(line.c_str(), "vt %f %f", &v.x, &v.y);
			rawUvs.emplace_back(v);
		}
		// Normal
		else if (identifier == "vn")
		{
			vec3 v(0, 0, 0);
			sscanf_s(line.c_str(), "vn %f %f %f", &v.x, &v.y, &v.z);
			rawNormals.emplace_back(v);
		}
		// Face
		else if (identifier == "f ")
		{
			// Assume importing a quad mesh then get ignore last, if is triangularized
			ivec3 a(0, 0, 0);
			ivec3 b(0, 0, 0);
			ivec3 c(0, 0, 0);
			ivec3 d(0, 0, 0);
			int read = sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&a.x, &a.y, &a.z,
				&b.x, &b.y, &b.z,
				&c.x, &c.y, &c.z,
				&d.x, &d.y, &d.z
			);

			// Is quad mesh
			if (read == 12)
			{
				rawTriangles.emplace_back(a);
				rawTriangles.emplace_back(b);
				rawTriangles.emplace_back(c);

				rawTriangles.emplace_back(a);
				rawTriangles.emplace_back(c);
				rawTriangles.emplace_back(d); // <- TODO CHECK THIS WORKS
			}

			// Is tri mesh
			else if(read == 9)
			{
				rawTriangles.emplace_back(a);
				rawTriangles.emplace_back(b);
				rawTriangles.emplace_back(c);
			}

			// Unsupported face format
			else
			{
				LOG_ERROR("Unsupported obj face format when attempting to import '%s'", path.c_str());
				file.close();
				return false;
			}
		}
	}

	file.close();

	// Build triangles
	std::vector<vec3> positions;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;
	std::vector<uint> triangles;

	bool bSetMinMax = false;
	vec3 maxPosition;
	vec3 minPosition;

	positions.reserve(rawPositions.size());
	normals.reserve(rawNormals.size());
	uvs.reserve(rawUvs.size());
	triangles.reserve(rawTriangles.size());


	// Use map for compression
	std::unordered_map<ivec3, uint, MapKeyFunctions, MapKeyFunctions> compressionTable;

	for (ivec3 tri : rawTriangles)
	{
		// Already has entry for this triangle, so reuse that rather than make a new one
		if (compressionTable.find(tri) != compressionTable.end())
			triangles.emplace_back(compressionTable[tri]);
		
		// Add a new entry
		else
		{
			uint index = positions.size();

			vec3 pos = rawPositions[tri.x - 1] * scale;
			positions.emplace_back(pos);

			// Update min max
			if (!bSetMinMax)
			{
				minPosition = pos;
				maxPosition = pos;
				bSetMinMax = true;
			}
			else
			{
				if (pos.x < minPosition.x) minPosition.x = pos.x;
				if (pos.y < minPosition.y) minPosition.y = pos.y;
				if (pos.z < minPosition.z) minPosition.z = pos.z;

				if (pos.x > maxPosition.x) maxPosition.x = pos.x;
				if (pos.y > maxPosition.y) maxPosition.y = pos.y;
				if (pos.z > maxPosition.z) maxPosition.z = pos.z;
			}

			uvs.emplace_back(rawUvs[tri.y - 1]);
			normals.emplace_back(rawNormals[tri.z - 1]);

			triangles.emplace_back(index);
			compressionTable[tri] = index;
		}
	}

	positions.shrink_to_fit();
	normals.shrink_to_fit();
	uvs.shrink_to_fit();
	triangles.shrink_to_fit();

	outTarget->Clear();
	outTarget->SetPositions(positions);
	outTarget->SetNormals(normals);
	outTarget->SetUVs(uvs);
	outTarget->SetTriangles(triangles);
	outTarget->m_aabb.SetMinMax(minPosition, maxPosition);
	LOG("Loaded obj mesh from path '%s'", path.c_str());
	return true;
}