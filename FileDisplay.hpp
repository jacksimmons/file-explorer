#pragma once

#include "imgui.h"
#include "magic_enum.hpp"

#include <vector>
#include <tuple>
#include <string>
#include <filesystem>

#include "Display.hpp"

#define ACCESS_DENIED "<Access Denied>"

constexpr ImVec4 DIR_COLOUR =
ImVec4(0, 255, 255, 1);
constexpr ImVec4 FILE_COLOUR =
ImVec4(255, 255, 255, 1);

const ImGuiTableFlags TABLE_FLAGS =
ImGuiTableFlags_Resizable |
ImGuiTableFlags_Reorderable |
ImGuiTableFlags_Sortable;

class FileDisplay : public Display {
private:
	std::vector<std::tuple<std::string, DirectoryAccess>> m_currentDirectories;
	std::vector<std::tuple<std::string, uintmax_t, DirectoryAccess>> m_currentFiles;
	fs::path m_currentPath;
	std::string m_selectedFile;
public:
	FileDisplay();
	bool Draw(bool update);
	void UpdateFiles();
	
	fs::path currentPath();
	std::string selectedFile();
};