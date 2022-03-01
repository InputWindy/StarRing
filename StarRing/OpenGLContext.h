#pragma once
#include "GraphicsContext.h"
struct GLFWwindow;
namespace StarRing {
	class SR_API OpenGLContext:public GraphicContext
	{
	public:
		OpenGLContext(GLFWwindow* window_handle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}

