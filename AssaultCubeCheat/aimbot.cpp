#include "aimbot.h"
#include "memory.h"
#include "player.h"
#include "offsets.h"
#include "openglHelper.h"
#include <iostream>


namespace aimbot
{
	bool isAimbotActive = false;

	bool getIsAimbotActive()
	{
		return isAimbotActive;
	}

	Vector3 getShootLocationForPlayer(Player* player)
	{
		Vector3 shootLocation = Vector3(player->location.x, player->location.y, player->location.z);
		shootLocation.z -= player->feetHeight;
		
		return shootLocation;
	}

	Player* findPlayerToTarget()
	{
		int playerCount = *(int*)(memory::getMemoryAddrFromPointer(offsets::playerCount, {}));
		Player* localPlayer = memory::getLocalPlayer();
		std::vector<Player*> playersInsideFOV = {};

		for (int i = 1; i < playerCount; i++)
		{
			Player* current = *(Player**)memory::getMemoryAddrFromPointer(offsets::playerList, { (unsigned int)(i * 4) });
			if (current->health <= 0 || current->team == localPlayer->team) continue;

			Vector3 shootLocation = getShootLocationForPlayer(current);
			Vector2 screenLocation = Vector2();

			openGLHelper::worldToScreen(shootLocation, screenLocation);
			if (openGLHelper::isInsideAimbotFov(screenLocation, aimbotFOV))
			{
				playersInsideFOV.push_back(current);
			}
		}

		if (playersInsideFOV.size() == 1)
		{
			return playersInsideFOV[0];
		}

		if (playersInsideFOV.size() > 1)
		{
			float minDistance = 9999999999;
			Player* closestPlayer = nullptr;

			for (Player* player : playersInsideFOV)
			{
				float distance = helper::computeDistance(localPlayer->location, player->location);
				if (distance < minDistance)
				{
					minDistance = distance;
					closestPlayer = player;
				}
			}

			return closestPlayer;
		}

		return nullptr;
	}

	void runAimbot()
	{
		if (drawFOVCircle)
		{
			openGLHelper::setupOpenGLForDrawing();
			openGLHelper::drawFOVCircle(aimbotFOV);
			openGLHelper::restoreGLState();
		}

		if (!shouldAim)
		{
			isAimbotActive = false;
			return;
		}

		Player* localPlayer = memory::getLocalPlayer();
		Player* target = findPlayerToTarget();
		if (target == nullptr)
		{
			isAimbotActive = false;
			return;
		}

		Vector3 shootLocation = getShootLocationForPlayer(target);

		Vector2 screenPosition = Vector2();
		openGLHelper::worldToScreen(shootLocation, screenPosition);

		Vector2 newLookDirection = helper::computeAngle(localPlayer->location, shootLocation);

		localPlayer->yaw = newLookDirection.x;
		localPlayer->pitch = newLookDirection.y;
		isAimbotActive = true;
	}
}