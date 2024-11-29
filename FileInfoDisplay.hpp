#pragma once

#include "imgui.h"

#include <filesystem>

#include "Display.hpp"
#include "FileInfoDisplay.hpp"

class FileInfoDisplay : public Display {
private:
	std::string m_name;
	fs::file_type m_type;
	fs::perms m_perms;
	fs::space_info m_spaceInfo;
	uintmax_t m_size;
	bool m_sizeKnown;
	bool m_spaceInfoKnown;
	//fs::file_time_type m_last_modified;
public:
	FileInfoDisplay();
	void Init();
	void Draw(bool update, fs::path path="", std::string file="");
};