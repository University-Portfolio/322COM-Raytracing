#include "Window.h"

#include <chrono>
#include "Logger.h"


Window::Window(std::string title, int width, int height)
{
	this->title = title;
	this->width = width;
	this->height = height;
	InitSDL();
}


Window::~Window()
{
}

void Window::InitSDL() 
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_ERROR("SDL failed to initialize: '%s'", SDL_GetError());
		return;
	}

	// Setup Window
	m_window = SDL_CreateWindow(
		title.c_str(),				// Title
		SDL_WINDOWPOS_UNDEFINED,	// Start  X
		SDL_WINDOWPOS_UNDEFINED,	// Start Y
		width,
		height,
		SDL_WINDOW_SHOWN			// Init flags
	);

	if (m_window == nullptr)
	{
		LOG_ERROR("Failed to initialize window: '%s'", SDL_GetError());
		return;
	}

	// Fetch surface
	m_surface = SDL_GetWindowSurface(m_window);
	
	LOG("Window '%s' initialized", title.c_str());
}

void Window::MainLoop(void(*callback)(float deltaTime))
{
	typedef std::chrono::high_resolution_clock Clock;
	const int sleepTime = (1000.0f / (float)tickRate);

	Clock::time_point lastTime = Clock::now();
	float deltaTime = 0;
	
	bIsRunning = true;
	SDL_Event currentEvent;


	while (true)
	{
		// Update events
		if (SDL_PollEvent(&currentEvent))
			ProcessEvents(currentEvent);

		// Calc delta time
		{
			int timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - lastTime).count();
			deltaTime = (float)timeDiff / 1000.0f;
		}
		callback(deltaTime);

		if (!bIsRunning)
			break;
		

		// Update surface then sleep
		SDL_UpdateWindowSurface(m_window);
		lastTime = Clock::now();


		// Sleep for remainder of tick
		int timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - lastTime).count() / 1000.0f;

		if(timeDiff < sleepTime)
			SDL_Delay(sleepTime - timeDiff);

	}


	LOG("Closing window '%s'", title.c_str());
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	LOG("SDL shutdown");
}

void Window::ProcessEvents(SDL_Event& currentEvent) 
{
	if (currentEvent.type == SDL_QUIT)
	{
		bIsRunning = false;
		LOG("Recieved quit event");
	}

}