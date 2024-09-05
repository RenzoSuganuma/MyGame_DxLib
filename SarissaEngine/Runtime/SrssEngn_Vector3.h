#pragma once

#include "string"
#include "math.h"
#include "sstream"
#include "stdio.h"

struct Vector3
{
public:
	float x, y, z;

	Vector3() { x = y = z = 0; }
	Vector3(float val) { x = y = z = val; }
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3 operator+(const Vector3& other) noexcept {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other) noexcept {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator*(const Vector3& other) noexcept {
		return Vector3(x * other.x, y * other.y, z * other.z);
	}
	Vector3 operator/(const Vector3& other) noexcept {
		return Vector3(x / other.x, y / other.y, z / other.z);
	}
	Vector3& operator+=(const Vector3& other) noexcept {
		x = x + other.x;
		y = y + other.y;
		z = z + other.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& other) noexcept {
		x = x - other.x;
		y = y - other.y;
		z = z - other.z;
		return *this;
	}
	Vector3& operator*=(const Vector3& other) noexcept {
		x = x * other.x;
		y = y * other.y;
		z = z * other.z;
		return *this;
	}
	Vector3& operator/=(const Vector3& other) noexcept {
		x = x / other.x;
		y = y / other.y;
		z = z / other.z;
		return *this;
	}
	// 各成分の値がすべて一致していたら 真
	bool operator==(const Vector3& other) const noexcept {
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}
	// C言語の文字列へ変換した値を返す
	const char* const ToString() const {
		char buff[64];
		snprintf(buff, sizeof(buff), "x:%.3f,y:%.3f,z:%.3f", x, y, z);
		buff[63] = '\0'; // 文字列の末尾には必ず'\0'が番兵として配置されている。これがないと文字列として認識されない。
		return &buff[0];
	}
	// ベクトルの正規化をしたのち、正規化されたベクトルを返す
	Vector3 normalized() noexcept {
		float magnitude = sqrtf(x * x + y * y + z * z);
		float calc_x = this->x / magnitude;
		float calc_y = this->y / magnitude;
		float calc_z = this->z / magnitude;

		return Vector3(calc_x, calc_y, calc_z);
	}
	// 大きさを返す
	float magnitude() noexcept {
		float magnitude = sqrtf(x * x + y * y + z * z);
		return magnitude;
	}
};

