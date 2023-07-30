#include "MenuBar.hpp"

void MenuBar::Draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("File Explorer", "", &m_fileExplorerSelected);
			ImGui::MenuItem("File Info", "Gives certain details about selected items in the File Explorer.", &m_fileInfoSelected);
			ImGui::MenuItem("File Viewer", "Reads files as text.", &m_fileViewerSelected);
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
}
