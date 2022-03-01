#pragma once
#include "Core.h"
#include <chrono>
#include <string>
#include "Log.h"
namespace StarRing {
	
	class Timer
	{
	public:
		
		Timer(const string name) ;
		~Timer();

		void Stop();
	private:
		const string m_Name;
		bool m_IsStopped;

		std::chrono::time_point<std::chrono::steady_clock> m_StarTimePoint;

	};
}