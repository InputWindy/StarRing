#pragma once
#include <StarRing.h>
#include "SceneHierarchyPanel.h"
#include "ContentBrowserPanel.h"
#include "ConsolePanel.h"
namespace StarRing {

	//EditorLayer用于编辑场景
	//在EditorLayer内部增减Entity，设置父子关系，设置动画等
	//最终存储为Scene文件
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		//初始化引擎编辑器
		virtual void OnAwake() override;

		//每帧更新编辑器：
		// 更新EditorCamera
		// 渲染画面
		// 更新编辑器UI
		virtual void OnUpdate() override;

		//渲染编辑器UI
		virtual void OnImGuiRender() override;
	private:
		EditorCamera m_EditorCamera;

		//测试用(这些应该由Mesh和Material维护)
		ShaderRef m_Shader;
		VertexArrayRef m_VertexArray;
		//==============================

		SceneRef m_ActiveScene;

		bool m_bEnableMainCamera = true;
		bool m_bViewportFocused = true;
		bool m_bViewportHovered = true;

		SceneHierarchyPanel m_SceneHierarchy;
		ContentBrowserPanel m_ContentBrowser;
		ConsolePanel		m_Console;
	
		Texture2DRef m_CheckerboardTexture, m_IconPlay, m_IconStop;

		vec2 m_viewPortPanelSize = {1280.0f,720.0f};
	};
}
