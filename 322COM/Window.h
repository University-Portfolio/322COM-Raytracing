#pragma once
#include <SDL.h>
#include <string>

/**
* Container for SDL window
*/
class Window
{
private:
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_surface = nullptr;

	std::string title;
	int width;
	int height;
	const int tickRate = 30;

	bool bIsRunning;

	
public:
	Window(std::string title, int width, int height);
	~Window();

	/**
	* Launches window into endless loop until closed by the user
	* @param callback	Callback function called each loop
	*/
	void MainLoop(void(*callback)(float deltaTime));

private:
	void InitSDL();

	void ProcessEvents(SDL_Event& currentEvent);
};

