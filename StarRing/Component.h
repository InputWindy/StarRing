#pragma once
#include "Core.h"
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "NativeScript.h"
#include "Light.h"
#include "Skeleton.h"
#include "VertexArray.h"
#include "SceneCamera.h"
#include "Mesh.h"
#include "Material.h"
using namespace std;
using namespace glm;
namespace StarRing {
	enum class ComponentType
	{
		None = 0, TagComponent = 1, TransformComponent = 2,
		CameraComponent = 3, NativeScriptComponent = 4,
		MeshFilterComponent = 5, MeshRendererComponent = 6,
		LightComponent = 7,
		SkeletonComponent = 8
	};

	class SR_API LightComponent
	{
		friend class Scene;
	public:
		LightType m_Type = LightType::DirectionalLight;
		union 
		{
			DirectionalLight	m_DirectionalLight;
			PointLight			m_PointLight;
			SpotLight			m_SpotLight;
			AreaLight			m_AreaLight;
		};
		LightComponent() {};
		~LightComponent() = default;

		template<typename T>
		inline void SetLight(T& light)
		{
			assert(0, "光源类型错误！");
		};

		template<>
		inline void SetLight<DirectionalLight>(DirectionalLight& light) {
			m_DirectionalLight = light;
			m_Type = LightType::DirectionalLight;
		}

		template<>
		inline void SetLight<PointLight>(PointLight& light) {
			m_PointLight = light;
			m_Type = LightType::PointLight;
		}

		template<>
		inline void SetLight<SpotLight>(SpotLight& light) {
			m_SpotLight = light;
			m_Type = LightType::SpotLight;
		}

		template<>
		inline void SetLight<AreaLight>(AreaLight& light) {
			m_AreaLight = light;
			m_Type = LightType::AreaLight;
		}



		template<typename T>
		T& GetLight()
		{
			if (T::ms_LightType == LightType::DirectionalLight) {
				return m_DirectionalLight;
			}
			if (T::ms_LightType == LightType::PointLight) {
				return m_PointLight;
			}
			if (T::ms_LightType == LightType::SpotLight) {
				return m_SpotLight;
			}
			if (T::ms_LightType == LightType::AreaLight) {
				return m_AreaLight;
			}
			assert(0, "光源类型错误");
		};

		static ComponentType ms_ComponentType;
		Entity* m_Owner = nullptr;

		bool m_IsEnable = true;
	private:
		void AddLightToRenderCtx();
	};

	class SR_API TagComponent
	{
		public:
			std::string Tag;
			Entity* m_Owner = nullptr;
			TagComponent() = default;
			TagComponent(const TagComponent&) = default;
			TagComponent(const std::string & tag)
				: Tag(tag) {}

			void SetOwner(Entity* entt) {
				m_Owner = entt;
			}
			void operator=(const TagComponent& nsc) {
				Tag = nsc.Tag;
			}
			void SetTag(const string& tag) {
				Tag = tag;
			}
			static ComponentType ms_ComponentType;
	};

	class SR_API TransformComponent
	{
	public:
		vec3 Translation = { 0.0f, 0.0f, 0.0f };//Position
		vec3 Rotation = { 0.0f, 0.0f, 0.0f };//绕x,y,z旋转角度
		vec3 Scale = { 0.01f,0.01f,0.01f };	//x,y,z放大倍数

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		//Transform组件复制粘贴
		TransformComponent(const vec3& translation)
			: Translation(translation) {}

		static ComponentType ms_ComponentType;
		Entity* m_Owner = nullptr;

		void SetOwner(Entity* entt) {
			m_Owner = entt;
		}

		//获取Entity当前位置
		mat4 GetTransform() const
		{
			mat4 rotation = toMat4(quat(Rotation));

			return translate(mat4(1.0f), Translation)
				* rotation
				* scale(mat4(1.0f), Scale);
		}
		void operator=(const TransformComponent& nsc) {
			Translation = nsc.Translation;
			Rotation = nsc.Rotation;
			Scale = nsc.Scale;
		}
	};

	//带着相机组件的物体就是相机，但是只有一个相机是主相机
	class SR_API CameraComponent
	{
	public:
		SceneCamera m_Camera;
		bool Primary = true;
		bool m_IsEnable = true;

		//FixedAspectRatio：是否固定宽高比
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		static ComponentType ms_ComponentType;
		Entity* m_Owner = nullptr;
		void SetOwner(Entity* entt) {
			m_Owner = entt;
		}
		void OnUpdate();

		const mat4& GetViewMatrix() const {
			return m_Camera.GetViewMatrix();
		};
		mat4 GetViewProjection() const {
			return m_Camera.GetViewProjection();
		};

		vec3 GetUpDirection() const {
			return m_Camera.GetUpDirection();
		};
		vec3 GetRightDirection() const {
			return m_Camera.GetRightDirection();
		};
		vec3 GetForwardDirection() const {
			return m_Camera.GetForwardDirection();
		};
		const vec3& GetPosition() const {
			return m_Camera.GetPosition();
		};

		//获取方向
		quat GetOrientation() const {
			return m_Camera.GetOrientation();
		};

		float GetPitch() const {
			return m_Camera.GetPitch();
		};
		float GetYaw() const {
			return m_Camera.GetYaw();
		};
	};

	//NativeScriptComponent维护一个NativeScript，用于执行其中的固定函数
	class SR_API NativeScriptComponent 
	{
	public:
		bool m_IsEnable = true;

		NativeScriptComponent() = default;
		NativeScriptComponent(const string& name):ScriptName(name) {};
		NativeScriptComponent(const NativeScriptComponent&) = default;
		NativeScriptComponent(NativeScript* ns,const string& name)
			: m_Owner(ns),ScriptName(name) {}
		~NativeScriptComponent() { delete m_Owner; m_Owner = nullptr; };

		NativeScript* m_Owner = nullptr;
		string ScriptName = string("New NativeScript");

		using OnInitial = function<NativeScript*(void)>;
		using OnDestroy = function<void(NativeScriptComponent*)>;

		using OnCreate	= function<void(void)>;
		using OnUpdate	= function<void(void)>;

		OnInitial	InstantiateScript;
		OnDestroy	DestroyScript;

		OnCreate	CreateScript;
		OnUpdate	UpdateScript;

		template<typename T>
		void Bind();

		void operator=(const NativeScriptComponent& nsc) {
			m_Owner = nsc.m_Owner;
			ScriptName = nsc.ScriptName;
			InstantiateScript = nsc.InstantiateScript;
			DestroyScript = nsc.DestroyScript;

			CreateScript = nsc.CreateScript;
			UpdateScript = nsc.UpdateScript;
		}
		static ComponentType ms_ComponentType;
	};

	//维护Mesh数据（Mesh不仅可以用于渲染，还可以用于碰撞检测，触发器，所以与网格渲染器分开）
	class SR_API MeshFilterComponent
	{
	public:
		MeshFilterComponent() = default;
		MeshFilterComponent(const string& name,VertexArrayRef mesh, Entity* owner) {
			SetMesh(name,mesh,owner);
		};
		~MeshFilterComponent() = default;

		void SetMesh(const string& name, VertexArrayRef mesh, Entity* owner) {
			m_Mesh = make_shared<Mesh>(name, mesh, owner);
		};
		MeshRef GetMesh() {
			return m_Mesh;
		};

		MeshRef m_Mesh;
		Entity* m_Owner = nullptr;
		void SetOwner(Entity* entt) {
			m_Owner = entt;
		}

		static ComponentType ms_ComponentType;
	};

	//网格渲染器实际上只维护一个Draw方法
	class SR_API MeshRendererComponent
	{
	public:
		MeshRendererComponent()= default;
		MeshRendererComponent(Entity* Owner, MeshRef mesh, MaterialRef material);
		~MeshRendererComponent() = default;
		void Draw();
		MaterialRef				m_Material;
		MeshRef					m_Mesh;
		static ComponentType	ms_ComponentType;
		Entity* m_Owner = nullptr;
		void SetOwner(Entity* entt) {
			m_Owner = entt;
		}
		bool m_IsEnable = true;
	};

	//骨骼组件维护模型骨骼数据（还没实现相关方法）
	class SR_API SkeletonComponent
	{
	public:
		SkeletonComponent() = default;
		~SkeletonComponent() = default;

		Skeleton m_Skeleton;

		static ComponentType ms_ComponentType;
	};


	class SR_API AnimatorComponent
	{
	public:
		AnimatorComponent() = default;
		~AnimatorComponent() = default;

		static ComponentType ms_ComponentType;
		Entity* m_Owner = nullptr;

		void SetOwner(Entity* entt) {
			m_Owner = entt;
		}
	private:

	};

}

