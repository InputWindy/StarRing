#pragma once
#include "Input.h"
namespace StarRing {
	class SR_API WindowsInput:public Input
	{
	public:

	protected:
		bool IsKeyPressedImpl(unsigned int key_code)override;
		virtual bool IsMouseBtnPressedImpl(unsigned int btn)override;
		virtual std::pair<float, float> GetMousePositionImpl()override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	private:
		
	};
}

