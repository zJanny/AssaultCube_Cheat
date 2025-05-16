#include "cheat.h"
#include "memory.h"
#include "ESP.h"
#include "aimbot.h"

namespace cheat
{
	void runCheat()
	{
		if (toggleESP)
		{
			ESP::runESP();
		}

		if (toggleAimbot)
		{
			aimbot::runAimbot();
		}
	}
}
