#include "pch.h"
#include "RendererCommand.h"
#include "OpenGLRendererAPI.h"

namespace StarRing {
	RendererAPI* RendererCommand::ms_RenderAPI = new OpenGLRendererAPI();
	inline void RendererCommand::Init()
	{
		ms_RenderAPI->Init();
	}
	void RendererCommand::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		ms_RenderAPI->SetViewPort( x,  y,  width,  height);
	}
}