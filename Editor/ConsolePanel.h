#pragma once

#include <StarRing.h>
namespace StarRing {

	class ConsolePanel
	{
	public:
		ConsolePanel();

		void SetScene(const SceneRef& scene);

		void OnUpdate();
		void OnImGuiRender();
	private:
	};

}

