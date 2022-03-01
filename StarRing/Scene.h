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

		//有且只有场景有生成Entity的接口
		EntityRef CreateEntity(const std::string& name);
		void AddPrefab(const std::string& name, const std::string& filepath);
		void AddEntity(EntityRef& entity);
		void AddSunLight();
		void DestroyEntity(const string& name);

		vector<EntityRef>& GetEntities();
		EntityRef GetEntityByName(const string& name);

		//Editor只需要渲染画面，不需要执行任何脚本和更新操作
		//遍历场景中的所有Entity，执行其中的MeshRendererComponent向Renderer提交数据
		void RenderScene(EditorCamera& editorCamera);

		//更新场景(实际游戏中需要执行脚本,更新相机)
		//遍历场景中的所有Entity，执行其中的MeshRendererComponent向Renderer提交数据
		void OnUpdateRuntime();

		//生成一个新场景
		static SceneRef Create(string name, uint32_t width, uint32_t height);

		void LogScene();
	private:
		template<typename T>
		void AddComponent(Entity* entt, T& component);

		//这个方法会删掉实体上所有挂载的脚本
		template<typename T>
		void DeleteComponent(Entity* entt);

		//这个方法专门用来删掉实体上某个脚本
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
