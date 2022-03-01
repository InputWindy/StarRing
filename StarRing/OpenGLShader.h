#pragma once
#include "Shader.h"
namespace StarRing {
	class SR_API OpenGLShader:public Shader
	{
	public:
		OpenGLShader(const string v_filepath, const string f_filepath);
		virtual~OpenGLShader();

		virtual void Bind()const  override;
		virtual void UnBind()const override;

		virtual void SetUniformInt(const string name, int value) override;
		//void SetUniformBool(const string name, bool value);
		virtual void SetUniformFloat(const string name, float value) override;
		virtual void SetUniformFloat2(const string name, float x, float y) override;
		virtual void SetUniformFloat3(const string name, float x, float y, float z) override;
		virtual void SetUniformFloat4(const string name, float x, float y, float z, float w) override;
		virtual void SetUniformMat3(const string name, mat3 mat) override;
		virtual void SetUniformMat4(const string name, mat4 mat) override;

		virtual const string GetName() const override;
	private:
		uint32_t m_RendererID;
		string m_Name;
	};
}	

