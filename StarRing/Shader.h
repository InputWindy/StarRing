#pragma once
#include "Core.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
using namespace std;
using namespace glm;
namespace StarRing {

	DECLARE_REF(Shader)
	DECLARE_SCOPE(Shader)
	class SR_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		virtual const string GetName() const = 0;

		virtual void SetUniformInt(const string name, int value) = 0;
		//void SetUniformBool(const string name, bool value);
		virtual void SetUniformFloat(const string name, float value) = 0;
		virtual void SetUniformFloat2(const string name, float x, float y) = 0;
		virtual void SetUniformFloat3(const string name, float x, float y, float z) = 0;
		virtual void SetUniformFloat4(const string name, float x, float y, float z, float w) = 0;
		virtual void SetUniformMat3(const string name, mat3 mat) = 0;
		virtual void SetUniformMat4(const string name, mat4 mat) = 0;

		static ShaderRef Create(const string& v_filePath, const string& f_filePath);

		static ShaderRef GetDefault();
		static ShaderRef GetSkyBox();
	};

	class ShaderLibrary
	{
	public:

		void Add(const ShaderRef& shader);
		ShaderRef Load(const string vfilepath, const string ffilepath);

		ShaderRef Get(const string name);
		bool Exists(const string name)const;
	private:
		unordered_map<string,ShaderRef> m_Shaders;
	};

	
}

