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
	Colour skyColour;

public:
	Scene(int workerCount);
	~Scene();

	/**
	* Render this scene to desired target using given camera
	* @param camera			The camera settings to use in this render
	* @param target			The desired rendering surface to draw to
	*/
	void Render(class Camera* camera, class RenderSurface* target);

	/**
	* Get colour from a single ray
	*/
	Colour FetchColour(Ray ray);

private:
	void HandleRender(int workerId, void* settingsPtr);


	/**
	* Getters & Setters
	*/
public:

	/** Add object to scene (Forfeits memory management rights to scene) */
	inline void AddObject(Object* obj) { m_objects.emplace_back(obj); }

	inline void SetSkyColour(Colour col) { skyColour = col; }
	inline Colour GetSkyColour() { return skyColour; }
};

