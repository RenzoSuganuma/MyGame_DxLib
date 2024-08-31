#pragma once

#include "SrssEngn_Component.hpp"

// 円形コライダー
class CircleCollider final : public Component
{
public:
	float radius_ = 100.0f;

	const bool IsIntersectedWith(const CircleCollider* other);

	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
};

