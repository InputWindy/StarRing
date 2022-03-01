#pragma once
#include "Core.h"
namespace StarRing {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

	};

	struct SR_API FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct SR_API FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct SR_API FramebufferSpecification
	{
		uint32_t Width = 1280, Height = 720;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;

		static FramebufferSpecification GetDefault(uint32_t Width = 1280, uint32_t Height = 720, uint32_t samples = 1);
	};

	//一个FrameBuffer实例已经包含了一个颜色缓冲和一个深度模板缓冲
	DECLARE_REF(FrameBuffer)
	DECLARE_SCOPE(FrameBuffer)
	class SR_API FrameBuffer
	{
	public:
		virtual FramebufferSpecification GetSpecifiation()const = 0;

		static FrameBufferRef Create(const FramebufferSpecification& spec) ;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Resize(const uint32_t x, const uint32_t y) = 0;

		virtual uint32_t GetColorAttachmentHandle(int idx)const = 0;
		virtual uint32_t GetDepthStencilAttachmentHandle()const = 0;
	};
}

