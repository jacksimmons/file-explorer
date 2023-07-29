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
			ImGui::MenuItem("File Explorer", "", &m_fileDisplaySelected);
			ImGui::MenuItem("File Info", "Gives certain details about selected items in the File Explorer.", &m_infoDisplaySelected);
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
}
