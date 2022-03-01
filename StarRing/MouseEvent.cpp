#include "pch.h"
#include "MouseEvent.h"

string StarRing::MouseMovedEvent::ToString() const
{
	stringstream ss;
	ss << "MouseMoveEvent" << m_MouseX << "," << m_MouseY;
	return ss.str();
}

string StarRing::MouseScrolledEvent::ToString() const
{
	stringstream ss;
	ss << "MouseScrolledEvent" << m_OffsetX << "," << m_OffsetY;
	return ss.str();
}

string StarRing::MouseButtonPressedEvent::ToString() const
{
	stringstream ss;
	ss << "MouseButtonPressedEvent" << m_Button;
	return ss.str();
}

string StarRing::MouseButtonReleasedEvent::ToString() const
{
	stringstream ss;
	ss << "MouseButtonReleasedEvent" << m_Button;
	return ss.str();
}
