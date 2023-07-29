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

	if (!ImGui::Begin("FileDisplay", &m_active))
	{
		ImGui::End();
		return false;
	}
	
	if (ImGui::Button("../"))
	{
		m_currentPath = m_currentPath.parent_path();
		ImGui::End();
		return true;
	}

	if (!ImGui::BeginTable("FileDisplayTable", 3))
	{
		ImGui::EndTable();
		return false;
	}

	ImGui::TableNextColumn(); ImGui::Text("Name");
	ImGui::TableNextColumn(); ImGui::Text("File/Directory");
	ImGui::TableNextColumn(); ImGui::Text("File Size (Bytes)");

	for (auto &directory : m_currentDirectories)
	{
		std::string name = std::get<0>(directory);
		DirectoryAccess access = std::get<1>(directory);

		ImGui::TableNextColumn(); bool button = ImGui::Button((name + "##DIR").c_str());
		ImGui::TableNextColumn(); ImGui::TextColored(GetTextColourFromAccess(access), "Directory");
		ImGui::TableNextColumn();

		if (button)
		{
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

		ImGui::TableNextColumn(); ImGui::TextColored(GetTextColourFromAccess(access), name.c_str());
		ImGui::TableNextColumn(); ImGui::Text("File");
		ImGui::TableNextColumn(); ImGui::Text((std::to_string(fileSizeDenomValue) + " " + fileSizeDenom).c_str());
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
		m_currentFiles.push_back(std::make_tuple("<Access Denied>", 0, DirectoryAccess_Denied));
	}

	for (const auto &entry : iter)
	{
		fs::path path = entry.path();

		if (!fs::exists(path))
			std::cout << "hi" << std::endl;

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
				access = DirectoryAccess_Granted;
			}
			catch (fs::filesystem_error)
			{
				access = DirectoryAccess_DirNotFound;
			}

			m_currentFiles.push_back(std::make_tuple(fname_str, file_size, DirectoryAccess_Granted));
		}
		else if (fs::is_directory(path))
		{
			m_currentDirectories.push_back(std::make_tuple(fname_str, DirectoryAccess_Granted));
		}
		else
		{
			m_currentDirectories.push_back(std::make_tuple(fname_str, DirectoryAccess_Denied));
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