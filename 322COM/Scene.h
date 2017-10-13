#pragma once
#include "Object.h"
#include "ThreadWorker.h"

#include "SDL_stdinc.h"
#include <vector>


class Scene
{
private:
	std::vector<ThreadWorker*> m_workers;
	const int m_workerCount;

	std::vector<Object*> m_objects;
	std::vector<Material*> m_materials;
	Colour skyColour;

	int renderCounter;

public:
	Scene(int workerCount);
	~Scene();

	/**
	* Render this scene to desired target using given camera
	* @param camera			The camera settings to use in this render
	* @param target			The desired rendering surface to draw to
	*/
	void Render(class Camera* camera, class RenderSurface* target, const int texelSize = 8);

	/**
	* Get colour from a single ray
	*/
	Colour CalculateColour(Ray ray) const;

private:
	void HandleRender(int workerId, void* settingsPtr) const;


	/**
	* Getters & Setters
	*/
public:

	/** Add object to scene (Forfeits memory management rights to scene) */
	inline void AddObject(Object* obj) { m_objects.emplace_back(obj); }

	/** Add material to scene (Forfeits memory management rights to scene) */
	inline Material* AddMaterial(Material* mat) { m_materials.emplace_back(mat); return mat; }

	inline void SetSkyColour(Colour col) { skyColour = col; }
	inline Colour GetSkyColour() const { return skyColour; }
};

