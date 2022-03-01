#pragma once
#include "Core.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"
namespace StarRing {
	
	//这个类维护glfw的所有输入数据，相当于glfw输入接口的抽象层，
	//通过该类可以在任何地方实时读取输入数据
	class SR_API Input
	{
	public:
		inline static bool IsKeyPressed(unsigned int key_code) { return ms_InputInstance->IsKeyPressedImpl(key_code); };
		inline static bool IsMouseBtnPressed(unsigned int btn) { return ms_InputInstance->IsMouseBtnPressedImpl(btn); };
		inline static std::pair<float, float> GetMousePosition() { return ms_InputInstance->GetMousePositionImpl(); };
		inline static float GetMouseX() { return ms_InputInstance->GetMouseXImpl(); };
		inline static float GetMouseY() { return ms_InputInstance->GetMouseYImpl(); };
	protected:
		virtual bool IsKeyPressedImpl(unsigned int key_code) = 0;
		virtual bool IsMouseBtnPressedImpl(unsigned int btn) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		static Input* ms_InputInstance;
	};
}