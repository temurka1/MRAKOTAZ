#include "pch.h"
#include "AutoResetEvent.h"

using AutoResetEvent = MRAKOTAZ::core::jobs::AutoResetEvent;

AutoResetEvent::AutoResetEvent() : _signal(false)
{
}

void AutoResetEvent::Notify()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_signal = true;
	_condition.notify_all();
}

void AutoResetEvent::Wait()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_condition.wait(lock, [&] { return _signal; });
	_signal = false;
}