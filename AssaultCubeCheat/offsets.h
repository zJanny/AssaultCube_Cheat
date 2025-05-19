#pragma once
#include <Windows.h>

namespace offsets
{
	inline uintptr_t localPlayer = 0x18AC00;
	inline uintptr_t playerList = 0x18AC04;
	inline uintptr_t playerCount = 0x18AC0C;
	inline uintptr_t projectionMatrix = 0x57DFD0;
	inline uintptr_t hasDRE = 0x57e148;

	inline const char* rendermodelSignature = "\x83\xEC\x24\x83\x3D\x9C\xAA\x58\x00\x00\x8B\xC2\x53\x8B\x5C\x24\x4C\x55\x8B";
	inline const char* renderModuleMask = "xxxxxxxx??xxxxxxxxx";

	inline const char* isOccludedSignature = "\x83\xEC\x08\x0F\x2F\xC2\x56\x0F\x28\xE1\xF3\x0F\x11\x44\x24\x08\x0F\x57\xFF\x0F\x57";
	inline const char* isOccludedMask = "xxxxxxxxxxxxxxxxxxxxx";
}
