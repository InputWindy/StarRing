#pragma once
#include "Event.h"
namespace StarRing {
	//�����¼�����ά��һ��������
	class SR_API KeyEvent:public Event
	{
	public:
		
		inline int GetKeyCode() { return m_KeyCode; };
		EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput);

	protected:
		KeyEvent(unsigned int key_code) {
			m_KeyCode = key_code;
		};
		unsigned int m_KeyCode;
	};

	//�����¼�����ά��һ������ʱ��
	class SR_API KeyPressedEvent :public KeyEvent
	{
	public:
		KeyPressedEvent(unsigned int key_code, int key_repeat_count):
			KeyEvent(key_code)
		{
			m_RepeatCount = key_repeat_count;
		};
		inline int GetRepeatCount() {
			return m_RepeatCount;
		}
		string ToString()const override;

		EVENT_CLASS_TYPE(KeyPressed);
	private:
		int m_RepeatCount;
	};

	//�ͷŰ������¼���������
	class SR_API KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(unsigned int key_code):
			KeyEvent(key_code)
		{
			
		};

		string ToString()const;

		EVENT_CLASS_TYPE(KeyReleased);

	};

	//�ͷŰ������¼���������
	class SR_API KeyTypedEvent :public KeyEvent
	{
	public:
		KeyTypedEvent(unsigned int key_code) :
			KeyEvent(key_code)
		{

		};

		string ToString()const;

		EVENT_CLASS_TYPE(KeyTyped);

	};
}