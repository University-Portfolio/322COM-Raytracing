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
	* Initialize everything required by the API (SDL)
	*/
	static void InitAPI();

	/**
	* Destroy eveything used by the API (SDL)
	*/
	static void DestroyAPI();

	/**
	* Launches window into endless loop until closed by the user
	* @param callback	Callback function called each loop
	*/
	void MainLoop(void(*callback)(Window* context, float deltaTime));

private:

	void InitSDL();

	void ProcessEvent(SDL_Event& currentEvent);

	
	/**
	* Getters and Setters
	*/
public:

	inline std::string GetTitle() { return title; }
	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
};

