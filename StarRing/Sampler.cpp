#include "pch.h"
#include "Sampler.h"
#include "Shader.h"
#include "RendererCommand.h"
namespace StarRing {
	void Sampler2D::SetSampler2D(ShaderRef shader) {
		shader->Bind();
		for (auto& sampler2d : m_Layout.m_Sampler2Ds) {
			RendererCommand::ActiveTexUnit(sampler2d._texUnit);
			sampler2d._Texture->Bind(sampler2d._texUnit);
			shader->SetUniformInt(sampler2d._nameInShader,sampler2d._texUnit);
		}
	};
}