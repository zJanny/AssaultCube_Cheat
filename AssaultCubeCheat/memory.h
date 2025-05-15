#pragma once
#include <Windows.h>
#include <vector>

namespace memory
{
	uintptr_t getMemoryAddrFromPointer(uintptr_t  startAddr, std::vector<unsigned int> pointers);
}