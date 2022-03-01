#pragma once
#include "Window.h"
#include "OpenGLContext.h"
struct GLFWwindow;
namespace StarRing {
	class SR_API WindowsWindow:public Window
	{
	
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate()override;

		inline unsigned int GetWidth()const override { return m_Data.Width; };
		inline unsigned int GetHeight()const override { return m_Data.Height; };

		//´°¿ÚÐÅÏ¢
		virtual void SetEventCallBackFunc(const EventCallBackFunc& event_callback_func) {
			m_Data.EventCallBackFunc = event_callback_func;
		};
		virtual void SetVSync(bool enabled);
		virtual bool IsVSync()const ;

		virtual void* GetNativeWindow()const override{ return m_Window; };
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();
	private:
		GLFWwindow* m_Window;
		GraphicContext* m_Context;

		struct WindowData
		{
			string Tittle;
			unsigned int Width, Height;
			bool VSync;

			EventCallBackFunc EventCallBackFunc;
		};

		WindowData m_Data;

	};
}

