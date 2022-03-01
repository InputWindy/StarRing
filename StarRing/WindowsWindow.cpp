#include "pch.h"
#include "WindowsWindow.h"
#include "Log.h"
#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace StarRing {
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallBack(int error,const char* description) {
		SR_CORE_ERROR("GLFW error:{0} {1}",error,description);
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

}

StarRing::WindowsWindow::WindowsWindow(const WindowProps& props)
{
	Init(props);
}

StarRing::WindowsWindow::~WindowsWindow()
{
	ShutDown();
}

void StarRing::WindowsWindow::OnUpdate()
{
	glfwPollEvents();
	m_Context->SwapBuffers();
}

void StarRing::WindowsWindow::SetVSync(bool enabled)
{
	if (enabled) {
		glfwSwapInterval(1);
	}
	else {
		glfwSwapInterval(0);
	}
	m_Data.VSync = enabled;
}

bool StarRing::WindowsWindow::IsVSync() const
{
	return m_Data.VSync;
}

void StarRing::WindowsWindow::Init(const WindowProps& props)
{
	m_Data.Tittle = props.Tittle;
	m_Data.Height = props.Height;
	m_Data.Width = props.Width;

	SR_CORE_INFO("Creating Window {0} ({1}, {2})",props.Tittle,props.Height,props.Width);
	
	if (!s_GLFWInitialized) {
		int success = glfwInit();
		glfwSetErrorCallback(GLFWErrorCallBack);
		s_GLFWInitialized = true;
	}
	
	m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Tittle.c_str(), NULL, NULL);;
	
	m_Context = new OpenGLContext(m_Window);
	m_Context->Init();

	glfwSetWindowUserPointer(m_Window,&m_Data);
	SetVSync(false);

	//设置glfw回调函数
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);
		window_data.Width = width;
		window_data.Height = height;
		
		WindowResizeEvent event(width,height);
		window_data.EventCallBackFunc(event);

	});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

		WindowCloseEvent event ;
		window_data.EventCallBackFunc(event); 

	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window,int key,int scancode,int action,int mods) {
		WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event(key,0);
			window_data.EventCallBackFunc(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event(key);
			window_data.EventCallBackFunc(event);
			break;
		} 
		case GLFW_REPEAT:
		{
			KeyPressedEvent event(key, 1);
			window_data.EventCallBackFunc(event);
			break;
		}	
		default:
			break;
		}

	});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window,unsigned int key_code) {
		WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

		KeyTypedEvent event(key_code);
		window_data.EventCallBackFunc(event);

	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int btn,int action, int mods) {
		WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(btn);
			window_data.EventCallBackFunc(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(btn);
			window_data.EventCallBackFunc(event);
			break;
		}
		default:
			break;
		}
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x, double y) {
		WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScrolledEvent event((float)x, (float)y);
		window_data.EventCallBackFunc(event);
	});

	glfwSetCursorPosCallback(m_Window,[](GLFWwindow* window,double x,double y) {
		WindowData& window_data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseMovedEvent event((float)x, (float)y);
		window_data.EventCallBackFunc(event);
	});

}

void StarRing::WindowsWindow::ShutDown()
{
	glfwDestroyWindow(m_Window);
}
