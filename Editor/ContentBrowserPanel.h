#pragma once

#include <filesystem>
#include <StarRing.h>
namespace StarRing {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnUpdate();
		void OnImGuiRender();

		void SetScene(const SceneRef& scene);
	private:
		std::filesystem::path m_CurrentDirectory;
		
		Texture2DRef m_DirectoryIcon;
		Texture2DRef m_FileIcon;
	};

}
