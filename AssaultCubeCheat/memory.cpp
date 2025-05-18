#include "memory.h"
#include "offsets.h"
#include <windows.h>
#include <psapi.h>

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

	uintptr_t FindPattern(uintptr_t base, size_t size, const char* pattern, const char* mask) 
	{
		size_t patternLen = strlen(mask);
		for (uintptr_t addr = base; addr < base + size - patternLen; ++addr) 
		{
			bool found = true;
			for (size_t i = 0; i < patternLen; ++i) 
			{
				if (mask[i] != '?' && pattern[i] != *reinterpret_cast<char*>(addr + i)) 
				{
					found = false;
					break;
				}
			}
			if (found) return addr;
		}
		return 0;
	}

	bool GetModuleBounds(const char* moduleName, uintptr_t& base, size_t& size) 
	{
		HMODULE hMod = GetModuleHandleA(moduleName);
		if (!hMod) return false;

		MODULEINFO mi = {};
		if (!GetModuleInformation(GetCurrentProcess(), hMod, &mi, sizeof(mi)))
			return false;

		base = reinterpret_cast<uintptr_t>(mi.lpBaseOfDll);
		size = static_cast<size_t>(mi.SizeOfImage);
		return true;
	}

	uintptr_t ScanModuleForPattern(const char* moduleName, const char* pattern, const char* mask) 
	{
		uintptr_t base; size_t size;
		if (!GetModuleBounds(moduleName, base, size))
			return 0;
		return FindPattern(base, size, pattern, mask);
	}
}