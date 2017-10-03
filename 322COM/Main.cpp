#include "Logger.h"
#include "Window.h"


static float t = 0;

void Tick(Window* context, float deltaTime)
{
	RenderSurface* surface = context->GetRenderSurface();

	surface->Clear(Colour(0, 0, 0));
	

	const int width = surface->GetWidth();
	const int height = surface->GetHeight();

	int circleCentreX = 200;
	int circleCentreY = 200;
	int circleRadius = 100;
	int secondCircleRadius = 120;

	Colour trueColour(0, 255, 0);
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
	Window::InitAPI();

	Window window("Hello World", 800, 600);
	window.MainLoop(Tick);

	Window::DestroyAPI();

#ifdef _DEBUG
	while (true);
#endif
	return 0;
}