#pragma once
#include <memory>
#include <sstream>
#ifdef SR_PLATFORM_WINDOWS
	#ifdef SR_BUILD_DLL
		#define SR_API __declspec(dllexport)
	#else
		#define SR_API __declspec(dllimport)
	#endif // SR_BUILD_DLL
#else
	#error SREngine only surports windows!
#endif // SR_PLATFORM_WINDOWS

#define BIT(num) (1<<num)

#define BIND_EVENT_FUNCTION(func) std::bind(&func,this,placeholders::_1)

#define DECLARE_REF(classname) class classname; using classname##Ref = StarRing::Ref<classname>;
#define DECLARE_SCOPE(classname) class classname; using classname##Scope = StarRing::Scope<classname>;

namespace StarRing {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}