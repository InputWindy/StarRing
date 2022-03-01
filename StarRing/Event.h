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

	//ά�������¼���Ϣ���¼�����/�¼�����/�¼���������/�ж��¼���������
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
		//�¼�״̬���¼��Ƿ񱻴����ˣ���ʼ��Ϊ��
		bool m_Handled = false;
	};

	//�¼��ַ��ߣ�ά��һ���������͵��¼�
	//ʵ��һ��API��Dispatch()--���ڴ����¼�
	//EventHandler��һ������:bool OnXXX()�ĺ���
	class SR_API EventDispatcher
	{
		template<typename EventType>
		using EventHandler = function<bool(EventType&)>;
	public:
		EventDispatcher(Event& event) :m_Event(event) {}

		/// <typeparam name="EventType"></typeparam>
		/// <param name="���봦��EventType�����¼���EventHandler(����)"></param>
		/// <returns>bool</returns>
		/// <summary>
		/// �ú������ڴ����¼�������һ������EventType���͵ĺ���ָ��EventHandler��
		/// ���EventHandler��EventType��ƥ�䣬��ִ�д������������true��
		/// ��������������false
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