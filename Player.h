#pragma once

#include "SarissaEngine\Runtime\SrssEngn_Actor.hpp"

class Player final : public Actor
{
private:
	unsigned int color_ = GetColor(255, 0, 0);
	float elapsedTime = 0;
	bool stay_ = false;
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;

	void OnBeginOverlap_(const CircleCollider* other) override;
	void OnStillOverlap_(const CircleCollider* other) override;
	void OnEndOverlap_(const CircleCollider* other) override;
};

