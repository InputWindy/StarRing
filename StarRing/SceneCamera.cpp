#include "pch.h"
#include "SceneCamera.h"
#include "Event.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Input.h"
#include "DeltaTime.h"
namespace StarRing {

	SceneCamera::SceneCamera()
	{
		m_FOV = 45.0f; m_AspectRatio = 1.778f; m_PerspectiveNear = 0.1f; m_PerspectiveFar = 1000.0f;

		m_ViewMatrix = mat4(1.0f);
		m_Position = { 0.0f, 0.0f, 3.0f };

		m_InitialMousePosition = { 0.0f, 0.0f };

		m_Pitch = 0.0f; m_Yaw = 0.0f;
		m_ViewportWidth = 1280; m_ViewportHeight = 720;
		UpdateProjection();
		UpdateView();
	}

	SceneCamera::SceneCamera(const mat4 projection) :Camera(projection)
	{
		UpdateProjection();
		UpdateView();
	}

	SceneCamera::SceneCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_PerspectiveNear(nearClip), m_PerspectiveFar(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateProjection();
		UpdateView();
	}

	void SceneCamera::OnUpdate()
	{
		float ts = DeltaTime::GetSeconds();
		if (Input::IsKeyPressed(SR_KEY_LEFT_ALT))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseBtnPressed(SR_MOUSE_BUTTON_LEFT))
				MousePan(delta);
			else if (Input::IsMouseBtnPressed(SR_MOUSE_BUTTON_LEFT))
				MouseRotate(delta);
			else if (Input::IsMouseBtnPressed(SR_MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y);
		}
		UpdateView();
	}

	void SceneCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(SceneCamera::OnMouseScroll));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(SceneCamera::OnKeyPressed));
	}

	vec3 SceneCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	vec3 SceneCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	vec3 SceneCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	quat SceneCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void SceneCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
	}

	void SceneCamera::UpdateView()
	{
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool SceneCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetOffsetY() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return true;
	}

	bool SceneCamera::OnKeyPressed(KeyPressedEvent& e)
	{
		float ts = DeltaTime::GetSeconds();
		if (Input::IsKeyPressed(SR_KEY_LEFT_ALT))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseBtnPressed(SR_MOUSE_BUTTON_MIDDLE))
				MousePan(delta);
			else if (Input::IsMouseBtnPressed(SR_MOUSE_BUTTON_LEFT))
				MouseRotate(delta);
			else if (Input::IsMouseBtnPressed(SR_MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y);
		}
		UpdateView();
		return true;
	}

	void SceneCamera::MousePan(const vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void SceneCamera::MouseRotate(const vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void SceneCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	vec3 SceneCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	pair<float, float> SceneCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float SceneCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float SceneCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	
}
