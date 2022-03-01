#pragma once
#include "Component.h"
#include "Mesh.h"
using namespace std;
namespace StarRing {
	
	class Scene;
	DECLARE_REF(Entity)
	DECLARE_SCOPE(Entity)
	/// <summary>
	/// Entity必须使用引用，不能使用栈，否则会导致子节点全部被销毁
	/// </summary>
	class SR_API Entity
	{
		friend class Scene;
		friend class SceneHierarchyPanel;
		friend class ResourceManager;
		friend class EditorLayer;
	public:
		Entity() = default;
		Entity(const string& name,Scene* scene, Entity* parent = nullptr);
		Entity(const Entity& entity) = default;
		virtual ~Entity();

		virtual void AddTagComponent(TagComponent tag) {
			if (!HasTagComponent) {
				tag.m_Owner = this;
				m_TagComponent = tag;
				HasTagComponent = true;
			}
		}
		virtual void AddTransformComponent(TransformComponent transform) {
			if (!HasTransformComponent) {
				transform.m_Owner = this;
				m_TransformComponent = transform;
				HasTransformComponent = true;
			}
		}
		virtual void AddCameraComponent(CameraComponent camera) {
			if (!HasCameraComponent) {
				camera.m_Owner = this;
				m_CameraComponent = camera;
				HasCameraComponent = true;
			}
		}
		virtual void AddLightComponent(LightComponent light) {
			if (!HasLightComponent) {
				light.m_Owner = this;
				m_LightComponent = light;
				HasLightComponent = true;
			}
		}
		virtual void AddMeshFilterComponent(MeshFilterComponent mesh) {
			if (!HasMeshFilterComponent) {
				mesh.m_Owner = this;
				m_MeshComponent = mesh;
				HasMeshFilterComponent = true;
			}
		}
		virtual void AddMeshRendererComponent(MeshRendererComponent mrc) {
			if (!HasMeshRendererComponent) {
				mrc.m_Owner = this;
				m_MeshRendererComponent = mrc;
				HasMeshRendererComponent = true;
			}
		}
		virtual void AddSkeletonComponent(SkeletonComponent sc) {
			if (!HasSkeletonComponent) {
				m_SkeletonComponent = sc;
				HasSkeletonComponent = true;
			}
		}

		virtual void AddNativeScriptComponent(NativeScriptComponent nsc) {
			for (auto& nsc : m_NativeScriptComponents) {
				if (nsc.ScriptName == nsc.ScriptName) {
					return ;
				}
			}

			m_NativeScriptComponents.push_back(nsc);
		}

		virtual TagComponent& GetTagComponent() {
			if (HasTagComponent) {
				return m_TagComponent;
			}
			assert(0, "没有TagComponent");
		}
		virtual CameraComponent& GetCameraComponent() {
			if (HasCameraComponent) {
				return m_CameraComponent;
			}
			assert(0, "没有TagComponent");
		}
		virtual LightComponent& GetLightComponent() {
			if (HasLightComponent) {
				return m_LightComponent;
			}
			assert(0, "没有TagComponent");
		}
		virtual MeshFilterComponent& GetMeshFilterComponent() {
			if (HasMeshFilterComponent) {
				return m_MeshComponent;
			}
			assert(0, "没有TagComponent");
		}
		virtual MeshRendererComponent& GetMeshRendererComponent() {
			if (HasMeshRendererComponent) {
				return m_MeshRendererComponent;
			}
			assert(0, "没有TagComponent");
		}
		virtual SkeletonComponent& GetSkeletonComponent() {
			if (HasSkeletonComponent) {
				return m_SkeletonComponent;
			}
			assert(0, "没有TagComponent");
		}
		virtual TransformComponent& GetTransformComponent() {
			if (HasTransformComponent) {
				return m_TransformComponent;
			}
			assert(0, "没有TransformComponent");
		}
		virtual NativeScriptComponent& GetNativeScriptComponentByName(const string& name) {
			for (auto& nsc : m_NativeScriptComponents) {
				if (nsc.ScriptName == name)
					return nsc;
			}
			assert(0, "没有NativeScriptComponent");
		}
		
		virtual vector<NativeScriptComponent>& GetNativeScriptComponents();

		virtual void RemoveTagComponent() {
			if (HasTagComponent) {
				HasTagComponent = false;
			}
		};
		virtual void RemoveTransformComponent() {
			if (HasTransformComponent) {
				HasTransformComponent = false;
			}
		};
		virtual void RemoveCameraComponent() {
			if (HasCameraComponent) {
				HasCameraComponent = false;
			}
		};
		virtual void RemoveLightComponent() {
			if (HasLightComponent) {
				HasLightComponent = false;
			}
		};
		virtual void RemoveMeshComponent() {
			if (HasMeshFilterComponent) {
				HasMeshFilterComponent = false;
			}
		};
		virtual void RemoveMeshRendererComponent() {
			if (HasMeshRendererComponent) {
				HasMeshRendererComponent = false;
			}
		};
		virtual void RemoveSkeletonComponent() {
			if (HasSkeletonComponent) {
				HasSkeletonComponent = false;
			}
		};
		virtual void RemoveNativeScriptComponentByName(const string& name) {
			for (auto iter = m_NativeScriptComponents.begin(); iter != m_NativeScriptComponents.end(); ++iter) {
				if (iter->ScriptName == name) {
					m_NativeScriptComponents.erase(iter);
				}
			}
		}
		
		virtual void RemoveNativeScriptComponents() {
			m_NativeScriptComponents.clear();
		}

		template<typename T>
		bool HasComponent() {
			if ((T::ms_ComponentType == ComponentType::TagComponent) && HasTagComponent) {
				return true;
			}
			if ((T::ms_ComponentType == ComponentType::TransformComponent) && HasTransformComponent) {
				return true;
			}
			if ((T::ms_ComponentType == ComponentType::CameraComponent) && HasCameraComponent) {
				return true;
			}
			if ((T::ms_ComponentType == ComponentType::LightComponent) && HasLightComponent) {
				return true;
			}
			if ((T::ms_ComponentType == ComponentType::MeshFilterComponent) && HasMeshFilterComponent) {
				return true;
			}
			if ((T::ms_ComponentType == ComponentType::MeshRendererComponent) && HasMeshRendererComponent) {
				return true;
			}
			if ((T::ms_ComponentType == ComponentType::SkeletonComponent) && HasSkeletonComponent) {
				return true;
			}
			if ((T::ms_ComponentType == ComponentType::NativeScriptComponent) && m_NativeScriptComponents.size()) {
				return true;
			}
			return false;
		};

		virtual void SetParent(Entity* parent);
		virtual const Entity* GetParent()const;
		virtual const vector<EntityRef>& GetChildren()const;
		virtual const string& GetName()const;
		virtual void SetName(const string& name);
		virtual const string& GetFilePath()const;
		virtual void SetScene(Scene* scene = nullptr);
		virtual bool operator==(const Entity& entity);
		virtual bool operator!=(const Entity& entity);

		virtual int GetChildCount()const;

		static EntityRef Create(const string& name, Scene* scene, Entity* parent = nullptr);
		static EntityRef Clone(const Entity& entity);
	protected:
		virtual void AddChildEntity(EntityRef child);		//只添加指针
		virtual void DeleteChildEntity(EntityRef child);	//只删除指针
	protected:
		mat4 m_TransformToParent = mat4(1.0f);
		mat4 m_InvTransformToParent = mat4(1.0f);

		string m_EntityName;
		string m_FilePath = " ";
		Scene* m_Scene = nullptr;
		vector<NativeScriptComponent> m_NativeScriptComponents;

		TagComponent m_TagComponent;
		TransformComponent m_TransformComponent;
		CameraComponent m_CameraComponent;
		LightComponent m_LightComponent;
		SkeletonComponent m_SkeletonComponent;
		MeshFilterComponent m_MeshComponent;
		MeshRendererComponent m_MeshRendererComponent;


		Entity* m_Parent = nullptr;
		vector<EntityRef> m_Children;

		bool HasTagComponent = false, HasTransformComponent = false;
		bool HasCameraComponent = false, HasLightComponent = false;
		bool HasSkeletonComponent = false, HasMeshFilterComponent = false;
		bool HasMeshRendererComponent = false;
	};
	
}

