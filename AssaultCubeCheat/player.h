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
	float viewAngle; //0x0034
	char pad_0038[20]; //0x0038
	float feetHeight; //0x004C
	float eyeHeight; //0x0050
	char pad_0054[152]; //0x0054
	int32_t health; //0x00EC
	char pad_00F0[336]; //0x00F0
}; //Size: 0x0240