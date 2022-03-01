#include "pch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
namespace StarRing {

	shared_ptr<spdlog::logger> Log::ms_CoreLogger;
	shared_ptr<spdlog::logger> Log::ms_ClientLogger;

	Log::Log()
	{
	}

	Log::~Log()
	{
	}

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n:%v%$");
		ms_CoreLogger = spdlog::stdout_color_mt("StarRing");
		ms_CoreLogger->set_level(spdlog::level::trace);

		ms_ClientLogger = spdlog::stdout_color_mt("App");
		ms_ClientLogger->set_level(spdlog::level::trace);
	}

}
