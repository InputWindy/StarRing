#include "pch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include "Log.h"
#include <fstream>

#include <glm/gtc/type_ptr.hpp>
namespace StarRing {
	//名称必须成对出现
	OpenGLShader::OpenGLShader(const string v_filepath, const string f_filepath)
	{
		auto lastSlash = v_filepath.find_last_of("/\\");
		lastSlash = lastSlash == string::npos ? 0 : lastSlash + 1;
		auto lastDot = v_filepath.rfind(".");
		auto count = lastDot == string::npos ? v_filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = v_filepath.substr(lastSlash,count);
		SR_CORE_TRACE("Shader名称：{0}",m_Name);

		string vertexCode, fragmentCode;
		std::ifstream in(v_filepath,std::ios::in,std::ios::binary);
		if (in) {
			in.seekg(0,std::ios::end);
			vertexCode.resize(in.tellg());
			in.seekg(0,std::ios::beg);
			in.read(&vertexCode[0], vertexCode.size());
			in.close();
		}
		else {
			assert(0, "VShader文件打开失败！");
		}
		 in = std::ifstream(f_filepath, std::ios::in, std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			fragmentCode.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&fragmentCode[0], fragmentCode.size());
			in.close();
		}
		else {
			assert(0, "FShader文件打开失败！");
		}

		//顶点着色器
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vertexSourceCode = vertexCode.c_str();

		glShaderSource(vertexShader, 1, &vertexSourceCode, 0);
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);

			SR_CORE_ERROR("{0}", infoLog.data());
			assert(0, "顶点着色器编译错误！");
			return;
		}

		//片元着色器
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fragmentSourceCode = fragmentCode.c_str();

		glShaderSource(fragmentShader, 1, &fragmentSourceCode, 0);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);

			SR_CORE_ERROR("{0}", infoLog.data());
			assert(0, "片元着色器编译错误！");
			return;
		}

		
		GLuint program = glCreateProgram();
		m_RendererID = program;

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program,GL_LINK_STATUS,(int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			SR_CORE_ERROR("{0}", infoLog.data());
			assert(0, "片元着色器编译错误！");
			return;
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}
	void OpenGLShader::SetUniformInt(const string name, int value)
	{
		glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}
	void OpenGLShader::SetUniformFloat(const string name, float value)
	{
		glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
	}
	void OpenGLShader::SetUniformFloat2(const string name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), x,y);
	}
	void OpenGLShader::SetUniformFloat3(const string name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), x,y,z);
	}
	void OpenGLShader::SetUniformFloat4(const string name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z,w);
	}
	void OpenGLShader::SetUniformMat3(const string name, mat3 mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void OpenGLShader::SetUniformMat4(const string name, mat4 mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}
	const string OpenGLShader::GetName() const
	{
		return m_Name;
	}
}