#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <condition_variable>

namespace MRAKOTAZ::core::jobs
{
	class AutoResetEvent
	{
	public:
		AutoResetEvent();

		AutoResetEvent(const AutoResetEvent&) = delete;
		AutoResetEvent& operator = (const AutoResetEvent&) = delete;

		void Notify();
		void Wait();
	private:
		std::mutex _mutex;
		std::condition_variable _condition;
		bool _signal;
	};
}

#endif

