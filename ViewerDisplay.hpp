#include "imgui.h"

#include <string>

#include "Display.hpp"

constexpr ImVec4 LINE_NUM_COLOUR =
ImVec4(20, 20, 20, 1);
constexpr ImVec4 TEXT_COLOUR =
ImVec4(220, 220, 220, 1);

class ViewerDisplay : public Display {
private:
	std::vector<std::string> m_lines;
	fs::path m_currentFilePath;
public:
	ViewerDisplay();
	void Draw(bool update, fs::path path="", std::string file="");
	void UpdateFile(fs::path path="", std::string file="");
	fs::path currentFilePath() { return m_currentFilePath; };
};