#include "UI.h"
#include <Windows.h>
#include <iostream>

#include "imgui/imgui.h"
#include "helpers.h"

namespace UI
{
	bool shouldDrawUI = false;

	void worldToScreen(const Vector3& world, Vector3& outScreen)
	{

	}

    void drawUI()
    {
		if (GetAsyncKeyState(VK_INSERT) & (0x8000 != 0))
		{
			shouldDrawUI = !shouldDrawUI;
		}

		if (!shouldDrawUI) return;

		ImGui::Begin("Overlay");
		ImGui::Text("Hello from ImGui!");
		ImGui::End();
    }
}
