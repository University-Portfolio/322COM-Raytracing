#pragma once

typedef unsigned int uint32;


/**
* Measures time passed in milliseconds
*/
class Timer
{
private:
	uint32 startTime;
	bool bIsRunning = false;

public:
	void Start();
	void Stop();

	uint32 GetTimeMilliseconds() const;
	inline bool IsRunning() const { return bIsRunning; }
};

