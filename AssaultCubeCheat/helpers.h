#pragma once
#include <cmath>

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