#pragma once

#include "SarissaEngine\Runtime\SrssEngn_Actor.hpp"
#include "DxLib.h"

class MovingCircle final : public Actor
{
private:
	unsigned int c = -1;

	void OnBeginOverlap_(const CircleCollider* other) override;
	void OnStillOverlap_(const CircleCollider* other) override;
	void OnEndOverlap_(const CircleCollider* other) override;
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
	void Draw_() override;
};

