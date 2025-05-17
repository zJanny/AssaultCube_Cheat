#pragma once
#include <cmath>
#define PI 3.14159

struct Vector4
{
	float x, y, width, height;

	Vector4(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	Vector4()
	{
		this->x = 0;
		this->y = 0;
		this->width = width;
		this->height = height;
	}
};

struct Vector3
{
	float x, y, z;

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
};

struct Vector2
{
	float x, y;

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2()
	{
		this->x = 0;
		this->y = 0;
	}

	float distance(Vector2 other)
	{
		float dx = x - other.x;
		float dy = y - other.y;

		return std::sqrt(dx * dx + dy * dy);
	}
};

namespace helper
{
	inline Vector2 computeAngle(Vector3 src, Vector3 dst)
	{
		float dx = dst.x - src.x;
		float dy = dst.y - src.y;
		float dz = dst.z - src.z;

		double yaw_rad = atan2f(dy, dx);
		double yaw_deg = yaw_rad * (180.0 / PI);

		double horiz_dist = sqrtf(dx * dx + dy * dy);
		double pitch_rad = atan2f(dz, horiz_dist);
		double pitch_deg = pitch_rad * (180.0 / PI);

		return Vector2((float)yaw_deg + 90, (float)pitch_deg);
	}

	inline float computeDistance(Vector3 src, Vector3 dst)
	{
		return sqrt(pow(dst.x - src.x, 2) + pow(dst.y - src.y, 2) + pow(dst.z - src.z, 2));
	}
}