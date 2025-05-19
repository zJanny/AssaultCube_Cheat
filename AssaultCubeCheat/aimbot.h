#pragma once

namespace aimbot
{
	inline bool shouldAim = false;
	inline bool drawFOVCircle = true;
	inline float aimbotFOV = 150;
	inline bool targetHead;
	void runAimbot();
	bool getIsAimbotActive();
}