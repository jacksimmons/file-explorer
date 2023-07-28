#include <GLFW/glfw3.h>
#include <filesystem>
#include <string>
#include <vector>
#include <iostream>

#include <cerrno>

#include "Window.hpp"

const ImGuiWindowFlags FILE_DISP_FLAGS =
ImGuiWindowFlags_AlwaysVerticalScrollbar |
ImGuiWindowFlags_AlwaysUseWindowPadding;

const ImGuiWindowFlags FILE_STATS_FLAGS =
ImGuiWindowFlags_None;

constexpr ImVec4 NO_PERM_COLOUR =
ImVec4(255, 255, 0, 1);
constexpr ImVec4 NO_EXIST_COLOUR =
ImVec4(255, 0, 0, 1);

namespace fs = std::filesystem;
fs::path currentPath = fs::current_path().root_path();
std::vector<std::string> directories;
std::vector<std::string> fineFiles;
std::vector<std::string> inaccessibleFiles;
std::vector<std::string> nonExistentFiles;

void DrawFileDisplay(bool update)
{
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

	for (std::string directory : directories)
	{
		if (ImGui::Button(directory.c_str()))
		{
			currentPath /= directory;
			UpdateFiles();
			ImGui::End();
			return;
		}
	}

	for (std::string file : fineFiles)
	{
		ImGui::Text(file.c_str());
	}

	for (std::string file : inaccessibleFiles)
	{
		ImGui::TextColored(NO_PERM_COLOUR, file.c_str());
	}

	for (std::string file : inaccessibleFiles)
	{
		ImGui::TextColored(NO_EXIST_COLOUR, file.c_str());
	}

	// Updates the directory every second in case new files are created.
	// Faster refresh rate causes lag as filesystem is slow.
	if (update)
	{
		UpdateFiles();
	}

	ImGui::End();
}

void UpdateFiles()
{
	directories.clear();
	fineFiles.clear();
	inaccessibleFiles.clear();
	nonExistentFiles.clear();
	for (const auto &entry : fs::directory_iterator(currentPath))
	{
		struct stat s;
		fs::path path = entry.path();
		std::string pathSTR = path.string();
		std::string fNameSTR = path.filename().string();

		// mbs = Multi-byte string
		const char *pathMBS = pathSTR.c_str();

		if (stat(pathMBS, &s) == 0)
		{
			if (s.st_mode & S_IFDIR)
			{
				directories.push_back(fNameSTR);
			}
			else if (s.st_mode & S_IFREG)
			{
				fineFiles.push_back(fNameSTR);
			}
		}
		else
		{
			if (errno == EACCES)
			{
				inaccessibleFiles.push_back(fNameSTR);
			}
			else if (errno == ENOENT)
			{
				nonExistentFiles.push_back(fNameSTR);
			}
			else
			{
				std::cout << pathMBS << std::endl;
			}
		}
	}
}
