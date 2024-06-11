#include <filesystem>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "FileDisplay.hpp"

FileDisplay::FileDisplay()
{
	m_currentPath = fs::current_path().root_path();
	m_selectedFile = "";
}

// Returns whether a post-update is needed
bool FileDisplay::Draw(bool update)
{
	if (!m_active)
		return false;

	if (!ImGui::Begin("File Explorer", &m_active))
	{
		ImGui::End();
		return false;
	}

	ImGui::TextColored(FILE_COLOUR, (utf8_encode(m_currentPath.c_str()) + "\\" + m_selectedFile).c_str());

	if (!ImGui::BeginTable("FileDisplayTable", 3, TABLE_FLAGS))
	{
		ImGui::EndTable();
		return false;
	}

	ImGui::TableSetupColumn("Type");
	ImGui::TableSetupColumn("Name");
	ImGui::TableSetupColumn("File Size");
	ImGui::TableHeadersRow();

	ImGui::TableNextColumn();
	ImGui::TableNextColumn(); bool go_back = ImGui::Button("../");
	ImGui::TableNextColumn();

	if (go_back)
	{
		m_selectedFile = "";
		m_currentPath = m_currentPath.parent_path();
		ImGui::EndTable();
		ImGui::End();
		return true;
	}

	for (auto &directory : m_currentDirectories)
	{
		std::string name = std::get<0>(directory);
		DirectoryAccess access = std::get<1>(directory);

		bool selected = false;
		ImGui::TableNextColumn(); ImGui::PushStyleColor(ImGuiCol_Text, DIR_COLOUR); ImGui::Text("Directory"); ImGui::PopStyleColor();
		ImGui::TableNextColumn(); ImGui::PushStyleColor(ImGuiCol_Text, GetTextColourFromAccess(access)); selected = ImGui::Selectable((name + "##DIR").c_str()); ImGui::PopStyleColor();
		ImGui::TableNextColumn();

		if (selected)
		{
			m_selectedFile = "";
			m_currentPath /= name;
			ImGui::EndTable();
			ImGui::End();
			return true;
		}
	}

	for (auto &file : m_currentFiles)
	{
		std::string name = std::get<0>(file);
		uintmax_t fileSize = std::get<1>(file);
		DirectoryAccess access = std::get<2>(file);

		std::tuple<std::string, float> fileSizeTuple = GetFileSizeDenom(fileSize);
		std::string fileSizeDenom = std::get<0>(fileSizeTuple);
		float fileSizeDenomValue = std::get<1>(fileSizeTuple);

		bool selected = false;
		ImGui::TableNextColumn();

		// All Access Denied entities are put into the files vector, we must handle them here.
		if (name == ACCESS_DENIED)
			ImGui::TextColored(NO_ACCESS_COLOUR, "???");
		else
			ImGui::TextColored(FILE_COLOUR, "File");

		ImGui::TableNextColumn(); ImGui::PushStyleColor(ImGuiCol_Text, GetTextColourFromAccess(access)); selected = ImGui::Selectable((name + "##FILE").c_str(), selected); ImGui::PopStyleColor();
		ImGui::TableNextColumn(); ImGui::Text((std::to_string(fileSizeDenomValue) + " " + fileSizeDenom).c_str());

		if (selected)
		{
			m_selectedFile = name;
		}
	}

	// Updates the directory every second in case new files are created.
	// Faster refresh rate causes lag as filesystem is slow.
	if (update)
	{
		UpdateFiles();
	}

	ImGui::EndTable();
	ImGui::End();

	return false;
}

void FileDisplay::UpdateFiles()
{
	m_currentDirectories.clear();
	m_currentFiles.clear();

	fs::directory_iterator iter;
	try
	{
		iter = fs::directory_iterator{ m_currentPath };
	}
	catch (fs::filesystem_error e)
	{
		m_currentFiles.push_back(std::make_tuple(ACCESS_DENIED, 0, DirectoryAccess::Denied));
	}

	for (const auto &entry : iter)
	{
		fs::path path = entry.path();

		std::string fname_str;
#ifdef _WIN32
		fname_str = utf8_encode(path.filename());
#elif defined(linux)
		fname_str = path.filename().string();
#endif

		bool error = false;
		if (fs::is_regular_file(path))
		{
			uintmax_t file_size = 0;
			DirectoryAccess access;
			try
			{
				file_size = fs::file_size(path);
				access = DirectoryAccess::Granted;
			}
			catch (fs::filesystem_error)
			{
				access = DirectoryAccess::NotFound;
			}

			m_currentFiles.push_back(std::make_tuple(fname_str, file_size, DirectoryAccess::Granted));
		}
		else if (fs::is_directory(path))
		{
			m_currentDirectories.push_back(std::make_tuple(fname_str, DirectoryAccess::Granted));
		}
		else
		{
			m_currentDirectories.push_back(std::make_tuple(fname_str, DirectoryAccess::Denied));
		}
	}
}

fs::path FileDisplay::currentPath()
{
	return m_currentPath;
}

std::string FileDisplay::selectedFile()
{
	return m_selectedFile;
}