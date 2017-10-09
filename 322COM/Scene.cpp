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

	/// Size of output
	int width;
	int height;
	float aspectRatio;

	/// Angle (In radians) 
	float viewAngle;
	vec3 viewRotation;
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

		settings.width = target->GetWidth();
		settings.height = target->GetHeight();
		settings.aspectRatio = (float)target->GetWidth() / (float)target->GetHeight();
		settings.viewAngle = radians(camera->GetFOV());
		settings.viewRotation = radians(camera->GetEularRotation());
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
	const float viewTan = tan(settings->viewAngle * 0.5f);
	float halfWidth = settings->width * 0.5f;
	float halfHeight = settings->height * 0.5f;


	// For each pixel try generate colour
	for (int i = workerId; i < totalPixels; i += m_workerCount)
	{
		int x = i / settings->height; 
		int y = i % settings->height;

		// Perform delayed texel render, to free up more processing
		{
			int l = (renderCounter) % (settings->texelSize * settings->texelSize);
			int xc = (x + l) % settings->texelSize;
			int yc = (y + l / settings->texelSize) % settings->texelSize;

			if (xc != 0 || yc != 0)
				continue;
		}


		// Remap pixels, so 0,0 is centre of screen
		float rx = (x - halfWidth);
		float ry = halfHeight - y;

		// Convert centralized pixels into into direction steps
		float vx = rx / halfWidth * viewTan * settings->aspectRatio;
		float vy = ry / halfHeight * viewTan;
		

		// Fetch colour and draw to screen
		vec3 direction = normalize(
			rotateZ(rotateY(rotateX(vec3(vx, vy, 1), settings->viewRotation.x), settings->viewRotation.y), settings->viewRotation.z)
		);
		Ray ray(settings->cameraLocation, direction);
		Colour currentColour = FetchColour(ray);
		settings->target->SetPixel(x, y, currentColour);
	}
}