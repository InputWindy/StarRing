#pragma once
#include "Core.h"
#include "OrthographicCamera.h"
#include "KeyEvent.h"
#include "ApplicationEvent.h"
#include "MouseEvent.h"
namespace StarRing {
	class SR_API OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = true);

		void OnUpdate();
		void OnEvent(Event& e);

		float GetZoomLevel()const { return m_ZoomLevel; }
		void  SetZoomLevel(float zoomlevel) {  m_ZoomLevel = zoomlevel; }

		const OrthographicCamera& GetCamera()const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 0.0f;

		OrthographicCamera m_Camera;
		
		bool m_Rotation;

		vec3 m_CameraPosition = {0.0f,0.0f,0.0f};
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 1.0f;
	};
}

