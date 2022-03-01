#include "ContentBrowserPanel.h"
using namespace std;
namespace StarRing {

	extern const filesystem::path s_AssetsPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel():m_CurrentDirectory(s_AssetsPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}
	void ContentBrowserPanel::OnUpdate()
	{
	}
	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");
		if (m_CurrentDirectory != filesystem::path(s_AssetsPath)) {
			if (ImGui::Button("<-")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}
		static float padding = 20.0f;
		static float thumbnailSize = 70.0f;
		float cellsize = thumbnailSize + padding;

		float PanelWidth = ImGui::GetContentRegionAvail().x;
		int columCount = (int)(PanelWidth / cellsize);
		if (columCount < 1) {
			columCount = 1;
		}

		ImGui::Columns(columCount, 0, false);

		for (auto& p : filesystem::directory_iterator(m_CurrentDirectory)) {

			const auto& path = p.path();
			auto relativePath = filesystem::relative(path, m_CurrentDirectory);
			string relativePathString = relativePath.string();
			
			//ImGui::PushID(relativePathString.c_str());
			
			Texture2DRef icon = p.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0,0,0,0));
			ImGui::ImageButton((ImTextureID)icon->GetHandle(), { thumbnailSize ,thumbnailSize }, { 0,1 }, {1,0});
			
			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM",itemPath,(wcslen(itemPath)+1)*sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}
			
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (p.is_directory()) {
					m_CurrentDirectory /= path.filename();
				}
			}

			ImGui::TextWrapped(relativePathString.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::End();
	}
	void ContentBrowserPanel::SetScene(const SceneRef& scene)
	{
	}
}