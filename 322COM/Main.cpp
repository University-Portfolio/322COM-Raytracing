#include "Logger.h"
#include "Window.h"

#include "Scene.h"
#include "Camera.h"
#include "Object_Sphere.h"


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

	g_mainScene->Render(&g_camera, context->GetRenderSurface(), 8);
}

int main(int argc, char** argv)
{
	Window::InitAPI();

	g_mainScene = new Scene(4);
	g_mainScene->SetSkyColour(Colour(52, 152, 219));
	g_camera.SetLocation(vec3(0, 1, 0));

	// Setup scene
	{
		//Object_Sphere* sphere = new Object_Sphere(vec3(0, 0, 10), 0.5f);
		//sphere->SetColour(Colour(255, 0, 0));
		//g_mainScene->AddObject(sphere);
	}
	{
		//Object_Sphere* sphere = new Object_Sphere(vec3(1, 0, 10), 0.5f);
		//sphere->SetColour(Colour(0, 255, 0));
		//g_mainScene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(-1, 0, 10), 0.5f);
		sphere->SetColour(Colour(0, 0, 255));
		g_mainScene->AddObject(sphere);
	}
	{
		//Object_Sphere* sphere = new Object_Sphere(vec3(-1, 0, 20), 5.0f);
		//sphere->SetColour(Colour(255, 255, 255));
		//g_mainScene->AddObject(sphere);
	}


	Window window("Hello World", 800, 600, 300);
	window.MainLoop(Tick);
	delete g_mainScene;


	Window::DestroyAPI();

#ifdef _DEBUG
	while (true);
#endif
	return 0;
}