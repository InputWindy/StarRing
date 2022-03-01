#pragma once
#include "Core.h"
namespace StarRing {
	class SR_API GraphicContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	private:

	};
}