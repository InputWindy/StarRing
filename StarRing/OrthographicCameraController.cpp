#include "pch.h"
#include "OrthographicCameraController.h"
#include "Input.h"
#include "DeltaTime.h"
namespace StarRing {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio,bool rotation)
		:m_AspectRatio(aspectRatio),m_Camera(-m_AspectRatio*m_ZoomLevel,m_AspectRatio*m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel),
		m_Rotation(rotation)
	{

	}
	void OrthographicCameraController::OnUpdate()
	{
		double deltaTime = DeltaTime::GetSeconds();
		if (Input::IsKeyPressed(SR_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(SR_KEY_D)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * deltaTime;
		}

		if (Input::IsKeyPressed(SR_KEY_W)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(SR_KEY_S)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * deltaTime;
		}

		if (m_Rotation) {
			if (Input::IsKeyPressed(SR_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * deltaTime;
			}
			else if (Input::IsKeyPressed(SR_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * deltaTime;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}
		m_Camera.SetPosition(m_CameraPosition);
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OrthographicCameraController::OnWindowResized));
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetOffsetY();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return true;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth()/(float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return true;
	}
}