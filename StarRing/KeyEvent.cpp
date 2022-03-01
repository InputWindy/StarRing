#include "pch.h"
#include "KeyEvent.h"

string StarRing::KeyPressedEvent::ToString() const
{
	stringstream ss;
	ss << "KeyPressedEvent" << m_KeyCode << "(" << m_RepeatCount << "repeats)";
	return ss.str();
}

string StarRing::KeyReleasedEvent::ToString() const
{
	stringstream ss;
	ss << "KeyPressedEvent" << m_KeyCode;
	return ss.str();
}

string StarRing::KeyTypedEvent::ToString() const
{
	stringstream ss;
	ss << "KeyTypedEvent" << m_KeyCode;
	return ss.str();
}
