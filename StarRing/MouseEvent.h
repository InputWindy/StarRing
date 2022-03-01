#pragma once
#include "Event.h"
namespace StarRing {
	class SR_API MouseMovedEvent:public Event
	{
	public:
		MouseMovedEvent(float x, float y) {
			m_MouseX = x;
			m_MouseY = y;
		};

		inline float GetMouseX()const { return m_MouseX; };
		inline float GetMouseY()const { return m_MouseY; };

		string ToString()const override;

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
	private:
		float m_MouseX, m_MouseY;
	};

	class SR_API MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(float x, float y) {
			m_OffsetX = x;
			m_OffsetY = y;
		};

		inline float GetOffsetX()const { return m_OffsetX; };
		inline float GetOffsetY()const { return m_OffsetY; };

		string ToString()const override;

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
	private:
		float m_OffsetX, m_OffsetY;
	};

	class SR_API MouseButtonEvent :public Event
	{
	public:
		
		inline int GetMouseButton()const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
	protected:
		MouseButtonEvent(int btn) {
			m_Button = btn;
		};
		int m_Button;
	};

	class SR_API MouseButtonPressedEvent :public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int btn) :MouseButtonEvent(btn) {};

		string ToString()const override;

		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class SR_API MouseButtonReleasedEvent :public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int btn) :MouseButtonEvent(btn) {};

		string ToString()const override;

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};

}