#include <Windows.h>
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_opengl2.h"
#include <kiero.h>
#include <SDL.h>
#include <MinHook.h>
#include <gl/GL.h>
#include <vector>
#include <string>

#include "UI.h"
#include "hooks.h"
#include "cheat.h"
#include "aimbot.h"
#include "memory.h"
#include "openglHelper.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "offsets.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

namespace hooks
{
	struct playerent;
	struct modelattach;

	typedef BOOL(APIENTRY* wglSwapBuffers_t) (HDC);
	typedef int (SDLCALL* SDL_PollEvent_t)(SDL_Event*);
	typedef void(APIENTRY* glBindTexture_t)(GLenum, GLuint);
	typedef void(APIENTRY* glDrawElements_t)(GLenum, GLsizei, GLenum, const void*);
	typedef int(__vectorcall* isoccluded_t)(float, float, float, float, float);

	SDL_PollEvent_t fpOriginalSDL_PollEvent = nullptr;
	wglSwapBuffers_t o_wglSwapBuffers = nullptr;
	glBindTexture_t o_glBindTexture = nullptr;
	glDrawElements_t o_glDrawElements = nullptr;
	isoccluded_t o_isOccluded = nullptr;
	void* o_rendermodel = nullptr;

	WNDPROC oWndProc;
	bool imgui_initialized = false;
	bool applyPlayerChams = false;

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

	void applyChams(const char* mdl)
	{
		if (strcmp(mdl, "playermodels") == 0)
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	//I have no idea why i have to hook it like this, but otherwhise it wont work
	__declspec(naked) void __fastcall hk_rendermodel()
	{
		__asm {
			pushad
			pushfd

			push ecx //model string is saved in ECX register
			call applyChams
			add esp, 4

			popfd
			popad

			mov  eax, dword ptr ds : [o_rendermodel]
			jmp  eax
		}
	}

	int __vectorcall hk_isOccluded(float vx, float vy, float cx, float cy, float csize)
	{
		o_isOccluded(vx, vy, cx, cy, csize);
		return 0;
	}

	void initGameHooks()
	{
		void* rendermodel = (void*)memory::ScanModuleForPattern(nullptr, offsets::rendermodelSignature, offsets::renderModuleMask);
		void* isOccluded = (void*)memory::ScanModuleForPattern(nullptr, offsets::isOccludedSignature, offsets::isOccludedMask);

		MH_STATUS status = MH_CreateHook(rendermodel, &hk_rendermodel, &o_rendermodel);
		std::cout << "Rendermodel hook status " << status << std::endl;

		status = MH_CreateHook(isOccluded, &hk_isOccluded, reinterpret_cast<LPVOID*>(&o_isOccluded));
		std::cout << "isOccluded hook status " << status << std::endl;

		MH_EnableHook(rendermodel);
		MH_EnableHook(isOccluded);
	}

	BOOL WINAPI hk_wglSwapBuffers(HDC hDc)
	{
		if (!imgui_initialized)
		{
			initImGUI(hDc);
			//initGameHooks();
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

	
	void APIENTRY hk_glBindTexture(GLenum target, GLuint texture)
	{
		o_glBindTexture(target, texture);
		if (!weaponChams) return;

		if (target == GL_TEXTURE_2D)
		{
			if (!openGLHelper::alreadyHasHashForTexture(texture))
			{
				GLint width, height;
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
				std::vector<GLubyte> buffer(width * height * 4);
				glPixelStorei(GL_PACK_ALIGNMENT, 1);
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());

				openGLHelper::buildHashForTexture(buffer.data(), buffer.size(), texture);
			}


			uint32_t hash = openGLHelper::getHashForTexture(texture);

			// Weapon chams
			if (hash == openGLHelper::carbineTextureHash || hash == openGLHelper::rifleTextureHash ||
				hash == openGLHelper::sniperTextureHash || hash == openGLHelper::shotgunTextureHash || 
				hash == openGLHelper::smgTextureHash || hash == openGLHelper::knifeTextureHash || hash == openGLHelper::pistolTextureHash || hash == openGLHelper::grenadeTextureHash)
			{
				if (openGLHelper::weaponChamsTexture == 0) openGLHelper::makeWeaponChamsTexture();

				o_glBindTexture(target, openGLHelper::weaponChamsTexture);
			}
		}
	}

	void APIENTRY hk_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
	{
		if (applyPlayerChams)
		{


			o_glDrawElements(mode, count, type, indices);


			applyPlayerChams = false;

			return;
		}

		o_glDrawElements(mode, count, type, indices);
	}


	void initKiero()
	{
		*reinterpret_cast<bool*>(offsets::hasDRE) = 0; //set hasDRE to 0 to make the game use glDrawElements 

		if (kiero::init(kiero::RenderType::OpenGL) != kiero::Status::Success)
		{
			std::cout << "Error on kiero init" << std::endl;
			return;
		}

		std::cout << "Kiero init successful" << std::endl;

		if (kiero::bind(336, (void**)&o_wglSwapBuffers, hk_wglSwapBuffers) != kiero::Status::Success)
		{
			std::cout << "Error when hooking wglSwapBuffers" << std::endl;
			return;
		}

		std::cout << "Hooked wglSwapBuffers" << std::endl;

		if (kiero::bind(5, (void**)&o_glBindTexture, hk_glBindTexture) != kiero::Status::Success)
		{
			std::cout << "Error when hooking glBindTexture" << std::endl;
			return;
		}

		std::cout << "Hooked glBindTexture" << std::endl;
		/*
		if (kiero::bind(67, (void**)&o_glDrawElements, hk_glDrawElements) != kiero::Status::Success)
		{
			std::cout << "Error when hooking glDrawElements" << std::endl;
			return;
		}

		std::cout << "Hooked glDrawElements" << std::endl;**/
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