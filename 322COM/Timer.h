#pragma once
#include <SDL_config.h>


/**
* Measures time passed in milliseconds
*/
class Timer
{
private:
	uint32_t startTime;
	bool bIsRunning = false;

public:
	void Start();
	void Stop();

	uint32_t GetTimeMilliseconds() const;
	inline bool IsRunning() const { return bIsRunning; }
};

