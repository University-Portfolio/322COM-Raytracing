#include "Logger.h"
#include "Window.h"

#include "Scene.h"
#include "Camera.h"
#include "Object_Sphere.h"


Scene* g_mainScene;
Camera g_camera;

static float timer = 0.0f;
static bool bMove = false;

void Tick(Window* context, float deltaTime)
{
	timer -= deltaTime;
	if (timer < 0.0f)
	{
		bMove = !bMove;
		timer = bMove ? 4.0f : 4.0f;
	}

	//if(bMove)
		g_camera.SetLocation(g_camera.GetLocation() + vec3(0, 0.0f, 0.5f) * deltaTime);

	g_mainScene->Render(&g_camera, context->GetRenderSurface(), 2);
}

int main(int argc, char** argv)
{
	Window::InitAPI();

	g_mainScene = new Scene(4);
	g_mainScene->SetSkyColour(Colour(52, 152, 219));
	g_camera.SetLocation(vec3(0, 1, 0));
	g_camera.SetFOV(90);

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