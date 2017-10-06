#pragma once
#include <SDL.h>
#include <string>
#include "Keyboard.h"
#include "RenderSurface.h"

/**
* Container for SDL window
*/
class Window
{
private:
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_surface = nullptr;
	RenderSurface* m_renderSurface = nullptr;

	Keyboard m_keyboard;

	std::string title;
	int width;
	int height;
	const int tickRate;

	bool bIsRunning;

	
public:
	Window(std::string title, int width, int height, const int tickRate = 30);
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

	inline std::string GetTitle() const { return title; }
	inline RenderSurface* GetRenderSurface() const { return m_renderSurface; }

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	inline Keyboard* GetKeyboard() { return &m_keyboard; }
};

