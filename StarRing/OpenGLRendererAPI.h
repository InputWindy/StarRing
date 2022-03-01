#pragma once
#include "RendererAPI.h"
namespace StarRing {
	class OpenGLRendererAPI:public RendererAPI
	{
	public:
		//RenderState≥ı ºªØ
		virtual void Init() override;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const vec4 color) override;
		virtual void Clear() override;

		virtual void ActiveTexUnit(unsigned int slot)override;

		virtual void DrawIndexed(const VertexArrayRef& vertexArray) override;

	};
}

