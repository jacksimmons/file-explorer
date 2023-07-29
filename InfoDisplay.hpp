#pragma once

#include "imgui.h"

#include <filesystem>

#include "Display.hpp"
#include "FileDisplay.hpp"

namespace fs = std::filesystem;

class InfoDisplay : public Display {
private:
	std::string m_name;
	fs::file_type m_type;
	fs::perms m_perms;
	fs::space_info m_space_info;
	uintmax_t m_size;
	//fs::file_time_type m_last_modified;
public:
	InfoDisplay();
	void init();
	void Draw(bool update, fs::path path="", std::string file="");
};