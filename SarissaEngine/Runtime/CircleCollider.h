#pragma once

#include "DxLib.h"
#include "SrssEngn_Component.hpp"

class CircleCollider final : public Component
{
public:
	float radian_;

	const bool const IsOverlapping(const CircleCollider& other);
	const VECTOR const Center();
};

