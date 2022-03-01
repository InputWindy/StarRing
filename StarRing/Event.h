#pragma once
#include "pch.h"
#include "Core.h"

using namespace std;

namespace StarRing {
	//EventType
	enum
	{
		ET_None = 0,
		WindowClose,WindowResize,WindowFocus,WindowLostFocus,WindowMoved,
		AppTick,AppUpdate,AppRender,
		KeyPressed,KeyReleased,KeyTyped,
		MouseButtonPressed,MouseButtonReleased,MouseMoved,MouseScrolled
	};

	//EventCategory
	enum 
	{
		EC_None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyBoard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

//Rtti
#define EVENT_CLASS_TYPE(type) \
static int GetStaticEventType(){return type;}\
virtual int GetEventType()const override{return GetStaticEventType();}\
virtual const char* GetName()const override{return #type;};

//Rtti
#define EVENT_CLASS_CATEGORY(category) \
virtual int GetCategoryFlags()const override{return category;};

	//维护基本事件信息：事件名称/事件类型/事件所属分类/判断事件所属分类
	class SR_API Event
	{
		friend class ImGuiLayer;
		friend class EventDispatcher;
	public:
		Event() {};
		virtual ~Event() {};

		virtual int GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual string ToString()const { return GetName(); };
		inline bool IsInCategory(int category) {
			return GetCategoryFlags() & category;
		}
		inline bool IsHandled() {
			return m_Handled;
		}
	protected:
		//事件状态：事件是否被处理了（初始化为否）
		bool m_Handled = false;
	};

	//事件分发者：维护一个任意类型的事件
	//实现一个API：Dispatch()--用于处理事件
	//EventHandler是一类形如:bool OnXXX()的函数
	class SR_API EventDispatcher
	{
		template<typename EventType>
		using EventHandler = function<bool(EventType&)>;
	public:
		EventDispatcher(Event& event) :m_Event(event) {}

		/// <typeparam name="EventType"></typeparam>
		/// <param name="传入处理EventType类型事件的EventHandler(函数)"></param>
		/// <returns>bool</returns>
		/// <summary>
		/// 该函数用于处理事件，传入一个处理EventType类型的函数指针EventHandler，
		/// 如果EventHandler和EventType相匹配，就执行处理操作，返回true，
		/// 否则不做处理，返回false
		/// </summary>
		template<typename EventType>
		bool Dispatch(EventHandler<EventType> EventHandler) {
			if (m_Event.GetEventType() == EventType::GetStaticEventType()) {
				m_Event.m_Handled = EventHandler(*(EventType*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline ostream& operator<<(ostream& os, const Event& eve) {
		return os << eve.ToString();
	}

}