#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "WindowsInput.h"
#include "Renderer.h"

namespace StarRing {

	Application* Application::ms_ApplicationInstance = NULL;

	Application::Application()
	{
		ms_ApplicationInstance = this;
		m_Window =Scope<Window>(Window::Create()) ;
		m_Window->SetEventCallBackFunc(BIND_EVENT_FUNCTION(Application::OnEvent));
	
		m_ImGuiLayer = make_unique<ImGuiLayer>();

		Renderer::Init();

	}
	Application::~Application()
	{

	}
	void Application::Run()
	{ 
		//engine loop here!
		while (m_IsRunning) {

			DeltaTime::UpdateTime();

				if (!m_Minimized) {
					#pragma region 所有layer更新
					for (Layer* layer : m_LayerStack) {
						layer->OnUpdate();
					}
					for (Layer* layer : m_LayerStack) {
						layer->OnFixedUpdate();
					}
					for (Layer* layer : m_LayerStack) {
						layer->OnLateUpdate();
					}
					#pragma endregion
				}
				#pragma region GUI渲染
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack) {
					layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
				#pragma endregion

			m_Window->OnUpdate();//glfw窗口更新
		}
	}

	void Application::Close()
	{
		m_IsRunning = false;
	}

	//对glfw窗口的输入操作都会同步影响所有的层
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(Application::OnWindowResizeEvent));
		
		//SR_CORE_INFO("{0}", e.ToString());

		for (auto iter = m_LayerStack.end(); iter != m_LayerStack.begin();) {
			(*--iter)->OnEvent(e);
			if (e.IsHandled()) {
				break;
			}
		}
	}
	void Application::PushLayer(Layer* layer)
	{
		if (layer) {
			m_LayerStack.PushLayer(layer);
			//layer->OnStart();
		}
	}
	void Application::PushOverlay(Layer* overlay)
	{
		if (overlay) {
			m_LayerStack.PushOverlay(overlay);
			//overlay->OnStart();
		}
	}
	void Application::OnStart()
	{
		for (Layer* layer : m_LayerStack) {
			layer->OnStart();
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}
	bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		//SR_CORE_WARN("窗口宽度：{0}  窗口高度：{1}",e.GetWidth(),e.GetHeight())
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return true;
		}
		Renderer::OnWindowResize(e.GetWidth(),e.GetHeight());
		m_Minimized = false;
		return true;
	}
	bool Application::OnAppTickEvent(AppTickEvent& e)
	{
		return false;
	}
	bool Application::OnAppUpdateEvent(AppUpdateEvent& e)
	{
		return false;
	}
	bool Application::OnAppRenderEvent(AppRenderEvent& e)
	{
		return false;
	}
}
