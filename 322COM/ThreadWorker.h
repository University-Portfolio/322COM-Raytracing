#pragma once
#include <thread>
#include <functional>


/**
* Interface for something which can queue and execute work amongst multiple threads
*/
class IWorkable 
{
public:
	/**
	* Callback for when a work request comes in
	* @param workerId		The id of the worker this is being executed by
	* @param data			The data to process
	*/
	virtual void ExecuteWork(int workerId, void* data) = 0;
};


/**
* Worker who will queue up any work and execute is on it's thread
*/
class ThreadWorker
{
private:
	IWorkable* m_workable;
	int id;

	std::thread* m_thread = nullptr;
	bool bThreadActive;

	void* activeWork;


	void ThreadFunction();


public:
	ThreadWorker(IWorkable* workable, int workerId);
	~ThreadWorker(); 
	
	/**
	* Queue worker callback work with this param as work
	* @param param			Passed to the worker callback
	*/
	inline void QueueWork(void* param) { activeWork = param; }
	inline bool IsWorkerActive() const { return activeWork != nullptr; }
};

