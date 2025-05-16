#include <Windows.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_opengl2.h"
#include <kiero.h>

#include "UI.h"
#include "hooks.h"
#include "cheat.h"

namespace hooks
{
	typedef BOOL(__stdcall* wglSwapBuffers_t) (HDC hDc);
	wglSwapBuffers_t o_wglSwapBuffers = nullptr;
	bool imgui_initialized = false;

	void initImGUI(HDC hDc)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(WindowFromDC(hDc));
		ImGui_ImplOpenGL2_Init();

		std::cout << "Initialized ImGUI" << std::endl;
	}

	BOOL WINAPI hk_wglSwapBuffers(HDC hDc)
	{
		if (!imgui_initialized)
		{
			initImGUI(hDc);
			imgui_initialized = true;
		}

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		UI::drawUI();
		cheat::runCheat();

		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		return o_wglSwapBuffers(hDc);
	}

	void initKiero()
	{
		if (kiero::init(kiero::RenderType::OpenGL) != kiero::Status::Success)
		{
			std::cout << "Error on kiero init" << std::endl;
			return;
		}

		std::cout << "Kiero init successful" << std::endl;

		if (kiero::bind(336, (void**)&o_wglSwapBuffers, hk_wglSwapBuffers) != kiero::Status::Success)
		{
			std::cout << "Error on kiero bind" << std::endl;
			return;
		}

		std::cout << "Kiero bind successful" << std::endl;
	}

	void enableHooks()
	{
		initKiero();

		std::cout << "Hooks enabled" << std::endl;
	}
}