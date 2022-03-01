#include "pch.h"
#include "Event.h"
#include "ApplicationEvent.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
namespace StarRing {
	template<typename EventType>
	inline bool EventDispatcher::Dispatch(EventHandler<EventType> EventHandler)
	{
		if (m_Event.GetEventType() == EventType::GetStaticType()) {
			m_Event.m_Handled = EventHandler(*(EventType*)&m_Event);
			return true;
		}
		return false;
	}
}
