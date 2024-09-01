#pragma once

// ３次元ベクトルクラス
struct Vector3
{
public:
	float x, y, z;

	Vector3() {
		Vector3(0, 0, 0);
	}
	Vector3(float x, float y, float z) : x(this->x), y(this->y), z(this->z) {}
};

