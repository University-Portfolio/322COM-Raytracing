#pragma once
#include "Object.h"
#include "ThreadWorker.h"

#include "SDL_stdinc.h"
#include <vector>


/**
* Stores all objects, lights and resources pertaining to a scene
* Also rendering will be handled through this
*/
class Scene : public IWorkable
{
private:
	std::vector<ThreadWorker*> m_workers;
	const int m_workerCount;

	std::vector<Object*> m_objects;
	std::vector<Material*> m_materials;
	Colour skyColour;

	int renderCounter;
	bool bSimpleRenderingEnabled = false;

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
	* @param ray		The ray to cast
	* @param outColour	The colour that this pixel should be
	* @returns Whether the ray hit anything or not
	*/
	bool CastRay(Ray ray, Colour& outColour) const;

private:
	/**
	* Callback for when a work request comes in (Render request)
	* @param workerId		The id of the worker this is being executed by
	* @param data			The data to process (Rendering settings)
	*/
	virtual void ExecuteWork(int workerId, void* data);


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

	/** Should complex rendering be enabled (e.g. lighting, relections etc.) */
	inline bool IsSimpleRenderingEnabled() const { return bSimpleRenderingEnabled; }
};

