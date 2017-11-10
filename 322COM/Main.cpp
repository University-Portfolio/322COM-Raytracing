#include "Logger.h"
#include "Window.h"

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"

#include "Object_Sphere.h"
#include "Object_Plane.h"
#include "Object_Mesh.h"
#include "Object_Triangle.h"

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

		// Get 2D movement vectors
		vec3 forward = g_camera.GetForward();
		vec3 right = g_camera.GetRight();
		forward.y = 0;
		forward = normalize(forward);
		right.y = 0;
		right = normalize(right);

		float speed = 3.0f;
		float rotationSpeed = 45.0f;

		if (keyboard->IsKeyDown(Key::KV_LSHIFT))
			speed *= 8.0f;


		if (keyboard->IsKeyDown(Key::KV_W))
			location += forward * deltaTime * speed;
		if (keyboard->IsKeyDown(Key::KV_S))
			location -= forward * deltaTime * speed;

		if (keyboard->IsKeyDown(Key::KV_A))
			location -= right * deltaTime * speed;
		if (keyboard->IsKeyDown(Key::KV_D))
			location += right * deltaTime * speed;

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
	
	g_mainScene->Render(&g_camera, context->GetRenderSurface(), 16);
}


int main(int argc, char** argv)
{
	Window::InitAPI();

	LOG("Controls:");
	LOG("\tWS: \tForward/Back");
	LOG("\tAD: \tLeft/Right");
	LOG("\tSpace: \tUp");
	LOG("\tLCtrl: \tDown");
	LOG("\tArrows: \tLook");
	LOG("\tLShift: \tSpeed up (Sprint)");


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
	PhysicalMaterial* testMaterial;
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
		reflMaterial->SetRefractionIndex(1.52f);
		//reflMaterial->SetReflectivity(0.5f);
		g_mainScene->AddMaterial(reflMaterial);

		testMaterial = new PhysicalMaterial();
		testMaterial->SetColour(Colour(1.0f, 1.0f, 1.0f));
		testMaterial->SetReflectivity(1.0f);
		g_mainScene->AddMaterial(testMaterial);
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
		std::vector<Object_Triangle*> tris = Object_Triangle::BreakMesh(teapotMesh, vec3(10, 0, 15));
		for (Object_Triangle* tri : tris)
		{
			tri->SetMaterial(testMaterial);
			g_mainScene->AddObject(tri);
		}

		//Object_Mesh* obj = new Object_Mesh(vec3(10, 0, 15));
		//obj->SetCullingMode(CullingMode::Nothing);
		//obj->SetMesh(&bunnyMesh);
		//obj->SetMaterial(testMaterial);
		//g_mainScene->AddObject(obj);
	}
	{


		//Object_Mesh* obj = new Object_Mesh(vec3(-10, 0, 15));
		//obj->SetCullingMode(CullingMode::Backface);
		//obj->SetMesh(&teapotMesh);
		//obj->SetMaterial(reflMaterial);
		//g_mainScene->AddObject(obj);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(0, 0.1f, 4), 0.1f);
		sphere->SetCullingMode(CullingMode::Backface);
		sphere->SetMaterial(basicColour);
		g_mainScene->AddObject(sphere);
	}


	Window window("Raytracing - Samuel Potter", 800, 600, 300);
	//Window window("Raytracing - Samuel Potter", 1280, 720, 300);
	window.MainLoop(Tick);
	delete g_mainScene;


	Window::DestroyAPI();

#ifdef _DEBUG
	while (true);
#endif
	return 0;
}