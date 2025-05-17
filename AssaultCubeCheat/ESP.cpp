#include "ESP.h"
#include "offsets.h"
#include "memory.h"
#include "openglHelper.h"

#include <iostream>

namespace ESP
{
	void drawESPBox(Player* player)
	{
		if (player == nullptr || player->health <= 0) return;

		Vector2 pos = Vector2();
		if (!openGLHelper::worldToScreen(player->location, pos)) return;

		Vector3 headPos = Vector3(player->location.x, player->location.y, player->location.z);
		headPos.z += 0.8f;

		Vector3 feetPos = Vector3(headPos.x, headPos.y, headPos.z);
		feetPos.z -= player->eyeHeight;

		Vector2 screenHead = Vector2();
		Vector2 screenFeet = Vector2();

		openGLHelper::worldToScreen(headPos, screenHead);
		openGLHelper::worldToScreen(feetPos, screenFeet);

		float distance = screenHead.distance(screenFeet) * 1.2f;
		Vector4 rectBox = Vector4(screenHead.x - (distance / 4.0f), screenHead.y, distance / 2.0f, distance);
		float* color = nullptr;

		if (memory::getLocalPlayer()->team == player->team)
		{
			color = friendlyColor;
		}
		else
		{
			color = enemyColor;
		}

		openGLHelper::drawBox(rectBox, color);
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