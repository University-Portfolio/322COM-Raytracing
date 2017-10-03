#include "Window.h"

#include "Timer.h"
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


static bool bIsAPIRunning = false;

void Window::InitAPI()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_ERROR("SDL failed to initialize: '%s'", SDL_GetError());
		return;
	}
	bIsAPIRunning = true;
}

void Window::DestroyAPI()
{
	SDL_Quit();
	bIsAPIRunning = false;
	LOG("SDL shutdown");
}


void Window::InitSDL() 
{
	if (!bIsAPIRunning)
	{
		LOG_ERROR("Failed to initialize window: 'Required Window API is not running'");
		return;
	}

	// Setup Window
	m_window = SDL_CreateWindow(
		title.c_str(),				// Title
		SDL_WINDOWPOS_UNDEFINED,	// Start  X
		SDL_WINDOWPOS_UNDEFINED,	// Start Y
		width,
		height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE		// Init flags
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

void Window::MainLoop(void(*callback)(Window* context, float deltaTime))
{
	Timer mainTimer;
	const int desiredSleep = (1000.0f / (float)tickRate);

	Timer deltaTimer;
	float deltaTime = 0;
	
	bIsRunning = true;
	SDL_Event currentEvent;


	while (bIsAPIRunning)
	{
		mainTimer.Start();

		// Update events
		while (SDL_PollEvent(&currentEvent))
			ProcessEvent(currentEvent);


		// Run update using delta time
		deltaTime = (float)deltaTimer.GetTimeMilliseconds() / 1000.0f;
		callback(this, deltaTime);
		deltaTimer.Start();


		if (!bIsRunning)
			break;
		

		// Update surface then sleep
		SDL_UpdateWindowSurface(m_window);

		const int sleepTime = desiredSleep - mainTimer.GetTimeMilliseconds();

		if(sleepTime > 0)
			SDL_Delay(sleepTime);

	}


	LOG("Closing window '%s'", title.c_str());
	SDL_DestroyWindow(m_window);
}

void Window::ProcessEvent(SDL_Event& currentEvent) 
{
	if (currentEvent.type == SDL_QUIT)
	{
		bIsRunning = false;
		LOG("Recieved quit event");
	}


	if (currentEvent.type == SDL_WINDOWEVENT)
	{
		switch (currentEvent.window.event)
		{

		// Window changed size
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		case SDL_WINDOWEVENT_RESIZED:
		{
			width = currentEvent.window.data1;
			height = currentEvent.window.data2;
			break;
		}

		default:
			break;
		}
	}
}