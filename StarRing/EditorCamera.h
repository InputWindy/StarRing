#pragma once

#include "Camera.h"
#include "DeltaTime.h"
#include "Event.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include <glm/glm.hpp>
using namespace glm;
using namespace std;
namespace StarRing {

	class SR_API EditorCamera : public Camera
	{
	public:
		EditorCamera(vec3 position = vec3(0.0f, 0.0f, 10.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f);
		EditorCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate();
		void OnEvent(Event& e);

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const mat4& GetViewMatrix() const { return m_ViewMatrix; }
		mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }
		mat4 GetView() const { return m_ViewMatrix; }
		mat4 GetProjection() const { return  m_Projection; }

		vec3 GetUpDirection() const { return m_Up; };
		vec3 GetRightDirection() const { return m_Right; };
		vec3 GetForwardDirection() const { return m_Front; };
		const vec3& GetPosition() const { return m_Position; }

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		static inline float m_MoveSpeed = 5.0f;
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);

		void UpdateCamera();
		void UpdateVectors();
	private:
		float lastX = m_ViewportWidth / 2.0f;
		float lastY = m_ViewportHeight / 2.0f;
		bool firstMouse = true;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		//VP矩阵
		mat4 m_ViewMatrix = mat4(1.0f);

		//相机属性
		vec3 m_Position = { 0.0f, 0.0f, 3.0f };
		vec3 m_Front = { 0.0f, 0.0f, -1.0f };
		vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		vec3 m_Right = { 1.0f, 0.0f, 0.0f };
		vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

		//旋转
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		//相机设置
		float m_MovementSpeed = 5.0f;
		float m_MouseSensitivity = 0.1f;
		float m_Zoom = 45.0f;
		
	};

}
