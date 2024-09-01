#pragma once

#include "SarissaEngine\Runtime\SrssEngn_Actor.hpp"

class MovingCircle final : public Actor
{
private:
	float r = 100;
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;

};

