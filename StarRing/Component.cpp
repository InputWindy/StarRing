#include "pch.h"
#include "Component.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Log.h"
namespace StarRing {
    ComponentType TagComponent::ms_ComponentType = ComponentType::TagComponent;
    ComponentType TransformComponent::ms_ComponentType = ComponentType::TransformComponent;
    ComponentType NativeScriptComponent::ms_ComponentType = ComponentType::NativeScriptComponent;
    ComponentType LightComponent::ms_ComponentType = ComponentType::LightComponent;
    ComponentType CameraComponent::ms_ComponentType = ComponentType::CameraComponent;
    ComponentType MeshFilterComponent::ms_ComponentType = ComponentType::MeshFilterComponent;
    ComponentType MeshRendererComponent::ms_ComponentType = ComponentType::MeshRendererComponent;
    ComponentType SkeletonComponent::ms_ComponentType = ComponentType::SkeletonComponent;
    ComponentType AnimatorComponent::ms_ComponentType = ComponentType::SkeletonComponent;
   
    template<typename T>
    void NativeScriptComponent::Bind()
    {
        InstantiateScript = []() { return static_cast<NativeScript*>(new T()); };
        DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->m_Owner; nsc->m_Owner = nullptr; };

        CreateScript = []() {((T*)m_Owner)->OnCreate(); };
        UpdateScript = []() {((T*)m_Owner)->OnUpdate(); };
    }
    void LightComponent::AddLightToRenderCtx()
    {
        if (m_Type == LightType::DirectionalLight) {
            Renderer::AddDirectionalLight(m_DirectionalLight);
        }
        else if (m_Type == LightType::PointLight) {
            Renderer::AddPointLight(m_PointLight);
        }
        else if (m_Type == LightType::SpotLight) {
            Renderer::AddSpotLight(m_SpotLight);
        }
        else if (m_Type == LightType::AreaLight) {
            Renderer::AddAreaLight(m_AreaLight);
        }
        else {
            assert(0, "光源类型错误！");
        }
    }

    void MeshRendererComponent::Draw()
    {
        if (m_IsEnable) {
            Renderer::SetVertexArray(m_Mesh->m_VertexArray);
            Renderer::SetMaterial(m_Material);

            Renderer::DrawCall();
        }
            
    }
    MeshRendererComponent::MeshRendererComponent(Entity* Owner, MeshRef mesh, MaterialRef material)
        :m_Owner(Owner),m_Mesh(mesh),m_Material(material)
    {
    }
    ;
    
    void CameraComponent::OnUpdate()
    {
        m_Camera.OnUpdate();
    }

}