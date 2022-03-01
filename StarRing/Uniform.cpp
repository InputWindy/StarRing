#include "pch.h"
#include "Uniform.h"
#include "Shader.h"
namespace StarRing {
    void Uniform::SetUniforms(ShaderRef shader)
    {
		shader->Bind();
		for (auto& uniform : m_Layout.m_Uniforms) {
			switch (uniform._m_UniformType)
			{
			case UNIFORM_INT:shader->SetUniformInt(uniform._nameInShader,uniform.GetData<int>());
			
			case UNIFORM_FLOAT:shader->SetUniformFloat(uniform._nameInShader, uniform.GetData<int>()); break;
			case UNIFORM_FLOAT2:shader->SetUniformFloat2(uniform._nameInShader, uniform.GetData<vec2>().x, uniform.GetData<vec2>().y); break;
			case UNIFORM_FLOAT3:shader->SetUniformFloat3(uniform._nameInShader, uniform.GetData<vec3>().x, uniform.GetData<vec3>().y, uniform.GetData<vec3>().z); break;
			case UNIFORM_FLOAT4:shader->SetUniformFloat4(uniform._nameInShader, uniform.GetData<vec4>().x, uniform.GetData<vec4>().y, uniform.GetData<vec4>().z, uniform.GetData<vec4>().w); break;
			
			case UNIFORM_MAT3:shader->SetUniformMat3(uniform._nameInShader, uniform.GetData<mat3>()); break;
			case UNIFORM_MAT4:shader->SetUniformMat4(uniform._nameInShader, uniform.GetData<mat4>()); break;
			default:
				break;
			};
		}
    }
}
