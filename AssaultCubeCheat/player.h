#pragma once

// Created with ReClass.NET 1.2 by KN4CK3R

struct Vector3
{
	float x, y, z;
};

class Player
{
public:
	char pad_0000[4]; //0x0000
	Vector3 location; //0x0004
	Vector3 velocity; //0x0010
	char pad_001C[24]; //0x001C
	float viewAngle; //0x0034
	char pad_0038[180]; //0x0038
	int32_t health; //0x00EC
	char pad_00F0[336]; //0x00F0
}; //Size: 0x0240

struct PlayerList
{
	Player* players[32];
};
