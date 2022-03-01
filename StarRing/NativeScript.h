#pragma once
#include "Core.h"
namespace StarRing {
	class SR_API NativeScript
	{
	public:
		NativeScript() = default;
		~NativeScript() = default;

		void OnCreate();
		void OnUpdate();

	};
}

