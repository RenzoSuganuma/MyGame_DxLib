#pragma once

#include "SrssEngn_Collider.hpp"

// 円形コライダー
class CircleCollider final : public Collider
{
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
};

