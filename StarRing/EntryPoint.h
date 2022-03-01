#pragma once
#ifdef SR_PLATFORM_WINDOWS
#include "Application.h"
//使用客户端实现的接口来创建一个App（在exe里）
extern StarRing::Application* StarRing::CreateApplication();
int main(int argc,char** argv) {
	
	StarRing::Log::Init();
	SR_CORE_INFO("星环引擎启动！     Σ( ° △ °|||)︴",5);
	auto app = StarRing::CreateApplication();
	app->OnStart();
	app->Run();
	delete app;
}

#endif // SR_PLATFORM_WINDOWS