#include "cheat.h"
#include "memory.h"
#include "ESP.h"

namespace cheat
{
	void runCheat()
	{
		if (toggleESP)
		{
			ESP::runESP();
		}
	}
}
