#include "magic_enum.hpp"

#include <iostream>

#include "FileInfoDisplay.hpp"

FileInfoDisplay::FileInfoDisplay()
{
	Init();
}

void FileInfoDisplay::Init()
{
	m_name = "(Unselected)";
	m_type = fs::file_type::unknown;
	m_perms = fs::perms::unknown;
	m_spaceInfo = fs::space_info();
	m_spaceInfo.capacity = 0;
	m_spaceInfo.available = 0;
	m_spaceInfo.free = 0;
	m_size = 0;

	m_sizeKnown = true;
	m_spaceInfoKnown = true;
}

void FileInfoDisplay::Draw(bool update, fs::path path, std::string file)
{
	if (!m_active)
		return;

	if (!ImGui::Begin("File Info", &m_active))
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
			Init();
		}
		else
		{
			try { m_name = path.filename().string(); }
			catch (fs::filesystem_error) { m_name = "<Access Denied>"; }

			try { 
				fs::file_status status;
				status = fs::status(path);
				m_type = status.type();
				m_perms = status.permissions();
			}
			catch (fs::filesystem_error) {
				m_type = fs::file_type::unknown;
				m_perms = fs::perms::unknown;
			}

			try { 
				m_spaceInfo = fs::space(path);
				m_spaceInfoKnown = true;
			}
			catch (fs::filesystem_error) { m_spaceInfoKnown = false; }

			try { 
				m_size = fs::file_size(path);
				m_sizeKnown = true;
			}
			catch (fs::filesystem_error) { m_sizeKnown = false; }
		}
	}

	ImGui::TableNextColumn(); ImGui::Text("Name");
	ImGui::TableNextColumn(); ImGui::Text(m_name.c_str());

	const char *type = magic_enum::enum_name(m_type).data();
	ImGui::TableNextColumn(); ImGui::Text("Type");
	ImGui::TableNextColumn(); ImGui::Text(type);

	if (!m_sizeKnown)
	{
		ImGui::TableNextColumn(); ImGui::Text("Size");
		ImGui::TableNextColumn(); ImGui::Text("Unknown");
	}
	else
	{
		std::tuple<std::string, float> denomTuple;
		std::string denom;
		float denomValue;

		denomTuple = GetFileSizeDenom(m_size);
		denom = std::get<0>(denomTuple);
		denomValue = std::get<1>(denomTuple);
		ImGui::TableNextColumn(); ImGui::Text("Size");
		ImGui::TableNextColumn(); ImGui::Text((std::to_string(denomValue) + denom).c_str());
	}

	std::string owner_read = (fs::perms::none == (m_perms & fs::perms::owner_read) ? "-" : "r");
	std::string owner_write = (fs::perms::none == (m_perms & fs::perms::owner_write) ? "-" : "w");
	std::string owner_exec = (fs::perms::none == (m_perms & fs::perms::owner_exec) ? "-" : "x");
	ImGui::TableNextColumn(); ImGui::Text("Owner Permissions");
	ImGui::TableNextColumn();
	if (m_perms != fs::perms::unknown)
		ImGui::Text((owner_read + owner_write + owner_exec).c_str());
	else
		ImGui::Text("Unknown");

	std::string group_read = (fs::perms::none == (m_perms & fs::perms::group_read) ? "-" : "r");
	std::string group_write = (fs::perms::none == (m_perms & fs::perms::group_write) ? "-" : "w");
	std::string group_exec = (fs::perms::none == (m_perms & fs::perms::group_exec) ? "-" : "x");
	ImGui::TableNextColumn(); ImGui::Text("Group Permissions");
	ImGui::TableNextColumn();
	if (m_perms != fs::perms::unknown)
		ImGui::Text((group_read + group_write + group_exec).c_str());
	else
		ImGui::Text("Unknown");

	std::string others_read = (fs::perms::none == (m_perms & fs::perms::others_read) ? "-" : "r");
	std::string others_write = (fs::perms::none == (m_perms & fs::perms::others_write) ? "-" : "w");
	std::string others_exec = (fs::perms::none == (m_perms & fs::perms::others_exec) ? "-" : "x");
	ImGui::TableNextColumn(); ImGui::Text("Other Permissions");
	ImGui::TableNextColumn();
	if (m_perms != fs::perms::unknown)
		ImGui::Text((others_read + others_write + others_exec).c_str());
	else
		ImGui::Text("Unknown");

	if (!m_spaceInfoKnown)
	{
		ImGui::TableNextColumn(); ImGui::Text("Disk: Capacity");
		ImGui::TableNextColumn(); ImGui::Text("Unknown");

		ImGui::TableNextColumn(); ImGui::Text("Disk: Available");
		ImGui::TableNextColumn(); ImGui::Text("Unknown");

		ImGui::TableNextColumn(); ImGui::Text("Disk: Free");
		ImGui::TableNextColumn(); ImGui::Text("Unknown");
	}
	else
	{
		std::tuple<std::string, float> denomTuple = GetFileSizeDenom(m_spaceInfo.capacity);
		std::string denom = std::get<0>(denomTuple);
		float denomValue = std::get<1>(denomTuple);
		ImGui::TableNextColumn(); ImGui::Text("Disk: Capacity");
		ImGui::TableNextColumn(); ImGui::Text((std::to_string(denomValue) + denom).c_str());

		denomTuple = GetFileSizeDenom(m_spaceInfo.available);
		denom = std::get<0>(denomTuple);
		denomValue = std::get<1>(denomTuple);
		ImGui::TableNextColumn(); ImGui::Text("Disk: Available");
		ImGui::TableNextColumn(); ImGui::Text((std::to_string(denomValue) + denom).c_str());

		denomTuple = GetFileSizeDenom(m_spaceInfo.free);
		denom = std::get<0>(denomTuple);
		denomValue = std::get<1>(denomTuple);
		ImGui::TableNextColumn(); ImGui::Text("Disk: Free");
		ImGui::TableNextColumn(); ImGui::Text((std::to_string(denomValue) + denom).c_str());
	}

	ImGui::EndTable();

	ImGui::End();
}