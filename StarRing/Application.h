#pragma once
#include "Core.h"

#include "WindowsWindow.h"
#include "LayerStack.h"

#include "Event.h"
#include "ApplicationEvent.h"
#include "ImguiLayer.h"
#include "VertexArray.h"
#include "OrthographicCamera.h"

#include "DeltaTime.h"

namespace StarRing {
	class SR_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void Close();
		//Ӧ�ó������ڴ����¼��Ľӿڣ�����һ���¼�����
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Scope<ImGuiLayer>& GetImGuiLayer() { return m_ImGuiLayer; }

		static inline Application& GetApplication() { return *ms_ApplicationInstance; }
		inline Window& GetWindow() { return *m_Window; }

		void OnStart();
	private:
		//AppEvent
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
		bool OnAppTickEvent(AppTickEvent& e);
		bool OnAppUpdateEvent(AppUpdateEvent& e);
		bool OnAppRenderEvent(AppRenderEvent& e);

		Scope<Window> m_Window;
		Scope<ImGuiLayer> m_ImGuiLayer;
		bool m_IsRunning = true, m_Minimized = false;

		LayerStack m_LayerStack;

	private:
		static Application* ms_ApplicationInstance;
	};
	//����ӿ����ɿͻ���ʵ�ֵ�
	Application* CreateApplication();
}




