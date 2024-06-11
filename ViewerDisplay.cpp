#include <fstream>
#include <iostream>
#include "ViewerDisplay.hpp"

ViewerDisplay::ViewerDisplay()
{
	m_currentFilePath = "";
}

void ViewerDisplay::Draw(bool update, fs::path path, std::string file)
{
	if (!m_active)
		return;

	if (!ImGui::Begin("File Viewer", &m_active))
	{
		ImGui::End();
		return;
	}

	if (update)
	{
		UpdateFile(path, file);
	}

	if (m_currentFilePath != "")
	{
		ImGui::TextColored(TEXT_COLOUR, utf8_encode(m_currentFilePath).c_str());
	}

	for (int i = 0; i < m_lines.size(); i++)
	{
		ImGui::TextColored(LINE_NUM_COLOUR, (std::to_string(i) + " ").c_str());
		ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOUR, m_lines[i].c_str());
	}

	ImGui::End();
}

void ViewerDisplay::UpdateFile(fs::path path, std::string file)
{
	if (path == "" || file == "")
		return;

	fs::path filePath = path / file;
	m_lines.clear();
	std::fstream f;
	f.open(filePath.string(), std::ios::in);

	int line_no = 0;
	if (f.is_open())
	{
		std::string current_line;
		while (std::getline(f, current_line))
		{
			m_lines.push_back(current_line);
		}
		m_currentFilePath = filePath;
		f.close();
	}
	else
	{
		ImGui::TextColored(TEXT_COLOUR, utf8_encode(filePath).c_str());
		ImGui::TextColored(NO_ACCESS_COLOUR, "Insufficient permissions to read file.");
		m_currentFilePath = "";
	}
}