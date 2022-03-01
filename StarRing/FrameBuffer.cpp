#include "pch.h"
#include "Renderer.h"
#include "OpenGLFrameBuffer.h"
namespace StarRing {
    FrameBufferRef FrameBuffer::Create(const FramebufferSpecification& spec)
    {
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None:break;
		case RendererAPI::API::OpenGL: return make_shared<OpenGLFrameBuffer>(spec);
		default:
			break;
		}
    }
	FramebufferSpecification FramebufferSpecification::GetDefault(uint32_t Width , uint32_t Height, uint32_t samples)
	{
		FramebufferSpecification spec ;
		spec.Height = Height;
		spec.Width = Width;
		spec.Samples = samples;
		spec.SwapChainTarget = false;
		spec.Attachments = FramebufferAttachmentSpecification{
			{FramebufferTextureFormat::RGBA8}
		};
		return spec;
	}
}

