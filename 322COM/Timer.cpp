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

uint32 Timer::GetTimeMilliseconds()
{
	if (bIsRunning)
		return SDL_GetTicks() - startTime;
	else
		return 0;
}