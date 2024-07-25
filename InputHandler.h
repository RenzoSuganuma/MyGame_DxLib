#pragma once

#include "SarissaEngine\Runtime\SrssEngn_RuntimeClasses.hpp"

class InputHandler : public SarissaEngine::Runtime::Framework::Component
{
public:
	VECTOR moveVec_{0};
	bool mouseLClick;
	void Begin_() override;
	void Tick_(float deltaTime)  override;
	void End_()override;
};

