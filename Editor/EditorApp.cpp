#include <StarRing.h>
#include "EntryPoint.h"
#include "EditorLayer.h"
using namespace StarRing;
class EditorApp:public StarRing::Application
{
public:
	EditorApp() {
		PushLayer(new EditorLayer());
	};
	~EditorApp() {};

private:
	
};

StarRing::Application* StarRing::CreateApplication() {
	return new EditorApp();
}

