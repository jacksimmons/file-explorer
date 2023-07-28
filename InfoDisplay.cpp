#include <iostream>

#include "magic_enum.hpp"

#include "ImGuiInclude.hpp"
#include "InfoDisplay.hpp"

InfoDisplay::InfoDisplay()
{
	init();
}

void InfoDisplay::init()
{
	m_name = "(Unselected)";
	m_type = fs::file_type::not_found;
	m_perms = fs::perms::unknown;
	m_space_info = fs::space_info();
	m_size = 0;
}

void InfoDisplay::Draw(bool update, fs::path path, std::string file)
{
	if (!ImGui::Begin("InfoDisplay", nullptr, DISP_FLAGS))
	{
		ImGui::End();
		return;
	}

	if (!ImGui::BeginTable("InfoDisplayTable", 2))
	{
		ImGui::EndTable();
		return;
	}

	if (update)
	{
		if (file != "")
		{
			path /= file;
		}

		if (!fs::exists(path))
		{
			init();
		}
		else
		{
			m_name = path.filename().string();
			fs::file_status status = fs::status(path);
			m_type = status.type();
			m_perms = status.permissions();
			m_space_info = fs::space(path);
			m_size = fs::file_size(path);
		}
	}

	ImGui::TableNextColumn(); ImGui::Text("Name");
	ImGui::TableNextColumn(); ImGui::Text(m_name.c_str());

	const char *type = magic_enum::enum_name(m_type).data();
	ImGui::TableNextColumn(); ImGui::Text("Type");
	ImGui::TableNextColumn(); ImGui::Text(type);

	ImGui::TableNextColumn(); ImGui::Text("Size");
	ImGui::TableNextColumn(); ImGui::Text(std::to_string(m_size).c_str());

	std::string owner_read = (fs::perms::none == (m_perms & fs::perms::owner_read) ? "-" : "r");
	std::string owner_write = (fs::perms::none == (m_perms & fs::perms::owner_write) ? "-" : "w");
	std::string owner_exec = (fs::perms::none == (m_perms & fs::perms::owner_exec) ? "-" : "x");
	ImGui::TableNextColumn(); ImGui::Text("Owner Permissions");
	ImGui::TableNextColumn(); ImGui::Text((owner_read + owner_write + owner_exec).c_str());

	std::string group_read = (fs::perms::none == (m_perms & fs::perms::group_read) ? "-" : "r");
	std::string group_write = (fs::perms::none == (m_perms & fs::perms::group_write) ? "-" : "w");
	std::string group_exec = (fs::perms::none == (m_perms & fs::perms::group_exec) ? "-" : "x");
	ImGui::TableNextColumn(); ImGui::Text("Group Permissions");
	ImGui::TableNextColumn(); ImGui::Text((group_read + group_write + group_exec).c_str());

	std::string others_read = (fs::perms::none == (m_perms & fs::perms::others_read) ? "-" : "r");
	std::string others_write = (fs::perms::none == (m_perms & fs::perms::others_write) ? "-" : "w");
	std::string others_exec = (fs::perms::none == (m_perms & fs::perms::others_exec) ? "-" : "x");
	ImGui::TableNextColumn(); ImGui::Text("Other Permissions");
	ImGui::TableNextColumn(); ImGui::Text((others_read + others_write + others_exec).c_str());

	ImGui::EndTable();

	ImGui::End();
}