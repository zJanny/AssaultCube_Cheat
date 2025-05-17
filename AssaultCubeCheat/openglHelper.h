#pragma once
#include "helpers.h"

namespace openGLHelper
{
	void setupOpenGLForDrawing();
	void restoreGLState();
	void drawFOVCircle(float posX);
	bool worldToScreen(Vector3 pos, Vector2& screen);
	void drawBox(Vector4 rectBox, float* color);
	bool isInsideAimbotFov(Vector2 pos, float radius);
}