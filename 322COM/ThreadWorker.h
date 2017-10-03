#pragma once
#include <thread>
#include <functional>


typedef std::function<void(void*)> WorkerCallback;


class ThreadWorker
{
private:
	WorkerCallback m_callback;

	std::thread* m_thread = nullptr;
	bool bThreadActive;

	void* activeWork;


	void ThreadFunction();


public:
	ThreadWorker(WorkerCallback callback);
	~ThreadWorker(); 
	
	/**
	* Queue worker callback work with this param as work
	* @param param			Passed to the worker callback
	*/
	inline void QueueWork(void* param) { activeWork = param; }
	inline bool IsWorkerActive() const { return activeWork != nullptr; }
};

