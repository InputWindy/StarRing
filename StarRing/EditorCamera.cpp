#include "pch.h"
#include "EditorCamera.h"
#include "Core.h"
#include "Input.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Log.h"
namespace StarRing {
	enum Movement
	{
		Forward = 0,
		Back = 1,
		Left = 2,
		Right = 3
	};

	EditorCamera::EditorCamera(vec3 position, vec3 up, float yaw, float pitch)
	{
		m_Position = position;
		m_WorldUp = up;
		m_Yaw = yaw;
		m_Pitch = pitch;
		UpdateCamera();
	}
	EditorCamera::EditorCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	{
		m_Position = vec3(posX, posY, posZ);
		m_WorldUp = vec3(upX, upY, upZ);
		m_Yaw = yaw;
		m_Pitch = pitch;
		UpdateCamera();
	}
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateCamera();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		/*for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				cout << m_ViewMatrix[i][j] << " ";
			}
			cout << endl;
		}*/
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float xoffset = e.GetOffsetX(), yoffset = e.GetOffsetY();
		m_Zoom -= (float)yoffset;
		if (m_Zoom < 1.0f)
			m_Zoom = 1.0f;
		if (m_Zoom > 45.0f)
			m_Zoom = 45.0f;
		UpdateCamera();
		return true;
	}

	bool EditorCamera::OnKeyPressed(KeyPressedEvent& e)
	{
		float delta = DeltaTime::GetSeconds();
		int keycode = e.GetKeyCode();
		if (keycode == SR_KEY_W) {
			m_Position += GetForwardDirection() * delta;
		}
		if (keycode == SR_KEY_S) {
			m_Position -= GetForwardDirection() * delta;
		}
		if (keycode == SR_KEY_A) {
			m_Position -= GetRightDirection() * delta;
		}
		if (keycode == SR_KEY_D) {
			m_Position += GetRightDirection() * delta;
		}
		//SR_CORE_TRACE("({0},{1},{2})", m_Position.x, m_Position.y, m_Position.z);
		UpdateView();
		return true;
	}

	bool EditorCamera::OnMouseMoved(MouseMovedEvent& e)
	{
		float xoffset = e.GetMouseX(), yoffset = e.GetMouseY();
		
		xoffset *= m_MouseSensitivity;
		yoffset *= m_MouseSensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		UpdateVectors();
		UpdateView();
		return true;
	}

	void EditorCamera::UpdateCamera()
	{
		UpdateProjection();
		UpdateView();
	}

	void EditorCamera::UpdateVectors()
	{
		vec3 front;
		front.x = cos(radians(m_Yaw)) * cos(radians(m_Pitch));
		front.y = sin(radians(m_Pitch));
		front.z = sin(radians(m_Yaw)) * cos(radians(m_Pitch));
		m_Front = normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = normalize(cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = normalize(cross(m_Right, m_Front));
	}

	void EditorCamera::OnUpdate()
	{
		if (Input::IsKeyPressed(SR_KEY_LEFT_ALT)) {
			if (Input::IsKeyPressed(SR_KEY_W)) {
				KeyPressedEvent e(SR_KEY_W, 1);
				OnEvent(e);
			}
			if (Input::IsKeyPressed(SR_KEY_A)) {
				KeyPressedEvent e(SR_KEY_A, 1);
				OnEvent(e);
			}
			if (Input::IsKeyPressed(SR_KEY_S)) {
				KeyPressedEvent e(SR_KEY_S, 1);
				OnEvent(e);
			}
			if (Input::IsKeyPressed(SR_KEY_D)) {
				KeyPressedEvent e(SR_KEY_D, 1);
				OnEvent(e);
			}
		}
		if (Input::IsMouseBtnPressed(SR_MOUSE_BUTTON_RIGHT)) {
			float xpos = Input::GetMousePosition().first;
			float ypos = Input::GetMousePosition().second;
			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;

			lastX = xpos;
			lastY = ypos;
			MouseMovedEvent e{ xoffset ,yoffset };
			OnEvent(e);
		}
			lastX = Input::GetMousePosition().first;
			lastY = Input::GetMousePosition().second;
		
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(EditorCamera::OnMouseScroll));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(EditorCamera::OnKeyPressed));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(EditorCamera::OnMouseMoved));
	}


}
