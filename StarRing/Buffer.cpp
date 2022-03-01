#include "pch.h"
#include "OpenGLBuffer.h"
#include "Renderer.h"
#include "Buffer.h"
float skyboxVertices[] = {
    // ǰ��    
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    //����
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    //����
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     //����
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    //����
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    //����
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
namespace StarRing {
	VertexBufferRef VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLVertexBuffer>(vertices, size);
		default:
			break;
		}
	}
	VertexBufferRef VertexBuffer::GetSkyBoxVBO()
	{
		switch(Renderer::GetRenderAPI())
        {
        case RendererAPI::API::None:break;
        case RendererAPI::API::OpenGL: return make_shared<OpenGLVertexBuffer>(skyboxVertices, sizeof(skyboxVertices));
        default:
            break;
        }
	}
	IndexBufferRef IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLIndexBuffer>(indices, count);
		default:
			break;
		}
	}
	/*IndexBufferRef IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLIndexBuffer>(indices, count);
		default:
			break;
		}
	}*/
}