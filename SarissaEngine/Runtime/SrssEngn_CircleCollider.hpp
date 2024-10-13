﻿#pragma once

#include "SrssEngn_Component.hpp"

// 円形コライダー
class CircleCollider final : public Component {
protected:
	float radius_ = 100;
public:

	const bool IsIntersectedWith(const CircleCollider* other);
	inline float GetRadius() { return radius_; }
	inline void const SetRadius(float r) { radius_ = r; }

	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
	void Draw_() override;
};

