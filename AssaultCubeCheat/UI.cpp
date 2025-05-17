#include <Windows.h>
#include <iostream>
#include <SDL.h>

#include "imgui/imgui.h"
#include "UI.h"
#include "cheat.h"
#include "ESP.h"
#include "aimbot.h"

namespace UI
{
	bool shouldDrawUI = false;
	bool defaultValuesSet = false;

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

		ImGui::Begin("AssaultCube Cheat");
		if (!defaultValuesSet)
		{
			ImGui::SetWindowSize(ImVec2(500, 300));
			defaultValuesSet = true;
		}
		
		ImGui::BeginTabBar("1");

		if (ImGui::BeginTabItem("Visuals"))
		{
			ImGui::Checkbox("ESP", &cheat::toggleESP);
			ImGui::Checkbox("Draw friendlies", &ESP::drawFriendlies);

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Combat"))
		{
			ImGui::Checkbox("Aimbot", &cheat::toggleAimbot);
			ImGui::SliderFloat("Aimbot FOV", &aimbot::aimbotFOV, 1, 360);
			ImGui::Checkbox("Draw FOV circle", &aimbot::drawFOVCircle);

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Settings"))
		{
			ImGui::ColorPicker3("Friendly color", ESP::friendlyColor);
			ImGui::ColorPicker3("Enemy color", ESP::enemyColor);

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();

		ImGui::Text("Developed by zJanny");
		ImGui::End();
    }
}
