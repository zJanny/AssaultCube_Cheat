#include "memory.h"
#include "offsets.h"

namespace memory
{
	Player* getLocalPlayer()
	{
		return *(Player**)(getMemoryAddrFromPointer(offsets::localPlayer, {}));
	}

	uintptr_t getMemoryAddrFromPointer(uintptr_t  startAddr, std::vector<unsigned int> pointers)
	{
		uintptr_t addr = startAddr + (uintptr_t)GetModuleHandle(NULL);

		for (unsigned int pointer : pointers)
		{
			addr = *(uintptr_t*)addr;
			addr += pointer;
		}

		return addr;
	}
}