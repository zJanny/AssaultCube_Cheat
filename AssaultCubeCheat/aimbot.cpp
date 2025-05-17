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

	Player* findPlayerToTarget()
	{
		int playerCount = *(int*)(memory::getMemoryAddrFromPointer(offsets::playerCount, {}));
		Player* target = nullptr;
		for (int i = 1; i < playerCount; i++)
		{
			target = *(Player**)memory::getMemoryAddrFromPointer(offsets::playerList, { (unsigned int)(i * 4) });
		}

		return target;
	}

	void runAimbot()
	{
		if (drawFOVCircle)
		{
			openGLHelper::setupOpenGLForDrawing();
			openGLHelper::drawFOVCircle(aimbotFOV);
			openGLHelper::restoreGLState();
		}

		if (!shouldAim) return;

		Player* localPlayer = memory::getLocalPlayer();
		Player* target = findPlayerToTarget();
		if (target == nullptr) return;

		Vector3 shootLocation = Vector3(target->location.x, target->location.y, target->location.z);
		shootLocation.z -= target->feetHeight;

		Vector2 screenPosition = Vector2();
		openGLHelper::worldToScreen(target->location, screenPosition);

		if (!openGLHelper::isInsideAimbotFov(screenPosition, aimbotFOV)) return;

		Vector2 newLookDirection = helper::computeAngle(localPlayer->location, shootLocation);

		localPlayer->yaw = newLookDirection.x;
		localPlayer->pitch = newLookDirection.y;
	}
}