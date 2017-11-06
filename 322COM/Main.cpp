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
		float speed = 3.0f;
		float rotationSpeed = 45.0f;

		if (keyboard->IsKeyDown(Key::KV_LSHIFT))
			speed *= 8.0f;


		if (keyboard->IsKeyDown(Key::KV_W))
			location += g_camera.GetForward() * deltaTime * speed;
		if (keyboard->IsKeyDown(Key::KV_S))
			location -= g_camera.GetForward() * deltaTime * speed;

		if (keyboard->IsKeyDown(Key::KV_A))
			location -= g_camera.GetRight() * deltaTime * speed;
		if (keyboard->IsKeyDown(Key::KV_D))
			location += g_camera.GetRight() * deltaTime * speed;

		if (keyboard->IsKeyDown(Key::KV_SPACE))
			location += vec3(0, 1, 0) * deltaTime * speed;
		if (keyboard->IsKeyDown(Key::KV_LCTRL))
			location -= vec3(0, 1, 0) * deltaTime * speed;


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


	g_mainScene->Render(&g_camera, context->GetRenderSurface(), 8);


	//Texture t ("H:\\Uni\\322COM - Raytracing\\Resources\\Test Texture.bmp");
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
	g_mainScene->SetMinimumBrightness(0.1f);
	g_mainScene->SetSkyColour(Colour(52, 152, 219));
	g_camera.SetLocation(vec3(0, 1, 0));


	// Setup materials
	Material* basicColour;
	PhysicalMaterial* basicTexture;
	PhysicalMaterial* tileTexture;
	PhysicalMaterial* physMaterial;
	PhysicalMaterial* reflMaterial;
	{
		basicColour = new Material();
		basicColour->SetColour(Colour(0, 255, 255, 150));
		g_mainScene->AddMaterial(basicColour);

		basicTexture = new PhysicalMaterial();
		basicTexture->SetTexture("..\\Resources\\Test Texture.bmp");
		basicTexture->GetTexture()->SetFilterMode(FilterMode::Linear);
		basicTexture->GetTexture()->SetWrapMode(WrapMode::Wrap);
		basicTexture->SetReflectivity(0.5f);
		g_mainScene->AddMaterial(basicTexture);

		tileTexture = new PhysicalMaterial();
		tileTexture->SetTexture("..\\Resources\\Tile Test.bmp");
		tileTexture->GetTexture()->SetFilterMode(FilterMode::Linear);
		tileTexture->GetTexture()->SetWrapMode(WrapMode::Wrap);
		tileTexture->SetReflectivity(0.0f);
		tileTexture->SetSmoothness(0.0f);
		g_mainScene->AddMaterial(tileTexture);

		physMaterial = new PhysicalMaterial();
		physMaterial->SetColour(Colour(1.0f, 0.0f, 0.0f));
		physMaterial->SetReflectivity(0.7f);
		g_mainScene->AddMaterial(physMaterial);

		reflMaterial = new PhysicalMaterial();
		reflMaterial->SetColour(Colour(1.0f, 0.0f, 1.0f, 0.5f));
		reflMaterial->SetReflectivity(0.5f);
		g_mainScene->AddMaterial(reflMaterial);
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
		Object_Plane* plane = new Object_Plane(vec3(0, 0, 0), vec3(0, 1, 0));
		plane->SetMaterial(tileTexture);
		plane->SetCullingMode(CullingMode::Backface);
		g_mainScene->AddObject(plane);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(0, 0.5f, 10), 0.5f);
		sphere->SetMaterial(basicColour);
		g_mainScene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(1, 0.5f, 10), 0.5f);
		sphere->SetMaterial(physMaterial);
		g_mainScene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(-1, 0.5f, 10), 0.5f);
		sphere->SetMaterial(reflMaterial);
		g_mainScene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(-1, 5.0f, 20), 5.0f);
		sphere->SetMaterial(basicTexture);
		g_mainScene->AddObject(sphere);
	}

	Mesh bunnyMesh;
	Mesh teapotMesh;
	Mesh::ImportObj("..\\Resources\\stanford bunny.obj", &bunnyMesh, 0.1f);
	Mesh::ImportObj("..\\Resources\\teapot.obj", &teapotMesh, 0.1f);

	{
		Object_Mesh* obj = new Object_Mesh(vec3(10, 0, 15));
		obj->SetCullingMode(CullingMode::Backface);
		obj->SetMesh(&bunnyMesh);
		obj->SetMaterial(physMaterial);
		g_mainScene->AddObject(obj);
	}
	{
		Object_Mesh* obj = new Object_Mesh(vec3(-10, 0, 15));
		obj->SetCullingMode(CullingMode::Backface);
		obj->SetMesh(&teapotMesh);
		obj->SetMaterial(reflMaterial);
		g_mainScene->AddObject(obj);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(0, 0.1f, 4), 0.1f);
		sphere->SetCullingMode(CullingMode::Backface);
		sphere->SetMaterial(basicColour);
		g_mainScene->AddObject(sphere);
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