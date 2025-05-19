#include "openglHelper.h"
#include "helpers.h"
#include "player.h"
#include "offsets.h"

#include <cstdint>
#include <cstddef>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <corecrt_math.h>
#include <iostream>
#include <map>

namespace openGLHelper
{
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


	bool isInsideAimbotFov(Vector2 pos, float radius)
	{
		GLint view[4];
		glGetIntegerv(GL_VIEWPORT, view);
		float centerX = view[2] / 2;
		float centerY = view[3] / 2;


		float dx = pos.x - centerX;
		float dy = pos.y - centerY;

		return (dx * dx + dy * dy) <= (radius * radius);
	}

	uint32_t fnv1a_hash32(const void* data, std::size_t len) {
		const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data);
		uint32_t hash = 2166136261u; // 32-bit FNV offset basis
		constexpr uint32_t prime = 16777619u; // 32-bit FNV prime

		for (std::size_t i = 0; i < len; ++i) {
			hash ^= bytes[i];
			hash *= prime;
		}
		return hash;
	}

	std::map<int, uint32_t> alreadyComputedHashes;

	bool alreadyHasHashForTexture(int textureID)
	{
		return alreadyComputedHashes.count(textureID);
	}

	uint32_t getHashForTexture(int textureID)
	{
		return alreadyComputedHashes[textureID];
	}

	uint32_t buildHashForTexture(const void* data, std::size_t len, int textureID)
	{
		if (!alreadyHasHashForTexture(textureID))
		{
			uint32_t hash = fnv1a_hash32(data, len);
			alreadyComputedHashes[textureID] = hash;

			return hash;
		}
		else
		{
			return alreadyComputedHashes[textureID];
		}
	}

	void drawFOVCircle(float radius)
	{
		GLint view[4];
		glGetIntegerv(GL_VIEWPORT, view);

		int segmentCount = 256;
		float posX = view[2] / 2;
		float posY = view[3] / 2;

		glBegin(GL_LINE_LOOP);

		for (int i = 0; i < segmentCount; i++) {
			float theta = 2.0f * 3.1415926f * float(i) / float(segmentCount);
			float x = radius * cosf(theta);
			float y = radius * sinf(theta);
			glVertex2f(x + posX, y + posY);
		}
		glEnd();
	}

	bool worldToScreen(Vector3 pos, Vector2& screen)
	{
		GLint view[4];
		glGetIntegerv(GL_VIEWPORT, view);
		float matrix[16];

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

	void convertColor(float colors[3], int* color)
	{
		for (int i = 0; i < 3; i++)
		{
			color[i] = static_cast<int>(colors[i] * 255.0f + 0.5f);
		}
	}

	void drawHealthBar(const Vector4& rectBox, float healthPercent)
	{
		const float barWidth = 5.0f;
		const float margin = 2.0f;

		float barX = rectBox.x - margin - barWidth;
		float barY = rectBox.y;
		float barH = rectBox.height;
		float filledH = barH * healthPercent;
		float filledY = barY + (barH - filledH);

		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);

		glBegin(GL_QUADS);
		if (healthPercent == 1) healthPercent == 0.99999f;

		GLubyte r, g;
		if (healthPercent >= 0.5f) {
			float t = (healthPercent - 0.5f) / 0.5f;
			r = (GLubyte)((1.0f - t) * 255.0f);
			g = 255;
		}
		else {
			float t = healthPercent / 0.5f;
			r = 255;
			g = (GLubyte)(t * 255.0f);
		}

		glColor3ub(r, g, 0.0f);
		glVertex2f(barX, barY + barH);
		glVertex2f(barX + barWidth, barY + barH);
		glVertex2f(barX + barWidth, filledY);
		glVertex2f(barX, filledY);
		glEnd();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);

		glLineWidth(1.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2f(barX, barY);
		glVertex2f(barX + barWidth, barY);
		glVertex2f(barX + barWidth, barY + barH);
		glVertex2f(barX, barY + barH);
		glEnd();
	}

	void drawBox(Vector4 rectBox, float* color, float playerHealth)
	{
		openGLHelper::setupOpenGLForDrawing();
		int gluColor[3] = { 0, 0, 0 };
		convertColor(color, gluColor);

		glLineWidth(1.0f);
		glBegin(GL_LINE_STRIP);
		glColor3ub(gluColor[0], gluColor[1], gluColor[2]);

		glVertex2f(rectBox.x - 0.5f, rectBox.y - 0.5f);
		glVertex2f(rectBox.x + rectBox.width + 0.5f, rectBox.y - 0.5f);
		glVertex2f(rectBox.x + rectBox.width + 0.5f, rectBox.y + rectBox.height + 0.5f);
		glVertex2f(rectBox.x - 0.5f, rectBox.y + rectBox.height + 0.5f);
		glVertex2f(rectBox.x - 0.5f, rectBox.y - 0.5f);
		glEnd();

		drawHealthBar(rectBox, playerHealth);

		openGLHelper::restoreGLState();
	}

	void makeWeaponChamsTexture()
	{
		int gluColor[3] = { 0, 0, 0 };
		openGLHelper::convertColor(weaponChamsColor, gluColor);

		glGenTextures(1, &weaponChamsTexture);
		glBindTexture(GL_TEXTURE_2D, weaponChamsTexture);
		GLubyte pixel[4] = { gluColor[0], gluColor[1], gluColor[2], 255 };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void restoreGLState()
	{
		glPopMatrix();
		glPopAttrib();
	}
}