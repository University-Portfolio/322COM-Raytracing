#include "ThreadWorker.h"
#include <SDL.h>


ThreadWorker::ThreadWorker(WorkerCallback callback) : m_callback(callback)
{
	m_thread = new std::thread(&ThreadWorker::ThreadFunction, this);
	m_thread->detach();
}


ThreadWorker::~ThreadWorker()
{
	bThreadActive = false;
	if(m_thread->joinable())
		m_thread->join();
	delete m_thread;
}

void ThreadWorker::ThreadFunction()
{
	bThreadActive = true;

	while (bThreadActive)
	{
		if (activeWork != nullptr)
		{
			if(m_callback)
				m_callback(activeWork);
			activeWork = nullptr;
		}
		else
			SDL_Delay(1);
	}
}
