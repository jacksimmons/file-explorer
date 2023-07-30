#include "imgui.h"

class MenuBar {
public:
	bool m_fileExplorerSelected = false;
	bool m_fileInfoSelected = false;
	bool m_fileViewerSelected = false;

	void Draw();
};
