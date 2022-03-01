#pragma once
#include "Core.h"
#include <glm/glm.hpp>
using namespace glm;
namespace StarRing{
	class SR_API Camera
	{
	public:
		Camera() = default;
		Camera(const mat4 projection) :m_Projection(projection) {};
		~Camera() = default;

		const mat4 GetProjection() { return m_Projection; }

	protected:
		mat4 m_Projection;
	};
}

