#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "AutoResetEvent.h"

namespace MRAKOTAZ::core::jobs
{
	class WorkerThread
	{
	public:
		WorkerThread(std::unique_ptr<std::thread>& thread);
		~WorkerThread();

		/// <summary>
		/// Wakes this thread
		/// </summary>
		void Wake();

		/// <summary>
		/// Blocks thread
		/// </summary>
		void Wait();
	private:
		std::unique_ptr<std::thread> _thread;

		AutoResetEvent _wakeSem;
		AutoResetEvent _doneSem;
	};
}

#endif
