#include "imgui.h"

class MenuBar {
public:
	bool m_fileDisplaySelected = false;
	bool m_infoDisplaySelected = false;

	void Draw();
};
