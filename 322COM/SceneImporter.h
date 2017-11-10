#pragma once
#include "Scene.h"

#include "rapidjson\document.h"
using JsonDoc = rapidjson::Document;
using JsonValue = rapidjson::Value;


/**
* Handles reading scenes from Json files
*/
static class SceneImporter
{
public:
	/**
	* Builds the default hard-coded scene
	* @param scene			Where to put the scene
	* @returns If the scene correctly imports
	*/
	static void BuildDefaultScene(Scene*& scene);

	/**
	* Attempt to import the scene from a json file
	* @param path			Path to scene file
	* @param scene			Where to put the scene
	* @returns If the scene correctly imports
	*/
	static bool ImportScene(const std::string& path, Scene*& scene);
	/**
	* Attempt to import the scene from a json file
	* @param json			The json scene file
	* @param scene			Where to put the scene
	* @returns If the scene correctly imports
	*/
	static bool ImportScene(const JsonDoc& json, Scene*& scene);
};

