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
	//����������Ĭ����Tag��Transform
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
			//�ڳ������ҵ���ʵ��
			if ((*iter)->GetName() == name) {
				//�ҵ�ʵ�����ɾ����ʵ���������ʵ��
				for (auto child : (*iter)->m_Children) {
					DestroyEntity(child->GetName());
				}
				//cout << (*iter)->m_Children.size() << endl;;
				//�����ʵ���ɾ����ǰʵ��(����и��ڵ㣬���Ƚ��,Ȼ��ӳ�����ɾ���Լ�)
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
			SR_CORE_WARN("ʵ�����ƣ�{0},������Ŀ��{1}",entt->m_EntityName.c_str(),entt.use_count());
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
		//�������(�༭��ʹ��Ĭ��EditorCamera)
		Renderer::SetViewMatrix(editorCamera.GetView());
		Renderer::SetProjectionMatrix(editorCamera.GetProjection());
		Renderer::SetCameraPosition(editorCamera.GetPosition());
		//���õƹ⣨������ȡ�����е����й�Դ���ύ��RenderCtx��
		for (auto& entt : m_Entities) {
			if (entt->HasLightComponent) {
				auto& light = entt->GetLightComponent();
				light.AddLightToRenderCtx();
			}
		}

		//ִ����Ⱦ����
		Renderer::BeginRendering();
		for (auto& entt : m_Entities) {
			///////////////////////////////////////////////////////////
			//Update Scene(���ó��������е�MeshRendererִ��Draw())/////
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
			//Update Scene(���ó��������е�MeshRenderer�ύ����)/////
			/////////////////////////////////////////////////////////
			/*Renderer::SetThisDrawCallRenderToScreen();
			Renderer::DrawCall();*/
			//PS:������֮��Ҫ�Ƶ�MeshRendererComponent��
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