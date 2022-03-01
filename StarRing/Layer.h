#pragma once
#include "Core.h"
#include "Event.h"
#include "DeltaTime.h"
#include "Timer.h"

////添加Log信息（目前只能显示代码段执行时间）
//#define ADD_PROFILE(name)\
//Timer time##__LINE__(name, [&](Profile profile) {\
//m_Profiles.push_back(profile);\
//});
//
//#define LOG_CODEBLOCK_DURATION  for (Profile profile : m_Profiles) {SR_CORE_TRACE("{0},{1}", profile.name, profile.time);}

#pragma region ImGUI_Docking
#define ImGui_Docking_Begin(name)  \
static bool opt_fullscreen = true;\
static bool opt_padding = false;\
static bool p_open = true;\
static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;\
\
ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;\
if (opt_fullscreen)\
{\
	const ImGuiViewport* viewport = ImGui::GetMainViewport();\
	ImGui::SetNextWindowPos(viewport->WorkPos);\
	ImGui::SetNextWindowSize(viewport->WorkSize);\
	ImGui::SetNextWindowViewport(viewport->ID);\
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);\
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);\
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;\
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;\
}\
else\
{\
	dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;\
}\
if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)\
window_flags |= ImGuiWindowFlags_NoBackground;\
if (!opt_padding)\
ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));\
ImGui::Begin(name, &p_open, window_flags);\
if (!opt_padding)\
ImGui::PopStyleVar();\
\
if (opt_fullscreen)\
ImGui::PopStyleVar(2);\
\
ImGuiIO& io = ImGui::GetIO();\
if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)\
{\
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");\
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);\
}

#define ImGui_Docking_End ImGui::End();
#pragma endregion


namespace StarRing {
	class SR_API Layer
	{
	public:
		Layer(const string& name = "Layer");
		virtual ~Layer();

		virtual void OnStart() {};			//相当于Unity的Start()
		virtual void OnAwake() {};			//相当于Unity的Awake()

		virtual void OnDestroy() {};		//销毁时调用
		
		virtual void OnUpdate() {};			//相当于Unity的Update()
		virtual void OnFixedUpdate() {};	//相当于Unity的FixedUpdate()
		virtual void OnLateUpdate() {};		//相当于Unity的LateUpdate()
		
		virtual void OnImGuiRender() {};	//GUI系统
		
		virtual void OnEvent(Event& e) {};	//事件系统（需要被调用）

		inline string GetName()const { return m_DebugName; }
	protected:
		string m_DebugName;

		//vector<Profile> m_Profiles;
	};
}

