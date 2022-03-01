#pragma once
#include "Pass.h"
#include "RenderState.h"
#include "Shader.h"
namespace StarRing {

	DECLARE_REF(Material)
	DECLARE_SCOPE(Material)
	class SR_API Material
	{
		friend class Renderer;
		friend class MeshRendererComponent;
		friend class SceneHierarchyPanel;
	public:
		Material() = default;
		Material(UniformRef uniforms, Sampler2DRef samplers,ShaderRef shader,RenderState renderstate ) 
			:m_Uniforms(uniforms),m_Samplers(samplers),m_Shader(shader) ,m_RenderState(renderstate){
			m_MaterialName = m_Shader->GetName();
		};
		~Material() = default;

		static MaterialRef Create(UniformRef uniforms, Sampler2DRef samplers,ShaderRef shader, RenderState renderstate) {
			return make_shared<Material>(uniforms, samplers, shader, renderstate);
		};
		//��ȡĬ�ϲ���(pbr)
		static MaterialRef GetDefault()
		{
			return make_shared<Material>(Uniform::GetDefault(),Sampler2D::GetDefault(), Shader::GetDefault(),RenderState::GetDefault());
		};

		static MaterialRef Clone(const Material& material) {
			Material ret = *new Material(material);
			return make_shared<Material>(ret);
		};
	private:
		UniformRef		m_Uniforms;
		Sampler2DRef	m_Samplers;
		RenderState		m_RenderState;

		ShaderRef		m_Shader;

		string			m_MaterialName;
		//TODO:��Ӷ����Pass�������Pass�������õ�
	};
}

