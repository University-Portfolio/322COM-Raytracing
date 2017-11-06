#pragma once
#include "Object.h"
#include "Light.h"
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
	std::vector<Light*> m_lights;
	std::vector<Material*> m_materials;
	Colour skyColour;
	float minimumBrightness;

	/// Used to decide whether to render or skip a pixel this frame
	int renderCounter;
	/// Used to cap recursive ray casts
	int maxRecursionLevel = 4;

	/// Used to adjust the quality level over time
	uint lastRenderTime;
	float m_qualityTimer = -1;

	// Used for counting how many pixels need to be rendererd
	uint m_completedRenderCount = 0;
	uint m_totalRenderCount = 0;

	vec3 lastRenderPosition;
	vec3 lastRenderRotation;


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
	* Detect whether the ray hits anything or not
	* @param ray				The ray to cast
	* @param hit				The information about the closest hit
	* @param recursionCount		How many recursive rays have been casted
	* @returns Whether the ray hit anything or not
	*/
	bool CastRay(Ray ray, PixelHitInfo& hit, int recursionCount) const;

	/**
	* Get colour for a single ray
	* @param ray				The ray to cast
	* @param recursionCount		How many recursive rays have been casted
	* @param missColour			What colour to use in the event of a miss
	* @returns The colour that this ray will produce
	*/
	Colour CalculateRayColour(Ray ray, int recursionCount, Colour missColour) const;

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
	void AddObject(Object* obj);
	inline const std::vector<Object*>& GetObjects() const { return m_objects; }

	/** Add material to scene (Forfeits memory management rights to scene) */
	inline Material* AddMaterial(Material* mat) { m_materials.emplace_back(mat); return mat; }
	inline const std::vector<Material*>& GetMaterials() const { return m_materials; }

	/** Add light to scene (Forfeits memory management rights to scene) */
	inline Light* AddLight(Light* light) { m_lights.emplace_back(light); return light; }
	inline const std::vector<Light*>& GetLights() const { return m_lights; }


	inline void SetSkyColour(Colour col) { skyColour = col; }
	inline const Colour& GetSkyColour() const { return skyColour; }


	/**
	* Get the current level of quality at which to render
	* Each level adds new rendering modes from the prior
	* 0 - Boxed, Flat colour
	* 1 - (Optional disable) Texturing
	* 2 - Transparency
	* 3 - Phong, Texel rendering starts
	* 4 - Complex shapes, Pause render after completed frame
	* 5 - Hard shadows
	* 6 - Reflection
	* 7 - Soft shadows
	* 10 - Recursion depth reaches maxium
	*/
	inline int GetRenderingQualityLevel() const { return (int)m_qualityTimer; }

	inline void SetMinimumBrightness(float value) { minimumBrightness = clamp(0.0f, 1.0f, value); }
	inline float GetMinimumBrightness() const { return minimumBrightness; }
};

