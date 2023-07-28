#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "ImGuiInclude.hpp"
#include "MenuBar.hpp"
#include "FileDisplay.hpp"
#include "InfoDisplay.hpp"

ImGuiWindowFlags DOCKER_FLAGS =
ImGuiWindowFlags_NoResize |
ImGuiWindowFlags_NoCollapse |
ImGuiWindowFlags_NoTitleBar |
ImGuiWindowFlags_MenuBar;

ImGuiWindowFlags MAIN_FLAGS =
DOCKER_FLAGS |
ImGuiWindowFlags_NoDocking;

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


	double prevUpdate = -INFINITY;
	double now;

	auto fileDisplay = std::make_unique<FileDisplay>();
	auto infoDisplay = std::make_unique<InfoDisplay>();
	fileDisplay->UpdateFiles();

	bool update = false;
	while (!glfwWindowShouldClose(window))
	{
		now = glfwGetTime();
		glfwPollEvents();

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
		ImGui::Begin("(Invis)", nullptr, MAIN_FLAGS);

		DrawAltMenuBar();

		// Docker window
		{
			ImGui::SetNextWindowPos(ImVec2(0, 20));
			ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
			ImGui::Begin("Docker", nullptr, DOCKER_FLAGS);

			if (now - prevUpdate > 1)
			{
				update = fileDisplay->Draw(true);
				infoDisplay->Draw(true, fileDisplay->currentPath(), fileDisplay->selectedFile());
				prevUpdate = now;
			}
			else
			{
				update = fileDisplay->Draw(update);
				infoDisplay->Draw(update);
			}

			ImGui::End();
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