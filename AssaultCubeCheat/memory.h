#pragma once
#include <Windows.h>
#include <vector>
#include "player.h"

namespace memory
{
	Player* getLocalPlayer();
	uintptr_t getMemoryAddrFromPointer(uintptr_t  startAddr, std::vector<unsigned int> pointers);
	uintptr_t ScanModuleForPattern(const char* moduleName, const char* pattern, const char* mask);
}