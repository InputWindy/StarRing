#pragma once
#include "Core.h"
#include <GLFW/glfw3.h>
namespace StarRing {
	class SR_API DeltaTime
	{
		friend class Application;
	public:
		static float GetSeconds()		{ return m_Time ; }
		static float GetMilliSeconds()	{ return m_Time * 1000.0f; }
		static float GetFrameRate()		{ return 1.0f / m_Time; }
	private:
		static void UpdateTime() {
			float time = (float)glfwGetTime();
			m_Time = time - m_LastFrameTime;
			m_LastFrameTime = time;
		};
	private:
		static inline float m_Time = 0.0f;
		static inline float m_LastFrameTime = 0.0f;
	};
	
}

