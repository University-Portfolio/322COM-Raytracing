#include "Timer.h"
#include <SDL.h>


void Timer::Start() 
{
	startTime = SDL_GetTicks();
	bIsRunning = true;
}

void Timer::Stop() 
{
	bIsRunning = false;
}

uint32_t Timer::GetTimeMilliseconds() const
{
	if (bIsRunning)
		return SDL_GetTicks() - startTime;
	else
		return 0;
}