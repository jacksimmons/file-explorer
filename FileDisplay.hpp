#pragma once

#include "imgui.h"

#include <vector>
#include <tuple>
#include <string>
#include <filesystem>

#include "Display.hpp"

namespace fs = std::filesystem;

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