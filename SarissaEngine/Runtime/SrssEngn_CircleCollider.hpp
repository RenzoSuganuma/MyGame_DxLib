#pragma once

#include "SrssEngn_Component.hpp"

// 円形コライダー
class CircleCollider final : public Component
{
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
};

