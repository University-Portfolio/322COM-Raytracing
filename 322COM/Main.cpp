#include "Logger.h"
#include "Window.h"


#include "Timer.h"
#include "Object_Sphere.h"
#include <math.h>
#include <cmath>
Timer timer;


void Tick(Window* context, float deltaTime)
{
	if (!timer.IsRunning())
		timer.Start();

	RenderSurface* surface = context->GetRenderSurface();
	surface->Clear(Colour(0, 0, 0));
	

	const int interval = 5000;
	float t = (float)(timer.GetTimeMilliseconds() % interval) / (float)interval;

	int offset = sinf(t * 3.14159f * 2.0f) * 100;



	const int width = surface->GetWidth();
	const int height = surface->GetHeight();

	int circleCentreX = 400 + offset;
	int circleCentreY = 200;
	int circleRadius = 100;
	int secondCircleRadius = 120;

	Colour trueColour(255 * (1.0f - t), 255 * t, 0);
	Colour falseColour(255, 0, 0);

	Uint32* currentPixel;

	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
		{
			int xpart = x - circleCentreX;
			int ypart = y - circleCentreY;

			if (xpart * xpart + ypart * ypart < circleRadius * circleRadius)
				surface->SetPixel(x, y, trueColour);
			else if (xpart * xpart + ypart * ypart < secondCircleRadius * secondCircleRadius)
				surface->SetPixel(x, y, falseColour);
		}
}

int main(int argc, char** argv)
{
	//Window::InitAPI();

	//Window window("Hello World", 800, 600);
	//window.MainLoop(Tick);

	//Window::DestroyAPI();

	Object_Sphere sphere(vec3(0, 0, 0), 1.0f);
	Ray ray(vec3(0, 0, -10), vec3(0, 0.1, 1));

	float d = 0;
	bool hit = sphere.IntersectsRay(ray, d);
	LOG("Hits? %s %f", (hit ? "true" : "false"), d);

#ifdef _DEBUG
	while (true);
#endif
	return 0;
}