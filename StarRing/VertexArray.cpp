#include "pch.h"
#include "OpenGLBuffer.h"
#include "Renderer.h"
#include "OpenGLVertexArray.h"
#include "VertexArray.h"
namespace StarRing {
	VertexArrayRef VertexArray::Create()
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLVertexArray>();
		default:
			break;
		}
	}
	VertexArrayRef VertexArray::GetSkyBoxVAO()
	{
		VertexArrayRef ret = VertexArray::Create();
		VertexBufferRef m_VertexBuffer = VertexBuffer::GetSkyBoxVBO();
		m_VertexBuffer->Bind();

		BufferLayout layout = {
			{ShaderDataType::Float3,"a_Pos"},
		};
		m_VertexBuffer->SetLayout(layout);
		ret->AddVertexBuffer(m_VertexBuffer);
		return ret;
	}
}