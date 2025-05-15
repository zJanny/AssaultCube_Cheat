#include "cheat.h"
#include "memory.h"
#include <Windows.h>
#include <iostream>
#include "player.h"
#include "offsets.h"
#include "UI.h"

namespace cheat
{
	void openConsole()
	{
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	}

	void cheatThread()
	{
		uintptr_t baseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

		openConsole();
		UI::initUI();

		std::cout << "Cheat injected" << std::endl;
		std::cout << "Base address " << std::hex << baseAddr << std::dec << std::endl;
		Player* localPlayer = *(Player**)(memory::getMemoryAddrFromPointer(offsets::localPlayer, {}));


		while (true)
		{
			int playerCount = *(int*)(memory::getMemoryAddrFromPointer(0x18AC0C, {}));
			for (int i = 1; i < playerCount; i++) 
			{
				Player* currentPlayer = *(Player**)(memory::getMemoryAddrFromPointer(offsets::playerList, {(unsigned int)(i) * 4}));
				std::cout << "PLAYER " << i << std::endl;
				std::cout << "Health " << currentPlayer->health << std::endl; 
			}

			Sleep(500);
		}
	}
}