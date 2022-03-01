#pragma once
#include "Core.h"
#include <spdlog/spdlog.h>

using namespace std;
namespace StarRing{
	class SR_API Log
	{
	public:
		Log();
		~Log();

		static void Init();
		inline static shared_ptr<spdlog::logger>& GetCoreLogger() { return ms_CoreLogger; }
		inline static shared_ptr<spdlog::logger>& GetClientLogger() { return ms_ClientLogger; }
	private:
		static shared_ptr<spdlog::logger> ms_CoreLogger;
		static shared_ptr<spdlog::logger> ms_ClientLogger;
	};
}


#define SR_CORE_ERROR(...)		::StarRing::Log::GetCoreLogger()->error(__VA_ARGS__);
#define SR_CORE_WARN(...)		::StarRing::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define SR_CORE_INFO(...)		::StarRing::Log::GetCoreLogger()->info(__VA_ARGS__);
#define SR_CORE_TRACE(...)		::StarRing::Log::GetCoreLogger()->trace(__VA_ARGS__);

#define SR_CLIENT_ERROR(...)	::StarRing::Log::GetClientLogger()->error(__VA_ARGS__);
#define SR_CLIENT_WARN(...)		::StarRing::Log::GetClientLogger()->warn(__VA_ARGS__);
#define SR_CLIENT_INFO(...)		::StarRing::Log::GetClientLogger()->info(__VA_ARGS__);
#define SR_CLIENT_TRACE(...)	::StarRing::Log::GetClientLogger()->trace(__VA_ARGS__);