#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include <functional>
#include <thread>

#include "ResultOr.hpp"
#include "JobQueue.hpp"

#include "WorkerThread.h"

namespace MRAKOTAZ::core::jobs
{
	/// <summary>
	/// Simple job manager for running function over some data of type T in thread pool
	/// </summary>
	template <typename T> class JobManager
	{
	public:
		JobManager(std::function<void(T&)> jobFn, const uint32_t workersCount = std::thread::hardware_concurrency()) : 
			_queue(std::make_unique<JobQueue<T>>()), _jobFn(jobFn), _shutdown(false), _workersCount(workersCount)
		{
			void* rawMemory = operator new[](_workersCount * sizeof(WorkerThread));
			_workers = static_cast<WorkerThread*>(rawMemory);

			for (size_t i = 0; i < _workersCount; ++i)
			{
				std::unique_ptr<std::thread> workerThread = std::make_unique<std::thread>(&JobManager::worker_thread_fn, this, i);
				new (&_workers[i]) WorkerThread(workerThread);
			}
		}

		~JobManager()
		{
			_shutdown = true;
			delete[] _workers;
		}

		void Enqueue(const T& data)
		{
			_queue->Push(data);

			// we have a data - let's wake threads
			//
			for (uint32_t i = 0; i < _workersCount; ++i)
			{
				WorkerThread& worker = _workers[i];
				worker.Wake();
			}
		}

		void WaitForAll()
		{

		}
	private:
		void worker_thread_fn(int threadIdx)
		{
			WorkerThread& worker = _workers[threadIdx];

			while (!_shutdown)
			{
				while (!_queue->Empty())
				{
					const core::ResultOr<T> data = _queue->Steal();
					if (data.Ok())
					{
						T param = std::move(data.Value());
						_jobFn(param);
					}
				}

				// wait untill someone pushes data to the queue
				worker.Wait();
			}
		}
	private:
		WorkerThread* _workers;
		std::unique_ptr<JobQueue<T>> _queue;

		std::function<void(T&)> _jobFn;

		bool _shutdown;
		uint32_t _workersCount;
	};
}

#endif

