#include "pch.h"
#include "OpenGLContext.h"
#include "Log.h"
namespace StarRing {
    OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
    {
        assert(window_handle);
        m_WindowHandle = window_handle;
    }
    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (!status) {
            SR_CORE_ERROR("{0} {1}", status, "Fail to initialize glad!");
        }

        SR_CORE_INFO("===============OpenGL Info===============");
        SR_CORE_INFO("OpenGL Vendor:  {0}", glGetString(GL_VENDOR));
        SR_CORE_INFO("OpenGL Renderer:{0}", glGetString(GL_RENDERER));
        SR_CORE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));
        SR_CORE_INFO("===============OpenGL Info===============");
    }
    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}