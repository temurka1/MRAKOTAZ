#include "pch.h"
#include "WorkerThread.h"

using WorkerThread = MRAKOTAZ::core::jobs::WorkerThread;

WorkerThread::WorkerThread(std::unique_ptr<std::thread>& thread) : _thread(std::move(thread))
{
}

WorkerThread::~WorkerThread()
{
	Wake();
	_thread->join();
}

void WorkerThread::Wake()
{
	_wakeSem.Notify();
}

void WorkerThread::Wait()
{
	_doneSem.Notify();
	_wakeSem.Wait();
}
