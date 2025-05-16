#include "UI.h"
#include <Windows.h>
#include <iostream>
#include <SDL.h>

#include "imgui/imgui.h"

namespace UI
{
	bool shouldDrawUI = false;

    void drawUI()
    {
		if (GetAsyncKeyState(VK_INSERT) & (0x8000 != 0))
		{
			shouldDrawUI = !shouldDrawUI;

			if (shouldDrawUI)
			{
				ShowCursor(true);
				SDL_CaptureMouse(SDL_FALSE);
				SDL_SetRelativeMouseMode(SDL_FALSE);
				SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				SDL_ShowCursor(true);
			}
			else
			{
				SDL_ShowCursor(false);
				SDL_CaptureMouse(SDL_TRUE);
				SDL_SetRelativeMouseMode(SDL_TRUE);
				SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "1");
				ShowCursor(false);
			}
		}

		if (!shouldDrawUI) return;

		ImGui::Begin("Overlay");
		ImGui::Text("Hello from ImGui!");
		ImGui::End();
    }
}
