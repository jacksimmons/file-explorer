#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "ImGuiInclude.hpp"
#include "MenuBar.hpp"
#include "FileDisplay.hpp"
#include "InfoDisplay.hpp"
#include "ViewerDisplay.hpp"

ImGuiWindowFlags MAIN_FLAGS =
ImGuiWindowFlags_NoResize |
ImGuiWindowFlags_NoCollapse |
ImGuiWindowFlags_NoTitleBar |
ImGuiWindowFlags_MenuBar;

int main(int argc, char **argv)
{
	if (!glfwInit())
		return -1;

	GLFWwindow *window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);
	if (window == nullptr)
		return -1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsDark();


	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	MenuBar menuBar;
	FileDisplay fileDisplay;
	InfoDisplay infoDisplay;
	ViewerDisplay viewerDisplay;

	double prevFileDispUpdate = -INFINITY;
	double prevInfoDispUpdate = -INFINITY;
	double now;
	bool updateFileSystem = false;
	while (!glfwWindowShouldClose(window))
	{
		now = glfwGetTime();
		glfwPollEvents();

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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
		// Menus which are updated at fixed intervals, or when updateFileSystem == true.
		if (updateFileSystem)
		{
			updateFileSystem = fileDisplay.Draw(true);
			infoDisplay.Draw(true, currentPath, selectedFile);
		}
		else if (now - prevInfoDispUpdate > 0.1)
		{
			updateFileSystem = fileDisplay.Draw(false);
			infoDisplay.Draw(true, currentPath, selectedFile);
			prevInfoDispUpdate = now;
		}
		else if (now - prevFileDispUpdate > 1)
		{
			updateFileSystem = fileDisplay.Draw(true);
			infoDisplay.Draw(false);
			prevFileDispUpdate = now;
		}
		else
		{
			updateFileSystem = fileDisplay.Draw(false);
			infoDisplay.Draw(false);
		}

		if (fileDisplay.currentPath() != "" && fileDisplay.selectedFile() != "")
		{
			viewerDisplay.Draw(viewerDisplay.currentFilePath() != (currentPath / selectedFile), currentPath, selectedFile);
		}
		else
		{
			viewerDisplay.Draw(false);
		}

		ImGui::End();
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}