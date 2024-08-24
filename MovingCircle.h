#pragma once

#include "SarissaEngine\Runtime\SrssEngn_Actor.hpp"

class MovingCircle final : public Actor
{
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;

};

