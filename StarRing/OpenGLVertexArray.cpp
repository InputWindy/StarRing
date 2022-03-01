#include "pch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>
namespace StarRing {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
			case StarRing::ShaderDataType::Float:		 return GL_FLOAT;
			case StarRing::ShaderDataType::Float2:		 return GL_FLOAT;
			case StarRing::ShaderDataType::Float3:		 return GL_FLOAT;
			case StarRing::ShaderDataType::Float4:		 return GL_FLOAT;
			case StarRing::ShaderDataType::Mat3:		 return GL_FLOAT;
			case StarRing::ShaderDataType::Mat4:		 return GL_FLOAT;
			case StarRing::ShaderDataType::Int:			 return GL_INT;
			case StarRing::ShaderDataType::Int2:		 return GL_INT;
			case StarRing::ShaderDataType::Int3:		 return GL_INT;
			case StarRing::ShaderDataType::Int4:		 return GL_INT;
			case StarRing::ShaderDataType::Bool:		 return GL_BOOL;
			default:
				break;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1,&m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
	uint32_t OpenGLVertexArray::GetHandle() const
	{
		return m_RendererID;
	}
	void OpenGLVertexArray::AddVertexBuffer(const VertexBufferRef& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		//layout不能为空
		assert(vertexBuffer->GetBufferLayout().GetElements().size());

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetBufferLayout();
		for (const auto& ele : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				ele.GetElementCount(),
				ShaderDataTypeToOpenGLBaseType(ele.Type),
				ele.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)ele.Offset);
			++index;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	//IndexBuffer没有layout
	void OpenGLVertexArray::SetIndexBuffer(const IndexBufferRef& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}