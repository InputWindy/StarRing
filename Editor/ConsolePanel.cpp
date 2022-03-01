#include "ConsolePanel.h"
namespace StarRing {
	ConsolePanel::ConsolePanel()
	{
	}
	void ConsolePanel::SetScene(const SceneRef& scene)
	{
	}
	void ConsolePanel::OnUpdate()
	{
	}
	void ConsolePanel::OnImGuiRender()
	{
		ImGui::Begin("Console");
		ImGui::End();
	}
}