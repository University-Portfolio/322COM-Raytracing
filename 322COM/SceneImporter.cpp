#define _CRT_SECURE_NO_DEPRECATE
#include <cstdio>
#include <unordered_map>
#include "rapidjson\filereadstream.h"

#include "SceneImporter.h"
#include "Logger.h"
#include "Mesh.h"


#include "Object_Plane.h"
#include "Object_Mesh.h"
#include "Object_Sphere.h"
#include "Object_Triangle.h"

#include "TexturedMaterial.h"
#include "PhysicalMaterial.h"

#include "DirectionalLight.h"


#define JSON_ASSERT(name, type) \
	if(!name.Is ## type ## ()) \
	{ \
		LOG_ERROR(#name " must be a " #type); \
		return false; \
	}


template<typename T>
static bool TryGet(const JsonValue& v, const char* key, T& out)
{
	static_assert(false, "No TryGet implementation");
	/*
	auto it = v.FindMember(key);
	if (it == v.MemberEnd() || !it->value.Is<T>())
		return false;
	out = it->value.Get<T>();
	return true;
	*/
	return false;
}
template<>
static bool TryGet(const JsonValue& v, const char* key, float& out)
{
	auto it = v.FindMember(key);
	if (it == v.MemberEnd() || !it->value.IsNumber())
		return false;
	out = it->value.GetFloat();
	return true;
}
template<>
static bool TryGet(const JsonValue& v, const char* key, std::string& out)
{
	auto it = v.FindMember(key);
	if (it == v.MemberEnd() || !it->value.IsString())
		return false;
	out = it->value.GetString();
	return true;
}
template<>
static bool TryGet(const JsonValue& v, const char* key, int& out)
{
	auto it = v.FindMember(key);
	if (it == v.MemberEnd() || !it->value.IsNumber())
		return false;
	out = it->value.GetInt();
	return true;
}
template<>
static bool TryGet(const JsonValue& v, const char* key, bool& out)
{
	auto it = v.FindMember(key);
	if (it == v.MemberEnd() || !it->value.IsBool())
		return false;
	out = it->value.GetBool();
	return true;
}
template<>
static bool TryGet(const JsonValue& v, const char* key, Colour& out)
{
	auto it = v.FindMember(key);
	if (it == v.MemberEnd() || !it->value.IsArray())
		return false;

	uint i = 0;
	for (const auto& v : it->value.GetArray())
	{
		if (v.IsNumber())
		{
			const float f = v.GetFloat();
			switch (i++)
			{
			case 0:
				out.r = f;
				break;
			case 1:
				out.g = f;
				break;
			case 2:
				out.b = f;
				break;
			case 3:
				out.a = f;
				break;
			default:
				return true;
			}
		}
	}

	return true;
}
template<>
static bool TryGet(const JsonValue& v, const char* key, vec3& out)
{
	auto it = v.FindMember(key);
	if (it == v.MemberEnd() || !it->value.IsArray())
		return false;

	uint i = 0;
	for (const auto& v : it->value.GetArray())
	{
		if (v.IsNumber())
		{
			const float f = v.GetFloat();
			switch (i++)
			{
			case 0:
				out.x = f;
				break;
			case 1:
				out.y = f;
				break;
			case 2:
				out.z = f;
				break;
			default:
				return true;
			}
		}
	}

	return true;
}
template<>
static bool TryGet(const JsonValue& v, const char* key, vec2& out)
{
	auto it = v.FindMember(key);
	if (it == v.MemberEnd() || !it->value.IsArray())
		return false;

	uint i = 0;
	for (const auto& v : it->value.GetArray())
	{
		if (v.IsNumber())
		{
			const float f = v.GetFloat();
			switch (i++)
			{
			case 0:
				out.x = f;
				break;
			case 1:
				out.y = f;
				break;
			default:
				return true;
			}
		}
	}

	return true;
}



void SceneImporter::BuildDefaultScene(Scene*& scene)
{
	scene->SetSkyColour(Colour(52, 152, 219));


	// Setup materials
	Material* basicColour;
	PhysicalMaterial* basicTexture;
	PhysicalMaterial* tileTexture;
	PhysicalMaterial* physMaterial;
	PhysicalMaterial* reflMaterial;
	PhysicalMaterial* testMaterial;
	{
		Texture* testTex = new Texture("Resources\\Test Texture.bmp");
		Texture* tileTex = new Texture("Resources\\Tile Test.bmp");
		scene->AddTexture(testTex);
		scene->AddTexture(tileTex);

		basicColour = new Material();
		basicColour->SetColour(Colour(0, 255, 255, 150));
		scene->AddMaterial(basicColour);

		basicTexture = new PhysicalMaterial();
		basicTexture->SetTexture(testTex);
		basicTexture->GetTexture()->SetFilterMode(FilterMode::Linear);
		basicTexture->GetTexture()->SetWrapMode(WrapMode::Wrap);
		basicTexture->SetReflectivity(0.5f);
		scene->AddMaterial(basicTexture);

		tileTexture = new PhysicalMaterial();
		tileTexture->SetTexture(tileTex);
		tileTexture->GetTexture()->SetFilterMode(FilterMode::Linear);
		tileTexture->GetTexture()->SetWrapMode(WrapMode::Wrap);
		tileTexture->SetReflectivity(0.0f);
		tileTexture->SetSmoothness(0.0f);
		scene->AddMaterial(tileTexture);

		physMaterial = new PhysicalMaterial();
		physMaterial->SetColour(Colour(1.0f, 0.0f, 0.0f));
		physMaterial->SetReflectivity(0.7f);
		scene->AddMaterial(physMaterial);

		reflMaterial = new PhysicalMaterial();
		reflMaterial->SetColour(Colour(1.0f, 0.0f, 1.0f, 0.5f));
		reflMaterial->SetRefractionIndex(1.52f);
		//reflMaterial->SetReflectivity(0.5f);
		scene->AddMaterial(reflMaterial);

		testMaterial = new PhysicalMaterial();
		testMaterial->SetColour(Colour(1.0f, 1.0f, 1.0f));
		testMaterial->SetReflectivity(1.0f);
		scene->AddMaterial(testMaterial);
	}

	// Setup lights
	{
		DirectionalLight* light = new DirectionalLight(vec3(1, -1, 0));
		//light->SetColour(Colour(255, 0, 0));
		scene->AddLight(light);
	}
	{
		//DirectionalLight* light = new DirectionalLight(vec3(-0.5f, -1, 0.86602540378f));
		//light->SetColour(Colour(0, 255, 0));
		//scene->AddLight(light);
	}
	{
		//DirectionalLight* light = new DirectionalLight(vec3(-0.5f, -1, -0.86602540378f));
		//light->SetColour(Colour(0, 0, 255));
		//scene->AddLight(light);
	}

	// Setup scene
	{
		Object_Plane* plane = new Object_Plane(vec3(0, 0, 0), vec3(0, 1, 0));
		plane->SetMaterial(tileTexture);
		plane->SetCullingMode(CullingMode::Backface);
		scene->AddObject(plane);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(0, 0.5f, 10), 0.5f);
		sphere->SetMaterial(basicColour);
		scene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(1, 0.5f, 10), 0.5f);
		sphere->SetMaterial(physMaterial);
		scene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(-1, 0.5f, 10), 0.5f);
		sphere->SetMaterial(reflMaterial);
		scene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(-1, 5.0f, 20), 5.0f);
		sphere->SetMaterial(basicTexture);
		scene->AddObject(sphere);
	}

	Mesh bunnyMesh;
	Mesh teapotMesh;
	Mesh::ImportObj("Resources\\Stanford\\bunny.obj", &bunnyMesh, 0.1f);
	Mesh::ImportObj("Resources\\Stanford\\dragon.obj", &teapotMesh, 0.1f);
	{
		std::vector<Object_Triangle*> tris = Object_Triangle::BreakMesh(bunnyMesh, vec3(10, 0, 15));
		for (Object_Triangle* tri : tris)
		{
			tri->SetMaterial(testMaterial);
			scene->AddObject(tri);
		}
	}
	{
		std::vector<Object_Triangle*> tris = Object_Triangle::BreakMesh(teapotMesh, vec3(-10, 0, 15));
		for (Object_Triangle* tri : tris)
		{
			tri->SetCullingMode(CullingMode::Nothing);
			tri->SetMaterial(reflMaterial);
			scene->AddObject(tri);
		}
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(0, 0.1f, 4), 0.1f);
		sphere->SetCullingMode(CullingMode::Backface);
		sphere->SetMaterial(basicColour);
		scene->AddObject(sphere);
	}

}

bool SceneImporter::ImportScene(const std::string& path, Scene*& scene)
{
	LOG("Loading scene from '%s'", path.c_str());

	FILE* file = fopen(path.c_str(), "rb");

	char buffer[65536];
	rapidjson::FileReadStream is(file, buffer, sizeof(buffer));

	JsonDoc json;
	json.ParseStream(is);

	return ImportScene(json, scene);
}

bool SceneImporter::ImportScene(const JsonDoc& json, Scene*& scene)
{
	JSON_ASSERT(json, Object);
	std::unordered_map<std::string, Texture*>	textureTable;
	std::unordered_map<std::string, Mesh*>		meshTable;
	std::unordered_map<std::string, Material*>	matTable;

	// Import assets
	{
		LOG("Importing assets:");
		const JsonValue& assets = json["Assets"];
		JSON_ASSERT(assets, Object);

		// Import textures
		{
			const JsonValue& textures = assets["Textures"];
			JSON_ASSERT(textures, Object);


			for (JsonValue::ConstMemberIterator it = textures.MemberBegin(); it != textures.MemberEnd(); ++it)
			{
				// Ignore any invalid types
				if (!it->value.IsObject())
					continue;

				const std::string key = it->name.GetString();
				std::string url;
				std::string filter;
				bool wrap;

				if (!TryGet(it->value, "URL", url))
				{
					LOG_ERROR("You must provide a URL in order to import a texture (%s)", key.c_str());
					continue;
				}
				Texture* texture = new Texture(url);

				if (TryGet(it->value, "Filter", filter))
				{
					if (filter == "LINEAR")
						texture->SetFilterMode(FilterMode::Linear);
					else if (filter == "NEAREST")
						texture->SetFilterMode(FilterMode::Nearest);
					else
						LOG_WARNING("Unknown texture filter type '%s'", filter.c_str());
				}

				if (TryGet(it->value, "Wrap", wrap))
					texture->SetWrapMode(wrap ? WrapMode::Wrap : WrapMode::None);


				textureTable[it->name.GetString()] = scene->AddTexture(texture);
			}
		}
		LOG("\tImported %i textures..", textureTable.size());


		// Import materials
		{
			const JsonValue& materials = assets["Materials"];
			JSON_ASSERT(materials, Object);


			for (JsonValue::ConstMemberIterator it = materials.MemberBegin(); it != materials.MemberEnd(); ++it)
			{
				// Ignore any invalid types
				if (!it->value.IsObject())
					continue;

				const std::string key = it->name.GetString();
				Material* material;
				std::string type;
				Colour colour;

				if (!TryGet(it->value, "Colour", colour))
					colour = Colour(1.0f, 1.0f, 1.0f, 1.0f);
				if (!TryGet(it->value, "Type", type))
				{
					LOG_ERROR("You must provide a Type in order to import a material (%s)", key.c_str());
					continue;
				}
				
				if (type == "PHYSICAL")
				{
					PhysicalMaterial* mat = new PhysicalMaterial;
					std::string	texture;
					float		smoothness;
					float		shininess;
					float		reflectivity;
					float		refraction;

					if (TryGet(it->value, "Texture", texture))
						mat->SetTexture(textureTable[texture]);
					if (TryGet(it->value, "Smoothness", smoothness))
						mat->SetSmoothness(smoothness);
					if (TryGet(it->value, "Shininess", shininess))
						mat->SetShininess(shininess);
					if (TryGet(it->value, "Reflectivity", reflectivity))
						mat->SetReflectivity(reflectivity);
					if (TryGet(it->value, "RefractionIndex", refraction))
						mat->SetRefractionIndex(refraction);

					material = mat;
				}
				else if (type == "TEXTURED")
				{
					TexturedMaterial* mat = new TexturedMaterial;
					std::string	texture;

					if (TryGet(it->value, "Texture", texture))
						mat->SetTexture(textureTable[texture]);

					material = mat;
				}
				else
				{
					if (type != "FLAT")
						LOG_WARNING("Unknown material type '%s' defaulting to flat", type.c_str());
					material = new Material;
				}

				material->SetColour(colour);
				matTable[it->name.GetString()] = scene->AddMaterial(material);
			}
		}
		LOG("\tImported %i materials..", matTable.size());


		// Import models
		{
			const JsonValue& models = assets["Models"];
			JSON_ASSERT(models, Object);


			for (JsonValue::ConstMemberIterator it = models.MemberBegin(); it != models.MemberEnd(); ++it)
			{
				// Ignore any invalid types
				if (!it->value.IsObject())
					continue;

				const std::string key = it->name.GetString();
				std::string url;
				float scale = 1.0f;

				TryGet(it->value, "Scale", scale);
				if (!TryGet(it->value, "URL", url))
				{
					LOG_ERROR("You must provide a URL in order to import a mesh (%s)", key.c_str());
					continue;
				}

				Mesh* mesh = new Mesh;
				Mesh::ImportObj(url, mesh, scale);

				meshTable[it->name.GetString()] = mesh;
			}
		}
		LOG("\tImported %i meshes..", meshTable.size());
		LOG("Imported assets.");
	}


	// Import scene
	{
		LOG("Importing scene:");
		const JsonValue& sceneJson = json["Scene"];
		JSON_ASSERT(sceneJson, Object);


		// Set sky colour
		Colour skyColour(52, 152, 219);
		TryGet(sceneJson, "SkyColour", skyColour);
		scene->SetSkyColour(skyColour);

		// Import lights
		{
			const JsonValue& lights = sceneJson["Lights"];
			JSON_ASSERT(lights, Array);

			for (const auto& val : lights.GetArray())
			{
				// Ignore any invalid types
				if (!val.IsObject())
					continue;


				Light* light;
				std::string type;
				vec3 location;
				Colour colour;

				if (!TryGet(val, "Type", type))
				{
					LOG_ERROR("You must provide a type in order to import a light");
					continue;
				}

				TryGet(val, "Location", location);
				TryGet(val, "Colour", colour);

				if (type != "DIRECTIONAL")
					LOG_WARNING("Unknown light type '%s' defaulting to directional", type.c_str());

				{
					vec3 dir(0, -1, 0);
					TryGet(val, "Direction", dir);
					DirectionalLight* dl = new DirectionalLight(dir);
					light = dl;
				}


				light->SetLocation(location);
				light->SetColour(colour);
				scene->AddLight(light);
			}
		}

		// Import objects
		{
			const JsonValue& objects = sceneJson["Objects"];
			JSON_ASSERT(objects, Array);

			for (const auto& val : objects.GetArray())
			{
				// Ignore any invalid types
				if (!val.IsObject())
					continue;


				Object* object;
				std::string type;
				std::string material;
				vec3 location;

				if (!TryGet(val, "Type", type))
				{
					LOG_ERROR("You must provide a type in order to import an object");
					continue;
				}

				TryGet(val, "Location", location);

				// Setup object
				if (type == "PLANE")
				{
					vec3 normal(0,1,0);
					float extent;
					vec2 uvScale;

					TryGet(val, "Normal", normal);
					Object_Plane* plane = new Object_Plane(location, normal);

					if (TryGet(val, "Extent", extent))
						plane->SetExtent(extent);
					if (TryGet(val, "UVScale", uvScale))
						plane->SetUVScale(uvScale);

					object = plane;
				}

				// Mesh splits mesh into individual triangles, so needs unqiue handle
				else if (type == "MESH")
				{
					// Get material
					Material* mat = nullptr;
					CullingMode culling = CullingMode::Backface;
					if (TryGet(val, "Material", material))
					{
						mat = matTable[material];
						if (mat != nullptr && mat->GetColour().a < 1.0f) // Override culling mode
							culling = CullingMode::Nothing;
					}
					
					std::string meshName;
					
					if (TryGet(val, "Model", meshName))
					{
						Mesh* mesh = meshTable[meshName];
						if (mesh == nullptr)
							continue;

						// Add each triangle separately
						for (Object_Triangle* tri : Object_Triangle::BreakMesh(*mesh, location))
						{
							tri->SetMaterial(mat);
							tri->SetCullingMode(culling);
							scene->AddObject(tri);
						}
					}

					// Don't continue with normal logic, as we've added many rather than one
					continue;
				}
				else if (type == "SLOW_MESH")
				{
					Object_Mesh* mo = new Object_Mesh;
					mo->SetLocation(location);
					std::string meshName;

					if (TryGet(val, "Model", meshName))
					{
						Mesh* mesh = meshTable[meshName];
						mo->SetMesh(mesh);
					}

					object = mo;
				}
				else
				{
					if (type != "SPHERE")
						LOG_WARNING("Unknown object type '%s' defaulting to sphere", type.c_str());

					float radius = 1.0f;
					TryGet(val, "Radius", radius);
					Object_Sphere* sphere = new Object_Sphere(location, radius);
					object = sphere;
				}


				// Setup material
				if (TryGet(val, "Material", material))
				{
					Material* mat = matTable[material];
					if (mat != nullptr && mat->GetColour().a < 1.0f) // Override culling mode
						object->SetCullingMode(CullingMode::Nothing);
					object->SetMaterial(mat);
				}


				scene->AddObject(object);
			}
		}
		LOG("Imported scene.");
	}

	LOG("Succesfully loaded scene from json.");

	// Delete meshes, as they're no longer needed
	for (auto it : meshTable)
		delete it.second;

	return true;
}