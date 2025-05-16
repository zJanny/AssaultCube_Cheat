#pragma once
#include "helpers.h"

// Created with ReClass.NET 1.2 by KN4CK3R

class Player
{
public:
	char pad_0000[4]; //0x0000
	Vector3 location; //0x0004
	Vector3 velocity; //0x0010
	char pad_001C[24]; //0x001C
	float yaw; //0x0034
	float pitch; //0x0038
	char pad_003C[16]; //0x003C
	float feetHeight; //0x004C
	float eyeHeight; //0x0050
	char pad_0054[152]; //0x0054
	int32_t health; //0x00EC
	char pad_00F0[540]; //0x00F0
	int32_t team; //0x030C
	char pad_0310[48]; //0x0310
}; //Size: 0x0340