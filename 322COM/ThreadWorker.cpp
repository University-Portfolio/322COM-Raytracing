#include "ThreadWorker.h"
#include <SDL.h>


ThreadWorker::ThreadWorker(IWorkable* workable, int workerId)
{
	m_workable = workable;
	id = workerId;

	m_thread = new std::thread(&ThreadWorker::ThreadFunction, this);
}


ThreadWorker::~ThreadWorker()
{
	bThreadActive = false;
	activeWork = nullptr;

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
			m_workable->ExecuteWork(id, activeWork);
			activeWork = nullptr;
		}
		else
			SDL_Delay(1);
	}
}
