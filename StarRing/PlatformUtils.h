#pragma once
#include "Core.h"
#include <string>
namespace StarRing
{
	class SR_API FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}
