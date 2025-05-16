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
		clipCoords.width = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
		clipCoords.height = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

		if (clipCoords.height < 0.1f)
		{
			return false;
		}

		Vector3 NDC;
		NDC.x = clipCoords.x / clipCoords.height;
		NDC.y = clipCoords.y / clipCoords.height;
		NDC.z = clipCoords.width / clipCoords.height;

		screen.x = (view[2] / 2 * NDC.x) + (NDC.x + view[2] / 2);
		screen.y = -(view[3] / 2 * NDC.y) + (NDC.y + view[3] / 2);

		return true;
	}

	void setupOpenGLForDrawing()
	{
		GLint viewport[4];
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		glGetIntegerv(GL_VIEWPORT, viewport);
		glViewport(0, 0, viewport[2], viewport[3]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
	}

	void restoreGLState()
	{
		glPopMatrix();
		glPopAttrib();
	}

	void drawESPBox(Player* player)
	{
		Vector2 pos = Vector2();
		if (!worldToScreen(player->location, pos)) return;

		Vector3 headPos = Vector3(player->location.x, player->location.y, player->location.z);
		headPos.z += 0.8f;

		Vector3 feetPos = Vector3(headPos.x, headPos.y, headPos.z);
		feetPos.z -= player->eyeHeight;

		Vector2 screenHead = Vector2();
		Vector2 screenFeet = Vector2();

		worldToScreen(headPos, screenHead);
		worldToScreen(feetPos, screenFeet);

		float distance = screenHead.distance(screenFeet) * 1.2f;
		Vector4 rectBox = Vector4(screenHead.x - (distance / 4.0f), screenHead.y, distance / 2.0f, distance);

		setupOpenGLForDrawing();

		glLineWidth(1.0f);
		glBegin(GL_LINE_STRIP);
		glColor3ub(255, 0 ,0);
		glVertex2f(rectBox.x - 0.5f, rectBox.y - 0.5f);
		glVertex2f(rectBox.x + rectBox.width + 0.5f, rectBox.y - 0.5f);
		glVertex2f(rectBox.x + rectBox.width + 0.5f, rectBox.y + rectBox.height + 0.5f);
		glVertex2f(rectBox.x - 0.5f, rectBox.y + rectBox.height + 0.5f);
		glVertex2f(rectBox.x - 0.5f, rectBox.y - 0.5f);
		glEnd();

		restoreGLState();
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