#pragma once

#include "SrssEngn_Component.hpp"

// �~�`�R���C�_�[
class CircleCollider final : public Component
{
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
};

