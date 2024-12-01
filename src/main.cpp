#include <iostream>
#include <memory>

#include "ImGuiInclude.hpp"

#include "main.hpp"
#include "MenuBar.hpp"
#include "FileExplorerDisplay.hpp"
#include "FileInfoDisplay.hpp"
#include "FileViewerDisplay.hpp"

ImGuiWindowFlags MAIN_FLAGS =
ImGuiWindowFlags_NoResize |
ImGuiWindowFlags_NoCollapse |
ImGuiWindowFlags_NoTitleBar |
ImGuiWindowFlags_MenuBar;

int main(int argc, char **argv)
{
	// GLFW setup
	if (!glfwInit())
		return -1;

	GLFWwindow *window = glfwCreateWindow(1280, 720, "File Explorer", nullptr, nullptr);
	if (window == nullptr)
		return -1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Dear ImGui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Vanity setup
	ImGui::StyleColorsDark();

	// Constants
	const float UPDATE_INTERVAL_FILE_DISP = 1.0f;
	const float UPDATE_INTERVAL_INFO_DISP = 0.1f;

	MenuBar menuBar;
	FileExplorerDisplay fileDisplay;
	FileInfoDisplay infoDisplay;
	ViewerDisplay viewerDisplay;

	double prevFileDispUpdate = -INFINITY;
	double prevInfoDispUpdate = -INFINITY;
	double now;
	bool updateFileSystem = false;

	// Loop: Poll -> NewFrame -> (Draw Code) -> Render
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		NewFrame();

		now = glfwGetTime();

		if (menuBar.m_fileExplorerSelected)
		{
			fileDisplay.setActive(true);
			menuBar.m_fileExplorerSelected = false;
		}
		if (menuBar.m_fileInfoSelected)
		{
			infoDisplay.setActive(true);
			menuBar.m_fileInfoSelected = false;
		}
		if (menuBar.m_fileViewerSelected)
		{
			viewerDisplay.setActive(true);
			menuBar.m_fileViewerSelected = false;
		}

		ImGui::SetNextWindowPos(ImVec2(0, 20));
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
		ImGui::Begin("Main", nullptr, MAIN_FLAGS);

		menuBar.Draw();

		fs::path currentPath = fileDisplay.currentPath();
		std::string selectedFile = fileDisplay.selectedFile();
		
		// Menus which are updated at fixed intervals, or when updateFileSystem == true
		updateFileSystem = fileDisplay.Draw(now - prevFileDispUpdate > 1 || updateFileSystem);
		infoDisplay.Draw(
			now - prevInfoDispUpdate > 0.1 || updateFileSystem,
			currentPath,
			selectedFile
		);
		viewerDisplay.Draw(
			fileDisplay.currentPath() != "" && fileDisplay.selectedFile() != "" || updateFileSystem,
			currentPath,
			selectedFile
		);

		ImGui::End();

		// 
		if (now - prevFileDispUpdate > UPDATE_INTERVAL_FILE_DISP)
		{
			prevFileDispUpdate = now;
		}

		if (now - prevInfoDispUpdate > UPDATE_INTERVAL_INFO_DISP)
		{
			prevInfoDispUpdate = now;
		}

		Render(window);
	}

	Teardown();
}

void NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Render(GLFWwindow *window)
{
	constexpr ImVec4 COL_CLEAR = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Render();

	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);

	glViewport(0, 0, display_w, display_h);
	glClearColor(COL_CLEAR.x * COL_CLEAR.w, COL_CLEAR.y * COL_CLEAR.w, COL_CLEAR.z * COL_CLEAR.w, COL_CLEAR.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}

void Teardown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}