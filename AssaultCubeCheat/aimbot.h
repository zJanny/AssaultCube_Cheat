#pragma once

namespace aimbot
{
	inline bool shouldAim = false;
	inline bool drawFOVCircle = true;
	inline float aimbotFOV = 150;
	void runAimbot();
	bool getIsAimbotActive();
}