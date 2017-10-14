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
		m_workers.emplace_back(new ThreadWorker(this, i));

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

	// Delete all materials
	for (Material* mat : m_materials)
		delete mat;

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


Colour Scene::CalculateColour(Ray ray) const
{
	// Find closest hit
	PixelHitInfo closestHit;
	float closestDistance = -1;

	for (Object* obj : m_objects)
	{
		PixelHitInfo hit;
		if (obj->IntersectsRay(ray, hit) && (hit.distance < closestDistance || closestDistance == -1) && hit.distance > 0)
		{
			closestHit = hit;
			closestDistance = hit.distance;
		}
	}

	
	// No hit so get sky
	if (closestHit.object == nullptr)
		return GetSkyColour();


	Material* mat = closestHit.object->GetMaterial();

	// Missing material so return pink
	if (mat == nullptr)
		return Colour(255, 0, 249);

	return mat->FetchColour(this, ray, closestHit);
}

void Scene::ExecuteWork(int workerId, void* data)
{
	RenderSettings* settings = (RenderSettings*)data;
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
			rotateY(rotateX(rotateZ(vec3(vx, vy, 1), settings->viewRotation.z), settings->viewRotation.x), settings->viewRotation.y)
		);
		Ray ray(settings->cameraLocation, direction);
		Colour currentColour = CalculateColour(ray);
		settings->target->SetPixel(x, y, currentColour);
	}
}