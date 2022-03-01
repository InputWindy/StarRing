#include "SceneHierarchyPanel.h"
#include "ImguiLayer.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "Input.h"
#include <glm/gtc/type_ptr.hpp>
#include "Log.h"
namespace StarRing {

	static bool ShowAddComponentSeletable = false;

	const vector<string> items =
	{
		"Camera",
		"MeshRenderer",
		"Light",
		"Skeleton"
	};

	static void Vec3UI(const char* label,vec3& data){
		ImGui::DragFloat3(label, value_ptr(data), 0.5f);
	}

	static void FloatUI(const char* label, float* data) {
		ImGui::DragFloat(label, data, 0.5f);
	}

	static void BoolUI(const char* label, bool* data) {
		ImGui::Checkbox(label, data);
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const SceneRef& scene)
	{
		SetScene(scene);
	}
	void SceneHierarchyPanel::SetScene(const SceneRef& scene)
	{
		m_Scene = scene;
	}
	void SceneHierarchyPanel::OnUpdate()
	{
		#pragma region 组件删除
		if (m_SceneHierarchyCtx.m_bDeleteCameraComponent) {
			m_Scene->DeleteComponent<CameraComponent>(m_SceneHierarchyCtx.m_SelectedEntity);
			m_SceneHierarchyCtx.m_bDeleteCameraComponent = false;
		}
		if (m_SceneHierarchyCtx.m_bDeleteLightComponent) {
			m_Scene->DeleteComponent<LightComponent>(m_SceneHierarchyCtx.m_SelectedEntity);
			m_SceneHierarchyCtx.m_bDeleteLightComponent = false;
		}
		if (m_SceneHierarchyCtx.m_bDeleteMeshRendererComponent) {
			m_Scene->DeleteComponent<MeshRendererComponent>(m_SceneHierarchyCtx.m_SelectedEntity);
			m_SceneHierarchyCtx.m_bDeleteMeshRendererComponent = false;
		}
		if (m_SceneHierarchyCtx.m_bDeleteSkeletonComponent) {
			m_Scene->DeleteComponent<SkeletonComponent>(m_SceneHierarchyCtx.m_SelectedEntity);
			m_SceneHierarchyCtx.m_bDeleteSkeletonComponent = false;
		}
		if (m_SceneHierarchyCtx.m_bDeleteAnimatorComponent) {
			m_Scene->DeleteComponent<AnimatorComponent>(m_SceneHierarchyCtx.m_SelectedEntity);
			m_SceneHierarchyCtx.m_bDeleteAnimatorComponent = false;
		}
		#pragma endregion

		#pragma region 组件添加
		if (m_SceneHierarchyCtx.m_bAddComponent) {
			if (m_SceneHierarchyCtx.m_AddComponentType == "Camera") {
				CameraComponent camera = CameraComponent();
				m_Scene->AddComponent<CameraComponent>(m_SceneHierarchyCtx.m_SelectedEntity, camera);
			}
			if (m_SceneHierarchyCtx.m_AddComponentType == "MeshRenderer") {
				MeshRendererComponent mrc = MeshRendererComponent();
				m_Scene->AddComponent<MeshRendererComponent>(m_SceneHierarchyCtx.m_SelectedEntity, mrc);
			}
			if (m_SceneHierarchyCtx.m_AddComponentType == "Light") {
				LightComponent lc = LightComponent();
				m_Scene->AddComponent<LightComponent>(m_SceneHierarchyCtx.m_SelectedEntity, lc);
			}
			if (m_SceneHierarchyCtx.m_AddComponentType == "Skeleton") {
				SkeletonComponent sc = SkeletonComponent();
				m_Scene->AddComponent<SkeletonComponent>(m_SceneHierarchyCtx.m_SelectedEntity,sc );
			}
			m_SceneHierarchyCtx.m_bAddComponent = false;
		}
		#pragma endregion

		
		//实体改名
		if (m_SceneHierarchyCtx.m_bChangeSelectedEntityName) {
			const string& name = string(m_SceneHierarchyCtx.buf);
			m_SceneHierarchyCtx.m_SelectedEntity->GetTagComponent().SetTag(name);
			m_SceneHierarchyCtx.m_SelectedEntity->SetName(name);
			m_SceneHierarchyCtx.m_SelectedEntity = nullptr;
			m_SceneHierarchyCtx.m_bChangeSelectedEntityName = false;
		}


		//实体删除
		if (m_SceneHierarchyCtx.m_bDeleteSelectedEntity) {

			m_Scene->DestroyEntity(m_SceneHierarchyCtx.m_SelectedEntity->GetName());
			m_SceneHierarchyCtx.m_SelectedEntity = nullptr;

			m_SceneHierarchyCtx.m_bDeleteSelectedEntity = false;
		}
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		for (auto& entt : m_Scene->GetEntities()) {
			if (entt->GetParent())
				continue;
			DrawEntityNode(entt.get());
		}

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
			char buf[256];
			memset(buf, 0, sizeof(buf));
			ImGui::Text("Create New Entity");
			if (ImGui::InputText("", buf, sizeof(buf))) {
				if (Input::IsKeyPressed(SR_KEY_ENTER)) {
					m_Scene->CreateEntity(string(buf));
				}
			}
            ImGui::EndPopup();
        }
		ImGui::End();

		ImGui::Begin("Inspector");
		DrawComponents(m_SceneHierarchyCtx.m_SelectedEntity);
		ImGui::End();
	}
	Entity* SceneHierarchyPanel::GetSelectedEntity() const
	{
		return m_SceneHierarchyCtx.m_SelectedEntity;
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity* entity)
	{
		ImGuiTreeNodeFlags flags = ((m_SceneHierarchyCtx.m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx(entity->GetName().c_str());

		if (ImGui::IsItemClicked() | ImGui::IsItemClicked(1))
		{
			m_SceneHierarchyCtx.m_SelectedEntity = entity;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity")) {
				m_SceneHierarchyCtx.m_bDeleteSelectedEntity = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			for (auto entt : entity->GetChildren()) {
				DrawEntityNode(entt.get());
			}
			ImGui::TreePop();
		}
	}
	void SceneHierarchyPanel::DrawComponents(Entity* entity)
	{

		if (entity) {
			//不可删除组件
			if (entity->HasComponent<TagComponent>()) {
				if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(),ImGuiTreeNodeFlags_DefaultOpen,"Tag")) {
					
					memset(m_SceneHierarchyCtx.buf, 0, sizeof(m_SceneHierarchyCtx.buf));
					strcpy_s(m_SceneHierarchyCtx.buf, sizeof(m_SceneHierarchyCtx.buf), entity->GetTagComponent().Tag.c_str());
					if (ImGui::InputText(" ", m_SceneHierarchyCtx.buf, sizeof(m_SceneHierarchyCtx.buf))) {
						if (Input::IsKeyPressed(SR_KEY_ENTER)) {
							m_SceneHierarchyCtx.m_bChangeSelectedEntityName = true;
						}
					}
					ImGui::TreePop();
				}
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
			}
			if (entity->HasComponent<TransformComponent>()) {
				if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
					TransformComponent& trans = entity->GetTransformComponent();
					Vec3UI("Position", trans.Translation);
					Vec3UI("Rotation", trans.Rotation);
					Vec3UI("Scale", trans.Scale);
					ImGui::TreePop();
				}
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
			}
			if (entity->HasComponent<MeshFilterComponent>()) {
				if (ImGui::TreeNodeEx((void*)typeid(MeshFilterComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "MeshFilter")) {
					MeshFilterComponent& mc = entity->GetMeshFilterComponent();
					char buf[256];
					strcpy_s(buf, sizeof(buf), mc.m_Mesh->m_MeshName.c_str());
					if (ImGui::InputText(" ", buf, sizeof(buf))) {

					}
					ImGui::TreePop();
				}
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
			}
			//可删除组件
			if (entity->HasComponent<CameraComponent>()) {
				bool opened = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera");
				
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component")) {
						m_SceneHierarchyCtx.m_bDeleteCameraComponent = true;
					}
					ImGui::EndPopup();
				}
				if (opened) {
					CameraComponent& cc = entity->GetCameraComponent();
					BoolUI("Primary:", &(cc.Primary)); ImGui::SameLine(); BoolUI("FixedAspectRatio:", &(cc.FixedAspectRatio));

					ImGui::TreePop();
				}
				
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
			}
			if (entity->HasComponent<LightComponent>()) {
				bool opened = ImGui::TreeNodeEx((void*)typeid(LightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Light");

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component")) {
						m_SceneHierarchyCtx.m_bDeleteLightComponent = true;
					}
					ImGui::EndPopup();
				}
				if (opened) {
					LightComponent& cc = entity->GetLightComponent();
					ImGui::ColorEdit3("Light Color",&cc.m_DirectionalLight.dColor[0]);
					ImGui::TreePop();
				}

				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
			}
			if (entity->HasComponent<MeshRendererComponent>()) {
				bool opened = ImGui::TreeNodeEx((void*)typeid(MeshRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "MeshRenderer");

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component")) {
						m_SceneHierarchyCtx.m_bDeleteMeshRendererComponent = true;
					}
					ImGui::EndPopup();
				}
				if (opened) {
					MeshRendererComponent& mrc = entity->GetMeshRendererComponent();
					BoolUI("Enable", &(mrc.m_IsEnable)); 
					char buf[256];
					strcpy_s(buf, sizeof(buf), mrc.m_Mesh->m_MeshName.c_str());
					if (ImGui::InputText("Mesh Name:", buf, sizeof(buf))) {

					}
					ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
					strcpy_s(buf, sizeof(buf), mrc.m_Material->m_MaterialName.c_str());
					if (ImGui::InputText("Material Name:", buf, sizeof(buf))) {

					}

					ImGui::TreePop();
				}

				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
			}
			if (entity->HasComponent<SkeletonComponent>()) {
				bool opened = ImGui::TreeNodeEx((void*)typeid(SkeletonComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Skeleton");

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component")) {
						m_SceneHierarchyCtx.m_bDeleteSkeletonComponent = true;
					}
					ImGui::EndPopup();
				}
				if (opened) {
					SkeletonComponent& sc = entity->GetSkeletonComponent();

					ImGui::TreePop();
				}

				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
			}
			if (entity->HasComponent<NativeScriptComponent>()) {
				bool opened = ImGui::TreeNodeEx((void*)typeid(NativeScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "NativeScript");

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Component")) {
						m_SceneHierarchyCtx.m_bDeleteSkeletonComponent = true;
					}
					ImGui::EndPopup();
				}
				if (opened) {
					//TODO:特殊组件
					ImGui::TreePop();
				}

				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
			}
			
			if(ImGui::Button("Add Component")){
				ShowAddComponentSeletable = !ShowAddComponentSeletable;
			};

			if (ShowAddComponentSeletable) {
				for (int n = 0; n < items.size(); n++)
				{
					if (ImGui::Selectable(items[n].c_str(), false)) {
						ShowAddComponentSeletable = false;
						m_SceneHierarchyCtx.m_AddComponentType = items[n];
						m_SceneHierarchyCtx.m_bAddComponent = true;
					}
				}
			}
			//ImGui::ShowDemoWindow();
		}
		
	}
}