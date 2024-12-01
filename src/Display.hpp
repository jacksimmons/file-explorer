#pragma once

#include "imgui.h"
#include <tuple>
#include <string>
#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace fs = std::filesystem;

enum class DirectoryAccess
{
	Granted,
	Denied,
	NotFound
};

// Colours
constexpr ImVec4 ACCESS_COLOUR =
ImVec4(255, 255, 255, 1);
constexpr ImVec4 NO_ACCESS_COLOUR =
ImVec4(255, 255, 0, 1);
constexpr ImVec4 NO_EXIST_COLOUR =
ImVec4(255, 0, 0, 1);
constexpr ImVec4 NO_IDEA_COLOUR =
ImVec4(255, 0, 255, 1);

ImVec4 GetTextColourFromAccess(DirectoryAccess access);
std::tuple<std::string, float> GetFileSizeDenom(uintmax_t fileSizeBytes);
// From: https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte
#ifdef _WIN32
std::string utf8_encode(const std::wstring &wstring);
#endif

class Display {
protected:
	bool m_active;
public:
	Display() { m_active = true; }
	void setActive(bool active) { m_active = active; }
	bool active() { return m_active; }
};