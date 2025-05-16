#include "memory.h"
#include "player.h"
#include "offsets.h"
#include <iostream>


namespace aimbot
{
	void runAimbot()
	{
		int playerCount = *(int*)(memory::getMemoryAddrFromPointer(offsets::playerCount, {}));
		Player* localPlayer = memory::getLocalPlayer();

		for (int i = 1; i < playerCount; i++)
		{
			Player* player = *(Player**)memory::getMemoryAddrFromPointer(offsets::playerList, { (unsigned int)(i * 4) });
			Vector2 newLookDirection = helper::computeAngle(localPlayer->location, player->location);

			localPlayer->yaw = newLookDirection.x;
			localPlayer->pitch = newLookDirection.y;
		}
	}
}