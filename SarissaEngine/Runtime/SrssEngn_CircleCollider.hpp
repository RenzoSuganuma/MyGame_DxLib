#pragma once

#include "SrssEngn_Collider.hpp"

// �~�`�R���C�_�[
class CircleCollider final : public Collider
{
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
};

