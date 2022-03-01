#include "pch.h"
#include "Timer.h"
namespace StarRing {
	Timer::Timer(const string name) :m_Name(name), m_IsStopped(false)
	{
		m_StarTimePoint = std::chrono::high_resolution_clock::now();
	}
	Timer::~Timer()
	{
		if (!m_IsStopped) {
			Stop();
		}
	}
	void Timer::Stop()
	{
		auto endtimepoint = std::chrono::high_resolution_clock::now();

		long long  start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StarTimePoint).time_since_epoch().count();
		long long  end = std::chrono::time_point_cast<std::chrono::microseconds>(endtimepoint).time_since_epoch().count();

		m_IsStopped = true;

		float duration = (end - start) * 0.001f;

		SR_CORE_TRACE("TimerÃû³Æ:{0}  Duration:{1}ms", m_Name, duration);
	}
}