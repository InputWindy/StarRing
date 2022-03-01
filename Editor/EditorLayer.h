#pragma once
#include <StarRing.h>
#include "SceneHierarchyPanel.h"
#include "ContentBrowserPanel.h"
#include "ConsolePanel.h"
namespace StarRing {

	//EditorLayer���ڱ༭����
	//��EditorLayer�ڲ�����Entity�����ø��ӹ�ϵ�����ö�����
	//���մ洢ΪScene�ļ�
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		//��ʼ������༭��
		virtual void OnAwake() override;

		//ÿ֡���±༭����
		// ����EditorCamera
		// ��Ⱦ����
		// ���±༭��UI
		virtual void OnUpdate() override;

		//��Ⱦ�༭��UI
		virtual void OnImGuiRender() override;
	private:
		EditorCamera m_EditorCamera;

		//������(��ЩӦ����Mesh��Materialά��)
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
