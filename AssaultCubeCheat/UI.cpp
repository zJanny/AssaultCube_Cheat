#include "UI.h"
#include "kiero/kiero.h"
#include <Windows.h>
#include <iostream>
#include <gl/GL.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_opengl2.h"

namespace UI
{

	typedef void(APIENTRY* glDrawElements_t)(GLenum mode, GLsizei count, GLenum type, const void* indices);
	glDrawElements_t o_glDrawElements = nullptr;
	bool imgui_initialized = false;

	void APIENTRY hk_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) 
	{
		if (!imgui_initialized)
		{
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			ImGui::StyleColorsDark();

			ImGui_ImplOpenGL2_Init();
			HDC hdc = wglGetCurrentDC();
			HWND hwnd = WindowFromDC(hdc);
			if (hwnd == NULL)
			{
				std::cerr << "Failed to retrieve HWND from HDC." << std::endl;
				return;
			}
			ImGui_ImplWin32_Init(hwnd);

			imgui_initialized = true;
		}

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Overlay");
		ImGui::Text("Hello from ImGui!");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		return o_glDrawElements(mode, count, type, indices);
	}

	void initKiero()
	{
		if (kiero::init(kiero::RenderType::OpenGL) != kiero::Status::Success)
		{
			std::cout << "Error on kiero init" << std::endl;
			return;
		}

		std::cout << "Kiero init successful" << std::endl;

		if (kiero::bind(67, (void**)&o_glDrawElements, hk_glDrawElements) != kiero::Status::Success)
		{
			std::cout << "Error on kiero bind" << std::endl;
			return;
		}

		std::cout << "Kiero bind successful" << std::endl;
	}

	void initUI()
	{
		initKiero();
	}
}