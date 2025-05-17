#include <Windows.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_opengl2.h"
#include <kiero.h>
#include <SDL.h>
#include <MinHook.h>

#include "UI.h"
#include "hooks.h"
#include "cheat.h"
#include "aimbot.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

namespace hooks
{
	typedef BOOL(__stdcall* wglSwapBuffers_t) (HDC hDc);
	typedef int (SDLCALL* SDL_PollEvent_t)(SDL_Event*);
	WNDPROC oWndProc;
	SDL_PollEvent_t fpOriginalSDL_PollEvent = nullptr;
	wglSwapBuffers_t o_wglSwapBuffers = nullptr;
	bool imgui_initialized = false;

	LRESULT CALLBACK hWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (UI::shouldDrawUI)
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		}

		return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
	}

	void initImGUI(HDC hDc)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();

		HWND hwnd = WindowFromDC(hDc);

		oWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)hWndProc);

		ImGui_ImplWin32_Init(hwnd);
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

		ImGui_ImplWin32_NewFrame();
		ImGui_ImplOpenGL2_NewFrame();
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

	int SDLCALL Hooked_SDL_PollEvent(SDL_Event* event)
	{
		int response = fpOriginalSDL_PollEvent(event);

		if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)
		{
			if (cheat::toggleAimbot)
			{
				SDL_MouseButtonEvent mouseEvent = event->button;
				if (mouseEvent.button == SDL_BUTTON_RIGHT)
				{
					if (mouseEvent.type == SDL_MOUSEBUTTONDOWN)
					{
						aimbot::shouldAim = true;
					}

					if (mouseEvent.type == SDL_MOUSEBUTTONUP)
					{
						aimbot::shouldAim = false;
					}
				}
			}
		}

		if (event->type == SDL_MOUSEMOTION)
		{
			if (aimbot::getIsAimbotActive() && cheat::toggleAimbot)
			{
				event->motion = SDL_MouseMotionEvent();
			}
		}

		if (UI::shouldDrawUI)
		{
			return 0;
		}

		return response;
	}

	void initSDLHooks()
	{
		HMODULE hSDL = GetModuleHandleA("SDL2.dll");
		if (hSDL == nullptr) throw std::runtime_error("Cant find SDL2.dll");

		void* pSDL_PollEvent = GetProcAddress(hSDL, "SDL_PollEvent");

		MH_STATUS status = MH_CreateHook(pSDL_PollEvent, &Hooked_SDL_PollEvent, reinterpret_cast<LPVOID*>(&fpOriginalSDL_PollEvent));
		std::cout << "Create SDL_PollEvent hook status " << status << std::endl;

		MH_EnableHook(pSDL_PollEvent);
	}

	void enableHooks()
	{
		initKiero();
		initSDLHooks();

		std::cout << "Hooks enabled" << std::endl;
	}
}