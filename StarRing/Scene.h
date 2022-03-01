#pragma once
#include "Entity.h"
#include "EditorCamera.h"
#include <map>
using namespace std;
namespace StarRing {
	DECLARE_REF(Scene)
	DECLARE_SCOPE(Scene)
	class SR_API Scene
	{
		friend class Entity;
		friend class EditorLayer;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

	public:
		Scene() = default;
		Scene(const string& name, uint32_t width, uint32_t height);
		~Scene() = default;

		//����ֻ�г���������Entity�Ľӿ�
		EntityRef CreateEntity(const std::string& name);
		void AddPrefab(const std::string& name, const std::string& filepath);
		void AddEntity(EntityRef& entity);
		void AddSunLight();
		void DestroyEntity(const string& name);

		vector<EntityRef>& GetEntities();
		EntityRef GetEntityByName(const string& name);

		//Editorֻ��Ҫ��Ⱦ���棬����Ҫִ���κνű��͸��²���
		//���������е�����Entity��ִ�����е�MeshRendererComponent��Renderer�ύ����
		void RenderScene(EditorCamera& editorCamera);

		//���³���(ʵ����Ϸ����Ҫִ�нű�,�������)
		//���������е�����Entity��ִ�����е�MeshRendererComponent��Renderer�ύ����
		void OnUpdateRuntime();

		//����һ���³���
		static SceneRef Create(string name, uint32_t width, uint32_t height);

		void LogScene();
	private:
		template<typename T>
		void AddComponent(Entity* entt, T& component);

		//���������ɾ��ʵ�������й��صĽű�
		template<typename T>
		void DeleteComponent(Entity* entt);

		//�������ר������ɾ��ʵ����ĳ���ű�
		void DeleteNativeScriptComponentByName(EntityRef entt,string& name);
		vector<EntityRef> m_Entities;
	private:
		string m_SceneName;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	};

	template<typename T>
	inline void Scene::AddComponent(Entity* entt, T& component)
	{
		assert(false);
	}
	template<>
	inline void Scene::AddComponent<CameraComponent>(Entity* entt, CameraComponent& component)
	{
		entt->AddCameraComponent(component);
	}
	template<>
	inline void Scene::AddComponent<LightComponent>(Entity* entt, LightComponent& component)
	{
		entt->AddLightComponent(component);
	}
	template<>
	inline void Scene::AddComponent<MeshRendererComponent>(Entity* entt, MeshRendererComponent& component)
	{
		entt->AddMeshRendererComponent(component);
	}
	template<>
	inline void Scene::AddComponent<SkeletonComponent>(Entity* entt, SkeletonComponent& component)
	{
		entt->AddSkeletonComponent(component);
	}

	template<typename T>
	inline void Scene::DeleteComponent(Entity* entt)
	{
		if (T::ms_ComponentType == ComponentType::CameraComponent)
			entt->RemoveCameraComponent();
		if (T::ms_ComponentType == ComponentType::LightComponent)
			entt->RemoveLightComponent();
		if (T::ms_ComponentType == ComponentType::MeshFilterComponent)
			entt->RemoveMeshComponent();
		if (T::ms_ComponentType == ComponentType::MeshRendererComponent)
			entt->RemoveMeshRendererComponent();
		if (T::ms_ComponentType == ComponentType::SkeletonComponent)
			entt->RemoveSkeletonComponent();
		if (T::ms_ComponentType == ComponentType::TagComponent)
			entt->RemoveTagComponent();
		if (T::ms_ComponentType == ComponentType::TransformComponent)
			entt->RemoveTransformComponent();
		if (T::ms_ComponentType == ComponentType::NativeScriptComponent)
			entt->RemoveNativeScriptComponents();
	}

}
