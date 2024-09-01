#pragma once

#include "SrssEngn_Component.hpp"

// �~�`�R���C�_�[
class CircleCollider final : public Component
{
protected:
	float radius_ = 100.0f;
public:

	const bool IsIntersectedWith(const CircleCollider* other);
	float GetRadius() { return radius_; }
	void const SetRadius(float r) { radius_ = r; }

	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
};

