#pragma once

#include "SarissaEngine\Runtime\SrssEngn_RuntimeClasses.hpp"

class InputHandler : public SarissaEngine::Runtime::Framework::Component
{
private:
	int mouse_L_pressing_frame_count = 0;
public:
	VECTOR moveVec_{0};
	bool mouseLClick;
	void Begin_() override;
	void Tick_(float deltaTime)  override;
	void End_()override;
};

