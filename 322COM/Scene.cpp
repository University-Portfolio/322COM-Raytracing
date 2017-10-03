#include "Scene.h"
#include "Camera.h"
#include "RenderSurface.h"
#include "Logger.h"


#ifdef GLM_ENABLE_EXPERIMENTAL
#include "gtx\rotate_vector.hpp"
#else
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx\rotate_vector.hpp"
#undef GLM_ENABLE_EXPERIMENTAL
#endif



struct RenderSettings
{
public:
	RenderSurface* target;
	vec3 cameraLocation;
	vec3 cameraForward;

	int texelSize;
	int width;
	int height;

	float stepX;
	float stepY;
};



Scene::Scene(int workerCount) : m_workerCount(workerCount)
{
	m_workers.reserve(m_workerCount);

	// Create enough workers
	for (int i = 0; i < m_workerCount; ++i)
	{
		ThreadWorker* worker = new ThreadWorker([this, i](void* ptr) { this->HandleRender(i, ptr); });
		m_workers.emplace_back(worker);
	}

	LOG("Created %i workers for Scene rendering", m_workerCount);
}

Scene::~Scene() 
{
	// Delete all workers
	for (ThreadWorker* worker : m_workers)
		delete worker;

	// Delete all objects
	for (Object* obj : m_objects)
		delete obj;

	LOG("Destroyed scene");
}


void Scene::Render(Camera* camera, RenderSurface* target, int renderTexelSize)
{
	RenderSettings settings;

	// Generate desired rendering settings
	{
		settings.target = target;
		settings.texelSize = renderTexelSize;
		settings.cameraLocation = camera->GetLocation();
		settings.cameraForward = vec3(0, 0, 1);


		// Work out how far to rotate forward vector based on FoV and screen size
		float aspectRatio = (float)target->GetWidth() / (float)target->GetHeight();

		settings.width = target->GetWidth();
		settings.height = target->GetHeight();

		float halfWidth = settings.width * 0.5f;
		float halfHeight = settings.height * 0.5f;

		const float fov = radians(camera->GetFOV() * 0.5f);
		settings.stepX = (fov * aspectRatio) / halfWidth;
		settings.stepY = fov / halfHeight;
	}


	// Queue work for all workers
	for (ThreadWorker* worker : m_workers)
		worker->QueueWork(&settings);

	// Wait until all workers are finished
	bool bIsWorking = true;

	while (bIsWorking)
	{
		bIsWorking = false;
		for (ThreadWorker* worker : m_workers)
			if (worker->IsWorkerActive())
			{
				bIsWorking = true;
				break;
			}

		if(bIsWorking)
			SDL_Delay(1);
	}

	++renderCounter;
}


Colour Scene::FetchColour(Ray ray) const
{
	Object* hit = nullptr;
	float distance = -1;

	for (Object* obj : m_objects)
	{
		float objDist;
		if (obj->IntersectsRay(ray, objDist) && (objDist < distance || distance == -1) && objDist > 0)
		{
			hit = obj;
			distance = objDist;
		}
	}
	
	if (hit != nullptr)
	{
		vec3 hitLocation = ray.origin + ray.direction * distance;
		vec3 diff = normalize(hitLocation - hit->GetLocation());

		return Colour(diff.x * 255, diff.y * 255, diff.z * 255);
		//return hit->GetColour();
	}
	else
		return GetSkyColour();
}


void Scene::HandleRender(int workerId, void* settingsPtr) const
{
	RenderSettings* settings = (RenderSettings*)settingsPtr;
	if (settings == nullptr)
		return;
	

	const int totalPixels = settings->width * settings->height;
	float halfWidth = settings->width * 0.5f;
	float halfHeight = settings->height * 0.5f;


	// For each pixel try generate colour
	for (int i = workerId; i < totalPixels; i += m_workerCount)
	{
		int x = i / settings->height; 
		int y = i % settings->height;

		// Perform checkered board render, to free up more processing
		int l = (renderCounter) % (settings->texelSize * settings->texelSize);
		int xc = (x + l) % settings->texelSize;
		int yc = (y + l / settings->texelSize) % settings->texelSize;

		if (xc != 0 || yc != 0)
			continue;

		vec3 direction = rotateX(rotateY(settings->cameraForward, (x - halfWidth) * settings->stepX), (y - halfHeight) * settings->stepY);
		Ray ray(settings->cameraLocation, direction);
		Colour currentColour = FetchColour(ray);
		settings->target->SetPixel(x, y, currentColour);
	}
}