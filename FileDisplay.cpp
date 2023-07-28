#include <GLFW/glfw3.h>
#include <filesystem>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <Windows.h>
#include <cerrno>

#include "FileDisplay.hpp"

namespace fs = std::filesystem;
fs::path currentPath = fs::current_path().root_path();

std::vector<std::tuple<std::string, DirectoryAccess>> directories;
std::vector<std::tuple<std::string, uintmax_t, DirectoryAccess>> files;

void DrawEnd()
{
	ImGui::EndTable();
	ImGui::End();
}

void DrawFileDisplay(bool update)
{
	static const ImGuiWindowFlags FILE_DISP_FLAGS =
		ImGuiWindowFlags_AlwaysVerticalScrollbar |
		ImGuiWindowFlags_AlwaysUseWindowPadding;

	static const ImGuiWindowFlags FILE_STATS_FLAGS =
		ImGuiWindowFlags_None;

	if (!ImGui::Begin("FileDisplay", nullptr, FILE_DISP_FLAGS))
	{
		ImGui::End();
		return;
	}
	
	if (ImGui::Button("../"))
	{
		currentPath /= "../";
		UpdateFiles();
		ImGui::End();
		return;
	}

	if (!ImGui::BeginTable("FileDisplayTable", 3))
	{
		ImGui::EndTable();
		return;
	}

	ImGui::TableNextColumn(); ImGui::Text("Name");
	ImGui::TableNextColumn(); ImGui::Text("File/Directory");
	ImGui::TableNextColumn(); ImGui::Text("File Size (Bytes)");

	for (auto &directory : directories)
	{
		std::string name = std::get<0>(directory);
		DirectoryAccess access = std::get<1>(directory);

		ImGui::TableNextColumn(); bool button = ImGui::Button((name + "##DIR").c_str());
		ImGui::TableNextColumn(); ImGui::TextColored(GetTextColourFromAccess(access), "Directory");
		ImGui::TableNextColumn();

		if (button)
		{
			currentPath /= name;
			DrawEnd();
			UpdateFiles();
			return;
		}
	}

	for (auto &file : files)
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

	DrawEnd();
}

void UpdateFiles()
{
	directories.clear();
	files.clear();

	for (const auto &entry : fs::directory_iterator(currentPath))
	{
		fs::path path = entry.path();
		bool is_dir = fs::is_directory(path);
		std::string fname_str = utf8_encode(path.filename());

		int err_state = 0;
		if (!is_dir)
		{
			uintmax_t file_size = 0;
			try
			{
				file_size = fs::file_size(path);
			}
			catch (fs::filesystem_error)
			{
				err_state = ENOENT;
			}

			if (err_state == 0)
			{
				files.push_back(std::make_tuple(fname_str, file_size, DirectoryAccess_Granted));
			}
			else
			{
				files.push_back(std::make_tuple(fname_str, file_size, DirectoryAccess_DirNotFound));
			}
		}
		else
		{
			directories.push_back(std::make_tuple(fname_str, DirectoryAccess_Granted));
		}
	}
}

ImVec4 GetTextColourFromAccess(DirectoryAccess access)
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

std::tuple<std::string, float> GetFileSizeDenom(uintmax_t fileSizeBytes)
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
std::string utf8_encode(const std::wstring &wstring)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), NULL, 0, NULL, NULL);
	std::string string(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), &string[0], size, NULL, NULL);
	return string;
}