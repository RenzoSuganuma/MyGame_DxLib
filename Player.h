#pragma once

#include "SarissaEngine\Runtime\SarissaEngine_RuntimeClasses.h"

class Player : public SarissaEngine::Runtime::Framework::Actor
{
	void Begin() override ;
	void Tick(float deltaTime) override ;
	void End() override ;
};

