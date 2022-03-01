#include "pch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
void StarRing::OpenGLRendererAPI::Init()
{
	SetClearColor(vec4(0.0f, 0.0f, 0.0f, 0.0f));
	Clear();
}
void StarRing::OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}
void StarRing::OpenGLRendererAPI::SetClearColor(const vec4 color)
{
	glClearColor(color.r,color.g,color.b,color.a);
}

void StarRing::OpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void StarRing::OpenGLRendererAPI::ActiveTexUnit(unsigned int slot) {
	glActiveTexture(slot);
}

void StarRing::OpenGLRendererAPI::DrawIndexed(const VertexArrayRef& vertexArray)
{
	glDrawElements(GL_TRIANGLES,vertexArray->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,nullptr);
}
