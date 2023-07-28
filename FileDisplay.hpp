#pragma once

#include <vector>
#include <tuple>
#include <string>
#include <filesystem>

#include "ImGuiInclude.hpp"

namespace fs = std::filesystem;

// Access-based colours
constexpr ImVec4 ACCESS_COLOUR =
ImVec4(255, 255, 255, 1);
constexpr ImVec4 NO_ACCESS_COLOUR =
ImVec4(255, 255, 0, 1);
constexpr ImVec4 NO_EXIST_COLOUR =
ImVec4(255, 0, 0, 1);
constexpr ImVec4 NO_IDEA_COLOUR =
ImVec4(255, 0, 255, 1);

enum DirectoryAccess
{
	DirectoryAccess_Granted,
	DirectoryAccess_Denied,
	DirectoryAccess_DirNotFound
};

class FileDisplay {
private:
	std::vector<std::tuple<std::string, DirectoryAccess>> m_currentDirectories;
	std::vector<std::tuple<std::string, uintmax_t, DirectoryAccess>> m_currentFiles;
	fs::path m_currentPath;
	std::string m_selectedFile;
public:
	FileDisplay();
	bool Draw(bool update);
	void UpdateFiles();
	ImVec4 GetTextColourFromAccess(DirectoryAccess access);
	std::tuple<std::string, float> GetFileSizeDenom(uintmax_t fileSizeBytes);
	
	fs::path currentPath();
	std::string selectedFile();

#ifdef _WIN32
	std::string utf8_encode(const std::wstring &wstring);
#endif
};