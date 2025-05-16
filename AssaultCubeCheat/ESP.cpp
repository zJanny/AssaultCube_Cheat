#include "ESP.h"
#include "memory.h"
#include "player.h"
#include "offsets.h"

#include <iostream>
#include <Windows.h>
#include <gl/GLU.h>
#include <gl/GL.h>

namespace ESP
{
	bool worldToScreen(Vector3 pos, Vector2& screen)
	{
		GLint   view[4];
		glGetIntegerv(GL_VIEWPORT, view);
		float matrix[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		for (int i = 0; i < 16; i++)
		{
			matrix[i] = *(float*)(offsets::projectionMatrix + (i * sizeof(float)));
		}

		Vector4 clipCoords;

		clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
		clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
		clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
		clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

		if (clipCoords.w < 0.1f)
		{
			return false;
		}

		Vector3 NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		screen.x = view[0] + (NDC.x * 0.5f + 0.5f) * view[2];
		screen.y = view[1] + (NDC.y * 0.5f + 0.5f) * view[3];

		return true;
	}

	void drawESPBox(Player* player)
	{
		Vector2 position = Vector2();
		if (!worldToScreen(player->location, position)) return;

		glEnd();
	}

	void runESP()
	{
		int playerCount = *(int*)(memory::getMemoryAddrFromPointer(offsets::playerCount, {}));
		for (int i = 1; i < playerCount; i++)
		{
			Player* player = *(Player**)memory::getMemoryAddrFromPointer(offsets::playerList, { (unsigned int)(i * 4) });

			drawESPBox(player);
		}
	}
}