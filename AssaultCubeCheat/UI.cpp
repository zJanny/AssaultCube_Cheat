#include "UI.h"
#include <Windows.h>
#include <iostream>

#include "imgui/imgui.h"

namespace UI
{
    void drawUI()
    {
		ImGui::Begin("Overlay");
		ImGui::Text("Hello from ImGui!");
		ImGui::End();
    }
}
