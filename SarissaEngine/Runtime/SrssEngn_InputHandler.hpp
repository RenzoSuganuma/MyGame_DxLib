#pragma once

#include "SrssEngn_RuntimeClasses.hpp"
#include "DxLib.h"
#include "list"
#include "map"

enum class InputDeviceType
{
	Mouse,
	KeyBoard,
	GamePad,
	XInput,
};

class InputHandler : public SarissaEngine::Runtime::Framework::Component
{
private:
public:
	VECTOR moveVec_{0};
	const bool const GetInput(int inputAction, InputDeviceType inputType);
	void Begin_() override;
	void Tick_(float deltaTime)  override;
	void End_()override;
};

