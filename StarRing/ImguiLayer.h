#pragma once
#include "Layer.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "ApplicationEvent.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_internal.h>
#include "DeltaTime.h"

//永久使用的头文件
#include <GLFW/glfw3.h>

namespace StarRing {

	class SR_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAwake() override;
		virtual void OnDestroy() override;
		virtual void OnEvent(Event& e)override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockImGuiEvents = block; }
	private:
		bool m_BlockImGuiEvents = true ;
		float m_Time = 0.0f;
	};
}

