#include "Scene.h"
#include "Camera.h"
#include "RenderSurface.h"

#ifdef GLM_ENABLE_EXPERIMENTAL
#include "gtx\rotate_vector.hpp"
#else
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx\rotate_vector.hpp"
#undef GLM_ENABLE_EXPERIMENTAL
#endif

Scene::~Scene() 
{
	// Delete all objects
	for (Object* obj : m_objects)
		delete obj;
}


void Scene::Render(Camera* camera, RenderSurface* target) 
{
	vec3 forward(0, 0, 1);
	Ray ray(camera->GetLocation(), forward);


	// Work out how far to rotate forward vector based on FoV and screen size
	float aspectRatio = (float)target->GetWidth() / (float)target->GetHeight();

	int halfWidth = target->GetWidth() * 0.5f;
	int halfHeight = target->GetHeight() * 0.5f;

	float fov = radians(camera->GetFOV());
	float stepX = (fov * aspectRatio) / halfWidth;
	float stepY = fov / halfHeight;


	// For each pixel try generate colour
	for (int x = -halfWidth; x < halfWidth; ++x)
		for (int y = -halfHeight; y < halfHeight; ++y)
		{
			vec3 direction = rotateX(rotateY(forward, x * stepX), y * stepY);
			ray.direction = direction;

			Object* hit = nullptr;
			for (Object* obj : m_objects)
			{
				float distance;
				if (obj->IntersectsRay(ray, distance))
				{
					hit = obj;
					break;
				}
			}


			if (hit != nullptr)
				target->SetPixel(halfWidth + x, halfHeight + y, hit->GetColour());
			else
				target->SetPixel(halfWidth + x, halfHeight + y, GetSkyColour());
		}
}