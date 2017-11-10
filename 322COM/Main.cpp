#include "Logger.h"
#include "Window.h"

#include "Scene.h"
#include "Camera.h"
#include "SceneImporter.h"


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
	rapidjson::Document json;

	Window::InitAPI();

	LOG("Controls:");
	LOG("\tWS: \tForward/Back");
	LOG("\tAD: \tLeft/Right");
	LOG("\tSpace: \tUp");
	LOG("\tLCtrl: \tDown");
	LOG("\tArrows: \t\tLook");
	LOG("\tLShift: \t\tSpeed up (Sprint)");


	g_mainScene = new Scene(4);
	g_mainScene->SetMinimumBrightness(0.1f);
	g_camera.SetLocation(vec3(0, 1, 0));

	std::string scenePath = "Scenes\\material-testing.scene.json";

	if (!SceneImporter::ImportScene(scenePath, g_mainScene))
	{
		LOG_WARNING("Failed to import '%s', loading default", scenePath.c_str());
		SceneImporter::BuildDefaultScene(g_mainScene);
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