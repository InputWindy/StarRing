#pragma once
#include "ImguiLayer.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"
namespace StarRing {

	class SceneHierarchyPanel
	{

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const SceneRef& scene);

		void SetScene(const SceneRef& scene);

		void OnUpdate();
		void OnImGuiRender();

		Entity* GetSelectedEntity()const;


	private:
		void DrawEntityNode(Entity* entity);
		void DrawComponents(Entity* entity);
	private:
		SceneRef m_Scene;

		struct SceneHierarchyContext
		{
			Entity* m_SelectedEntity = nullptr;
			
			bool m_bDeleteSelectedEntity = false;
			bool m_bChangeSelectedEntityName = false;

			bool m_bDeleteCameraComponent = false;
			bool m_bDeleteLightComponent = false;
			bool m_bDeleteMeshRendererComponent = false;
			bool m_bDeleteSkeletonComponent = false;
			bool m_bDeleteAnimatorComponent = false;

			string m_AddComponentType;
			bool m_bAddComponent = false;


			char buf[256];
		};

		SceneHierarchyContext m_SceneHierarchyCtx;

	};

}
