#pragma once

#include "SarissaEngine\Runtime\SrssEngn_RuntimeClasses.h"

class Player : public SarissaEngine::Runtime::Framework::Actor
{
private:
	unsigned int color_ = GetColor(255, 0, 0);
	float elapsedTime = 0;
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
};

