#pragma once

#include "SrssEngn_RuntimeClasses.hpp"
#include "DxLib.h"

class InputHandler : public SarissaEngine::Runtime::Framework::Component
{
private:
	int mouse_L_pressing_frame_count = 0;
	bool mouse_l_trigger = false;
public:
	VECTOR moveVec_{0};
	const bool GetMouseLTrigger();
	void Begin_() override;
	void Tick_(float deltaTime)  override;
	void End_()override;
};

