#include "Logger.h"
#include "Window.h"


static float t = 0;

void Tick(Window* context, float deltaTime)
{
	t += deltaTime;

	if (t >= 1.0f)
	{
		t -= 1.0f;
		LOG("Size %ix%i", context->GetWidth(), context->GetHeight());
	}
}

int main(int argc, char** argv)
{
	Window::InitAPI();

	Window window("Hello World", 800, 600);
	window.MainLoop(Tick);

	Window::DestroyAPI();
	return 0;
}