#pragma once
#include "Core.h"
#include <glm/glm.hpp>
using namespace glm;
namespace StarRing {
	//正交相机	
	class SR_API OrthographicCamera
	{
	public:
		OrthographicCamera(float left,float right,float bottom,float top);

		void SetProjection(float left, float right, float bottom, float top);


		vec3 GetPosition() const { return m_Position; }
		float GetRotation()const { return m_Rotation; }

		void SetPosition(const vec3 position) { m_Position = position; UpdateCamera(); }
		void SetRotation(float rotation) { m_Rotation = rotation; UpdateCamera(); }

		const mat4 GetProjectionMatrix()const { return m_ProjectionMatrix; }
		const mat4 GetViewMatrix()const { return m_ViewMatrix; }
		const mat4 GetViewProjectionMatrix()const { return m_ViewProjectionMatrix; }

	private:
		void UpdateCamera() ;
	private:
		mat4 m_ViewMatrix;
		mat4 m_ProjectionMatrix;
		mat4 m_ViewProjectionMatrix;

		vec3 m_Position = {0.0f,0.0f,0.0f};
		float m_Rotation = 0.0f;
	};
}

