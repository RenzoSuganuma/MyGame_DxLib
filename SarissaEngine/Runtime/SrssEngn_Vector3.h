#pragma once

#include "string"
#include "sstream"
#include "stdio.h"

struct Vector3
{
public:
	float x, y, z;

	Vector3() { x = y = z = 0; }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3 operator+(const Vector3& other) noexcept {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other) noexcept {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3& operator+=(const Vector3& other) noexcept {
		x + other.x;
		y + other.y;
		z + other.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& other) noexcept {
		x - other.x;
		y - other.y;
		z - other.z;
		return *this;
	}
	// Še¬•ª‚Ì’l‚ª‚·‚×‚Äˆê’v‚µ‚Ä‚¢‚½‚ç ^
	bool operator==(const Vector3& other) noexcept {
		return x == other.x && y == other.y && z == other.z;
	}
	inline const char* const ToString() const {
		char buff[64];
		snprintf(buff, sizeof(buff), "x:%.3f,y:%.3f,z:%.3f", x, y, z);
		buff[63] = '\0';
		return &buff[0];
	}
};

