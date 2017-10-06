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
	const float speed = 1.0f;

	if (keyboard->IsKeyDown(Key::KV_W))
		g_camera.SetLocation(g_camera.GetLocation() + vec3(0, 0, 1) * deltaTime * speed);
	if (keyboard->IsKeyDown(Key::KV_S))
		g_camera.SetLocation(g_camera.GetLocation() + vec3(0, 0, -1) * deltaTime * speed);

	if (keyboard->IsKeyDown(Key::KV_A))
		g_camera.SetLocation(g_camera.GetLocation() + vec3(-1, 0, 0) * deltaTime * speed);
	if (keyboard->IsKeyDown(Key::KV_D))
		g_camera.SetLocation(g_camera.GetLocation() + vec3(1, 0, 0) * deltaTime * speed);

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
		Object_Sphere* sphere = new Object_Sphere(vec3(0, 0, 10), 0.5f);
		sphere->SetColour(Colour(255, 0, 0));
		g_mainScene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(1, 0, 10), 0.5f);
		sphere->SetColour(Colour(0, 255, 0));
		g_mainScene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(-1, 0, 10), 0.5f);
		sphere->SetColour(Colour(0, 0, 255));
		g_mainScene->AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(-1, 0, 20), 5.0f);
		sphere->SetColour(Colour(255, 255, 255));
		g_mainScene->AddObject(sphere);
	}


	Window window("Hello World", 1280, 720, 300);
	window.MainLoop(Tick);
	delete g_mainScene;


	Window::DestroyAPI();

#ifdef _DEBUG
	while (true);
#endif
	return 0;
}