#pragma once
#include "helpers.h"

namespace openGLHelper
{
	void setupOpenGLForDrawing();
	void restoreGLState();
	void drawFOVCircle(float posX);
	bool worldToScreen(Vector3 pos, Vector2& screen);
	void drawBox(Vector4 rectBox, float* color);
	bool isInsideAimbotFov(Vector2 pos, float radius);
	uint32_t buildHashForTexture(const void* data, std::size_t len, int textureID);
	uint32_t getHashForTexture(int textureID);
	bool alreadyHasHashForTexture(int textureID);
	inline uint32_t carbineTextureHash = 2110898661;
	inline uint32_t smgTextureHash = 775416272;
	inline uint32_t rifleTextureHash = 3196943842;
	inline uint32_t sniperTextureHash = 1099342562;
	inline uint32_t shotgunTextureHash = 2523319033;
	inline uint32_t knifeTextureHash = 1473260039;
	inline uint32_t pistolTextureHash = 2190390898;
	inline uint32_t grenadeTextureHash = 542601924;
}