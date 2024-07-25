#pragma once

#include "SarissaEngine/Runtime/SrssEngn_RuntimeClasses.hpp"

class MovingCircle final : public SarissaEngine::Runtime::Framework::Actor
{
public:
	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;

};

