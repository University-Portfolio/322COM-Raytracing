#include "Logger.h"
#include "Window.h"

#include "Scene.h"
#include "Camera.h"
#include "Object_Sphere.h"


Scene g_mainScene;
Camera g_camera;


void Tick(Window* context, float deltaTime)
{
	g_camera.SetLocation(g_camera.GetLocation() + vec3(0, 0.1f, 1) * deltaTime);

	g_mainScene.Render(&g_camera, context->GetRenderSurface());
}

int main(int argc, char** argv)
{
	Window::InitAPI();

	// Setup scene
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(0, 0, 10), 0.5f);
		sphere->SetColour(Colour(255, 0, 0));
		g_mainScene.AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(1, 0, 10), 0.5f);
		sphere->SetColour(Colour(0, 255, 0));
		g_mainScene.AddObject(sphere);
	}
	{
		Object_Sphere* sphere = new Object_Sphere(vec3(-1, 0, 10), 0.5f);
		sphere->SetColour(Colour(0, 0, 255));
		g_mainScene.AddObject(sphere);
	}


	Window window("Hello World", 800, 600);
	window.MainLoop(Tick);


	Window::DestroyAPI();

#ifdef _DEBUG
	while (true);
#endif
	return 0;
}