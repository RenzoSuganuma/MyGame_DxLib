#pragma once

#include "SarissaEngine\Runtime\SrssEngn_Actor.hpp"

class MovingCircle final : public Actor
{
private:
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;

};

