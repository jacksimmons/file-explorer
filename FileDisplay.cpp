#include <GLFW/glfw3.h>
#include <filesystem>
#include <vector>
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include "Windows.h"
#endif

#include "FileDisplay.hpp"

FileDisplay::FileDisplay()
{
	m_currentPath = fs::current_path().root_path();
	m_selectedFile = "";
}

// Returns whether a post-update is needed
bool FileDisplay::Draw(bool update)
{
	if (!ImGui::Begin("FileDisplay", nullptr, DISP_FLAGS))
	{
		ImGui::End();
		return false;
	}
	
	if (ImGui::Button("../"))
	{
		m_currentPath /= "../";
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

		ImGui::TableNextColumn(); ImGui::Text(name.c_str());
		ImGui::TableNextColumn(); ImGui::TextColored(GetTextColourFromAccess(access), "File");
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

	std::cout << m_currentPath << std::endl;

	for (const auto &entry : fs::directory_iterator(m_currentPath))
	{
		fs::path path = entry.path();
		bool is_dir = fs::is_directory(path);

		std::string fname_str;
#ifdef _WIN32
		fname_str = utf8_encode(path.filename());
#elif defined(linux)
		fname_str = path.filename().string();
#endif

		bool error = false;
		if (!is_dir)
		{
			uintmax_t file_size = 0;
			try
			{
				file_size = fs::file_size(path);
			}
			catch (fs::filesystem_error)
			{
				error = true;
			}

			if (!error)
			{
				m_currentFiles.push_back(std::make_tuple(fname_str, file_size, DirectoryAccess_Granted));
			}
			else
			{
				m_currentFiles.push_back(std::make_tuple(fname_str, file_size, DirectoryAccess_DirNotFound));
			}
		}
		else
		{
			m_currentDirectories.push_back(std::make_tuple(fname_str, DirectoryAccess_Granted));
		}
	}
}

ImVec4 FileDisplay::GetTextColourFromAccess(DirectoryAccess access)
{
	ImVec4 colour;
	switch (access)
	{
	case DirectoryAccess_Granted:
		colour = ACCESS_COLOUR;
		break;
	case DirectoryAccess_Denied:
		colour = NO_ACCESS_COLOUR;
		break;
	case DirectoryAccess_DirNotFound:
		colour = NO_EXIST_COLOUR;
		break;
	default:
		colour = NO_IDEA_COLOUR;
		break;
	}
	return colour;
}

std::tuple<std::string, float> FileDisplay::GetFileSizeDenom(uintmax_t fileSizeBytes)
{
	std::string denoms[] = { "B", "KB", "MB", "GB", "TB" };
	int denomIndex = 0;
	float fileSizeDenom = (float)fileSizeBytes;
	while (fileSizeDenom > 1000 && denomIndex < 4)
	{
		fileSizeDenom /= 1000;
		denomIndex++;
	}

	std::string denom = denoms[denomIndex];
	return std::make_tuple(denom, fileSizeDenom);
}

// From: https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte
std::string FileDisplay::utf8_encode(const std::wstring &wstring)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), NULL, 0, NULL, NULL);
	std::string string(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), &string[0], size, NULL, NULL);
	return string;
}

fs::path FileDisplay::currentPath()
{
	return m_currentPath;
}

std::string FileDisplay::selectedFile()
{
	return m_selectedFile;
}