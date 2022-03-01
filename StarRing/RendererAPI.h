#pragma once
#include "Core.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "VertexArray.h"
namespace StarRing {
	class SR_API RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};
	public:
		virtual ~RendererAPI() {};

	public:
		virtual void Init() = 0;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const vec4 color) = 0;
		virtual void Clear() = 0;

		virtual void ActiveTexUnit(unsigned int slot) = 0;

		virtual void DrawIndexed(const VertexArrayRef& vertexArray) = 0;

		static inline API GetAPI() { return ms_API; }
	private:
		static API ms_API;

	};
}

