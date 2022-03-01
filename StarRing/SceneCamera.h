#pragma once
#include "Camera.h"
#include <glm/glm.hpp>
#include "MouseEvent.h"
#include "KeyEvent.h"
namespace StarRing {

	class SR_API SceneCamera : public Camera
	{
		friend class SceneHierarchyPanel;
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		SceneCamera(const mat4 projection);
		SceneCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate();
		void OnEvent(Event& e);

		const mat4& GetViewMatrix() const { return m_ViewMatrix; }
		mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		vec3 GetUpDirection() const ;
		vec3 GetRightDirection() const;
		vec3 GetForwardDirection() const;
		const vec3& GetPosition() const { return m_Position; }

		//获取方向
		quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		void MousePan(const vec2& delta);
		void MouseRotate(const vec2& delta);
		void MouseZoom(float delta);

		vec3 CalculatePosition() const;

		pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
		
		mat4 m_ViewMatrix = mat4(1.0f);//v矩阵
		vec3 m_Position = { 0.0f, 0.0f, 0.0f };//相机位置

		vec2 m_InitialMousePosition = { 0.0f, 0.0f };//上一帧鼠标位置

		vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };//相机盯着的点

		float m_Pitch = 0.0f, m_Yaw = 0.0f, m_Row = 0.0f;

		float m_FOV = 45.0f;
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 1.778f;//宽高比
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		float m_Distance = 10.0f;
	};

}
