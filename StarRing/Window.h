#pragma once
#include "pch.h"
#include "Core.h"
#include "Event.h"

namespace StarRing {
	//窗口信息
	struct WindowProps
	{
		string Tittle;
		unsigned int Width, Height;
		WindowProps(const string& tittle = "StarRing Engine",
			unsigned int width = 1280,
			unsigned int height = 720):
			Tittle(tittle)
		{
			Width  = width ;
			Height = height;
		}
	};

	//窗口基类：声明窗口基本接口
	class SR_API Window
	{
	public:
		using EventCallBackFunc = function<void(Event&)>;

		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth()const = 0;
		virtual unsigned int GetHeight()const = 0;

		//窗口信息接口
		virtual void SetEventCallBackFunc(const EventCallBackFunc& event_callback_func) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync()const = 0;

		virtual void* GetNativeWindow()const = 0;

		static Window* Create(const WindowProps& win_props = WindowProps());
	};

}