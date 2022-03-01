#pragma once
#include "FrameBuffer.h"
using namespace std;
namespace StarRing {
	class SR_API OpenGLFrameBuffer:public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();
		virtual void Bind()override;
		virtual void UnBind()override;

		virtual void Resize(const uint32_t x, const uint32_t y) override;

		virtual FramebufferSpecification GetSpecifiation()const override;
		virtual uint32_t GetColorAttachmentHandle(int idx)const override;
		virtual uint32_t GetDepthStencilAttachmentHandle()const override;
	protected:
		//��������glfw���ɵ�
		uint32_t m_RendererID = 0, m_DepthStencilAttachment = 0;
		vector<uint32_t> m_ColorAttachments;
		
		//�����FBO���캯����Ҫ�õ���
		FramebufferSpecification m_FrameBufferSpecification;

		vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
	};
}

