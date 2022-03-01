#include "pch.h"
#include "ApplicationEvent.h"

string StarRing::WindowResizeEvent::ToString() const
{
	stringstream ss;
	ss << "WindowResizeEvent:" << "width(" << m_Width << "),height(" << m_Height << ")";
	return ss.str();
}
