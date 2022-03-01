#include "pch.h"
#include "Scene.h"
#include "Component.h"
#include "DeltaTime.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Log.h"
namespace StarRing {
	Scene::Scene(const string& name, uint32_t width, uint32_t height):m_SceneName(name),m_ViewportWidth(width),m_ViewportHeight(height)
	{
	}
	//创建的物体默认有Tag和Transform
	EntityRef Scene::CreateEntity(const std::string& name)
	{
		EntityRef ret = Entity::Create(name,this,nullptr);
		ret->AddTagComponent(TagComponent(name));
		ret->AddTransformComponent(TransformComponent(vec3(0.0f,0.0f,0.0f)));
		m_Entities.push_back(ret);
		return ret;
	}

	void Scene::AddPrefab(const std::string& name,const std::string& filepath)
	{
		EntityRef prefab = ResourceManager::GetPrefab(filepath);
		prefab->m_EntityName = name;
		AddEntity(prefab);
	}

	void Scene::AddEntity(EntityRef& entity)
	{
		entity->SetScene(this);
		m_Entities.push_back(entity);
		for (auto child : entity->m_Children) {
			AddEntity(child);
		}
	}

	void Scene::AddSunLight() {
		EntityRef sun = CreateEntity("Sun");
		LightComponent dLight = LightComponent();
		dLight.m_DirectionalLight = DirectionalLight();
		dLight.m_DirectionalLight.dColor = vec3(1.0f, 1.0f, 1.0f);
		dLight.m_DirectionalLight.dLightDir = vec3(-1.0f,-1.0f,-1.0f);
		sun->AddLightComponent(dLight);
	};

	void Scene::DestroyEntity(const string& name)
	{
		for (auto iter = m_Entities.begin(); iter != m_Entities.end();++iter) {
			//在场景中找到该实体
			if ((*iter)->GetName() == name) {
				//找到实体后先删除该实体的所有子实体
				for (auto child : (*iter)->m_Children) {
					DestroyEntity(child->GetName());
				}
				//cout << (*iter)->m_Children.size() << endl;;
				//清空子实体后，删除当前实体(如果有父节点，就先解绑,然后从场景中删除自己)
				if ((*iter)->GetParent()) {
					(*iter)->SetParent(nullptr);
				}
				m_Entities.erase(iter);
				return;
			}
		}
	}

	SceneRef Scene::Create(string name, uint32_t width, uint32_t height)
	{
		return make_shared<Scene>(name,width,height);
	}

	void Scene::LogScene()
	{
		for (auto entt:m_Entities)
		{
			SR_CORE_WARN("实体名称：{0},引用数目：{1}",entt->m_EntityName.c_str(),entt.use_count());
		}
	}

	void Scene::DeleteNativeScriptComponentByName(EntityRef entt, string& name)
	{
	}

	vector<EntityRef>& Scene::GetEntities() 
	{
		return m_Entities;
	}
	EntityRef Scene::GetEntityByName(const string& name) {
		for (auto iter = m_Entities.begin(); iter != m_Entities.end(); ++iter) {
			if ((*iter)->GetName() == name) {
				return *iter;
			}
		}
	};

	void Scene::RenderScene(EditorCamera& editorCamera)
	{
		//架设相机(编辑器使用默认EditorCamera)
		Renderer::SetViewMatrix(editorCamera.GetView());
		Renderer::SetProjectionMatrix(editorCamera.GetProjection());
		Renderer::SetCameraPosition(editorCamera.GetPosition());
		//设置灯光（遍历读取场景中的所有光源，提交到RenderCtx）
		for (auto& entt : m_Entities) {
			if (entt->HasLightComponent) {
				auto& light = entt->GetLightComponent();
				light.AddLightToRenderCtx();
			}
		}

		//执行渲染队列
		Renderer::BeginRendering();
		for (auto& entt : m_Entities) {
			///////////////////////////////////////////////////////////
			//Update Scene(调用场景中所有的MeshRenderer执行Draw())/////
			///////////////////////////////////////////////////////////
			if (entt->HasComponent<MeshRendererComponent>()) {
				mat4 trans = entt->GetTransformComponent().GetTransform();
				//Utils::PrintMat4(trans);
				Renderer::SetModelMatrix(trans);
				Renderer::SetThisDrawCallRenderToScreen();
				entt->GetMeshRendererComponent().Draw();
			}
			
		}
		Renderer::EndRendering();
	};
	void Scene::OnUpdateRuntime() {

		for (auto& entt : m_Entities) {
			if (entt->HasComponent<NativeScriptComponent>()) {
				vector<NativeScriptComponent>&nscs = entt->GetNativeScriptComponents();
				for (auto& nsc : nscs) {
					if (!nsc.m_Owner) {
						nsc.InstantiateScript();
					}
					nsc.UpdateScript();
				}
			}
		}

		///////////////////
		//Set Main Camera//
		///////////////////
		Renderer::BeginRendering();
		for (auto& entt : m_Entities) {
			/////////////////////////////////////////////////////////
			//Update Scene(调用场景中所有的MeshRenderer提交数据)/////
			/////////////////////////////////////////////////////////
			/*Renderer::SetThisDrawCallRenderToScreen();
			Renderer::DrawCall();*/
			//PS:这三行之后要移到MeshRendererComponent里
		}
		Renderer::EndRendering();
	};

	/*template<typename _Type>
	void Scene::AddComponent(Entity* entt, _Type component)
	{
		if (_Type::ms_ComponentType == ComponentType::CameraComponent)
			entt->AddCameraComponent(component);
		if (_Type::ms_ComponentType == ComponentType::LightComponent)
			entt->AddLightComponent(component);
		if (_Type::ms_ComponentType == ComponentType::MeshFilterComponent)
			entt->AddMeshFilterComponent(component);
		if (_Type::ms_ComponentType == ComponentType::MeshRendererComponent)
			entt->AddMeshRendererComponent(component);
		if (_Type::ms_ComponentType == ComponentType::SkeletonComponent)
			entt->AddSkeletonComponent(component);
		if (_Type::ms_ComponentType == ComponentType::TagComponent)
			entt->AddTagComponent(component);
		if (_Type::ms_ComponentType == ComponentType::TransformComponent)
			entt->AddTransformComponent(component);
		if (_Type::ms_ComponentType == ComponentType::NativeScriptComponent)
			entt->AddNativeScriptComponent(component);
	}*/

	/*template<typename _Type>
	void Scene::DeleteComponent(Entity* entt)
	{
		if (_Type::ms_ComponentType == ComponentType::CameraComponent)
			entt->RemoveCameraComponent();
		if (_Type::ms_ComponentType == ComponentType::LightComponent)
			entt->RemoveLightComponent();
		if (_Type::ms_ComponentType == ComponentType::MeshFilterComponent)
			entt->RemoveMeshComponent();
		if (_Type::ms_ComponentType == ComponentType::MeshRendererComponent)
			entt->RemoveMeshRendererComponent();
		if (_Type::ms_ComponentType == ComponentType::SkeletonComponent)
			entt->RemoveSkeletonComponent();
		if (_Type::ms_ComponentType == ComponentType::TagComponent)
			entt->RemoveTagComponent();
		if (_Type::ms_ComponentType == ComponentType::TransformComponent)
			entt->RemoveTransformComponent();
		if (_Type::ms_ComponentType == ComponentType::NativeScriptComponent)
			entt->RemoveNativeScriptComponents();
	}*/


}