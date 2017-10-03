#include "Logger.h"
#include "Window.h"


static float t = 0;

void Tick(float deltaTime)
{
	t += deltaTime;

	if (t >= 1.0f)
	{
		t -= 1.0f;
		LOG("Yett");
	}
}


int main(int argc, char** argv)
{
	Window window("Hello World", 800, 600);
	window.MainLoop(Tick);
	
	return 0;
}