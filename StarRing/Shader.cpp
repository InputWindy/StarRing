#include "pch.h"
#include "OpenGLShader.h"
#include "Renderer.h"
#include "Shader.h"

const string& defaultShaderv = "assets/EngineDefault/shaders/pbr.vert";
const string& defaultShaderf = "assets/EngineDefault/shaders/pbr.frag";

const string& SkyBoxShaderv = "assets/EngineDefault/shaders/skyBox.vert";
const string& SkyBoxShaderf = "assets/EngineDefault/shaders/skyBox.frag";

namespace StarRing {
	

	ShaderRef Shader::Create(const string& v_filePath, const string& f_filePath)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLShader>(v_filePath, f_filePath);
		default:
			break;
		}
	}

	ShaderRef Shader::GetDefault()
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLShader>(defaultShaderv, defaultShaderf);
		default:
			break;
		}
	}
	ShaderRef Shader::GetSkyBox()
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLShader>(SkyBoxShaderv, SkyBoxShaderf);
		default:
			break;
		}
	}
	;
	
	void ShaderLibrary::Add(const ShaderRef& shader)
	{
		auto name = shader->GetName();
		assert(m_Shaders.find(name)!=m_Shaders.end(),"÷ÿ∏¥º”‘ÿµƒShader:{0}",name);
		m_Shaders[name] = shader;
	}
	ShaderRef ShaderLibrary::Load(const string vfilepath, const string ffilepath)
	{
		auto shader = Shader::Create(vfilepath, ffilepath);
		Add(shader);
		return shader;
	}
	ShaderRef ShaderLibrary::Get(const string name)
	{
		if (Exists(name))
			return m_Shaders[name];
		else
			return NULL;
	}
	bool ShaderLibrary::Exists(const string name) const
	{
		return (m_Shaders.find(name) == m_Shaders.end());
	}
}