#include "cheat.h"
#include "memory.h"
#include <Windows.h>
#include <iostream>
#include "player.h"
#include "offsets.h"
#include "UI.h"

namespace cheat
{
	void runCheat()
	{
		int playerCount = *(int*)(memory::getMemoryAddrFromPointer(offsets::playerCount, {}));
		for (int i = 1; i < playerCount; i++)
		{
			Player* player = *(Player**)memory::getMemoryAddrFromPointer(offsets::playerList, {(unsigned int)(i * 4)});
			
			std::cout << "Player " << i << " health: " << player->health << std::endl;
		}
	}
}
