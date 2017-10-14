#include "Logger.h"
#include "Window.h"

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"

#include "Object_Sphere.h"
#include "Object_Plane.h"
#include "Object_Mesh.h"

#include "PhysicalMaterial.h"

#include "DirectionalLight.h"


Scene* g_mainScene;
Camera g_camera;


void Tick(Window* context, float deltaTime)
{
	const Keyboard* keyboard = context->GetKeyboard();

	// Camera movement
	{
		vec3 location = g_camera.GetLocation();
		vec3 rotation = g_camera.GetEularRotation();
		const float speed = 3.0f;
		const float rotationSpeed = 45.0f;

		if (keyboard->IsKeyDown(Key::KV_W))
			location += g_camera.GetForward() * deltaTime * speed;
		if (keyboard->IsKeyDown(Key::KV_S))
			location -= g_camera.GetForward() * deltaTime * speed;

		if (keyboard->IsKeyDown(Key::KV_A))
			location -= g_camera.GetRight() * deltaTime * speed;
		if (keyboard->IsKeyDown(Key::KV_D))
			location += g_camera.GetRight() * deltaTime * speed;


		if (keyboard->IsKeyDown(Key::KV_LEFT))
			rotation += vec3(0, -1, 0) * deltaTime * rotationSpeed;
		if (keyboard->IsKeyDown(Key::KV_RIGHT))
			rotation += vec3(0, 1, 0) * deltaTime * rotationSpeed;

		if (keyboard->IsKeyDown(Key::KV_DOWN))
			rotation += vec3(1, 0, 0) * deltaTime * rotationSpeed;
		if (keyboard->IsKeyDown(Key::KV_UP))
			rotation += vec3(-1, 0, 0) * deltaTime * rotationSpeed;

		g_camera.SetLocation(location);
		g_camera.SetEularRotation(rotation);
	}

#ifdef _DEBUG
	g_mainScene->Render(&g_camera, context->GetRenderSurface(), 8);
#else
	g_mainScene->Render(&g_camera, context->GetRenderSurface(), 4);
#endif


	//Texture& t = g_testTexture;
	//t.SetWrapMode(WrapMode::Wrap);

	//for (int x = 0; x < t.GetWidth() * 2; ++x)
	//	for (int y = 0; y < t.GetHeight() * 2; ++y)
	//	{
	//		context->GetRenderSurface()->SetPixel(x, y, t.GetColour(x, y));
	//	}
}

int main(int argc, char** argv)
{
	Window::InitAPI();

	g_mainScene = new Scene(4);
	g_mainScene->SetSkyColour(Colour(52, 152, 219));
	g_camera.SetLocation(vec3(0, 1, 0));


	// Setup materials
	Material* basicColour;
	PhysicalMaterial* basicTexture;
	PhysicalMaterial* tileTexture;
	PhysicalMaterial* physMaterial;
	{
		basicColour = new Material();
		basicColour->SetColour(Colour(0, 255, 255));
		g_mainScene->AddMaterial(basicColour);

		basicTexture = new PhysicalMaterial();
		basicTexture->SetTexture("H:\\Uni\\322COM - Raytracing\\Resources\\Test Texture.bmp");
		basicTexture->GetTexture()->SetFilterMode(FilterMode::Linear);
		basicTexture->GetTexture()->SetWrapMode(WrapMode::Wrap);
		g_mainScene->AddMaterial(basicTexture);

		tileTexture = new PhysicalMaterial();
		tileTexture->SetTexture("H:\\Uni\\322COM - Raytracing\\Resources\\Tile Test.bmp");
		tileTexture->GetTexture()->SetFilterMode(FilterMode::Nearest);
		tileTexture->GetTexture()->SetWrapMode(WrapMode::Wrap);
		tileTexture->SetSmoothness(0.0f);
		g_mainScene->AddMaterial(tileTexture);

		physMaterial = new PhysicalMaterial();
		physMaterial->SetColour(Colour(255, 134, 86));
		g_mainScene->AddMaterial(physMaterial);
	}

	// Setup lights
	{
		DirectionalLight* light = new DirectionalLight(vec3(1, -1, 0));
		//light->SetColour(Colour(255, 0, 0));
		g_mainScene->AddLight(light);
	}
	{
		//DirectionalLight* light = new DirectionalLight(vec3(-0.5f, -1, 0.86602540378f));
		//light->SetColour(Colour(0, 255, 0));
		//g_mainScene->AddLight(light);
	}
	{
		//DirectionalLight* light = new DirectionalLight(vec3(-0.5f, -1, -0.86602540378f));
		//light->SetColour(Colour(0, 0, 255));
		//g_mainScene->AddLight(light);
	}

	// Setup scene
	{
		Object_Plane* plane = new Object_Plane(vec3(0, -5, 0), vec3(0, 1, 0));
		plane->SetMaterial(tileTexture);
		plane->SetCullingMode(CullingMode::Backface);
		g_mainScene->AddObject(plane);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(0, 0, 10), 0.5f);
		sphere->SetMaterial(basicColour);
		g_mainScene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(1, 0, 10), 0.5f);
		sphere->SetMaterial(physMaterial);
		g_mainScene->AddObject(sphere);
	}
	{
		//Object_Sphere* sphere = new Object_Sphere(vec3(-1, 0, 10), 0.5f);
		//sphere->SetCullingMode(CullingMode::Backface);
		//sphere->SetMaterial(physMaterial);
		//g_mainScene->AddObject(sphere);
	}
	{
		//Object_Sphere* sphere = new Object_Sphere(vec3(-1, 0, 20), 5.0f);
		//sphere->SetCullingMode(CullingMode::Backface);
		//sphere->SetMaterial(basicTexture);
		//g_mainScene->AddObject(sphere);
	}

	Mesh mesh;
	Mesh::ImportObj("H:\\Uni\\322COM - Raytracing\\Resources\\torus_low.obj", &mesh, 0.1f);
	{
		Object_Mesh* obj = new Object_Mesh(vec3(0, 0, 4));
		obj->SetCullingMode(CullingMode::Backface);
		obj->SetMesh(&mesh);
		obj->SetMaterial(basicTexture);
		g_mainScene->AddObject(obj);
	}
	{
		//Object_Sphere* sphere = new Object_Sphere(vec3(0, 0, 4), 0.1f);
		//sphere->SetCullingMode(CullingMode::Backface);
		//sphere->SetMaterial(basicColour);
		//g_mainScene->AddObject(sphere);
	}


	Window window("Raytracing - Samuel Potter", 800, 600, 300);
	window.MainLoop(Tick);
	delete g_mainScene;


	Window::DestroyAPI();

#ifdef _DEBUG
	while (true);
#endif
	return 0;
}