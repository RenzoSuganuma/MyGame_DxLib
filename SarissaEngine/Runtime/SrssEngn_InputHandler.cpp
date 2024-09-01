#include "string"
#include "DxLib.h"
#include "SrssEngn_InputHandler.hpp"

const bool const InputHandler::GetInput(int inputAction, InputDeviceType inputType)
{
	bool ret = 0;
	switch (inputType)
	{
	case InputDeviceType::Mouse:
		ret = GetMouseInput() == inputAction;
		break;
	case InputDeviceType::KeyBoard:
		ret = CheckHitKey(inputAction);
		break;
	case InputDeviceType::GamePad:
		break;
	case InputDeviceType::XInput:
		break;
	}

	return ret;
}

void const InputHandler::CheckKeyboardInput()
{
	performed_[static_cast<int>(KeyboardKey::A)] = CheckHitKey(KEY_INPUT_A) == 1;
	performed_[static_cast<int>(KeyboardKey::B)] = CheckHitKey(KEY_INPUT_B) == 1;
	performed_[static_cast<int>(KeyboardKey::C)] = CheckHitKey(KEY_INPUT_C) == 1;
	performed_[static_cast<int>(KeyboardKey::D)] = CheckHitKey(KEY_INPUT_D) == 1;
	performed_[static_cast<int>(KeyboardKey::E)] = CheckHitKey(KEY_INPUT_E) == 1;
	performed_[static_cast<int>(KeyboardKey::F)] = CheckHitKey(KEY_INPUT_F) == 1;
	performed_[static_cast<int>(KeyboardKey::G)] = CheckHitKey(KEY_INPUT_G) == 1;
	performed_[static_cast<int>(KeyboardKey::H)] = CheckHitKey(KEY_INPUT_H) == 1;
	performed_[static_cast<int>(KeyboardKey::I)] = CheckHitKey(KEY_INPUT_I) == 1;
	performed_[static_cast<int>(KeyboardKey::J)] = CheckHitKey(KEY_INPUT_J) == 1;
	performed_[static_cast<int>(KeyboardKey::K)] = CheckHitKey(KEY_INPUT_K) == 1;
	performed_[static_cast<int>(KeyboardKey::L)] = CheckHitKey(KEY_INPUT_L) == 1;
	performed_[static_cast<int>(KeyboardKey::M)] = CheckHitKey(KEY_INPUT_M) == 1;
	performed_[static_cast<int>(KeyboardKey::N)] = CheckHitKey(KEY_INPUT_N) == 1;
	performed_[static_cast<int>(KeyboardKey::O)] = CheckHitKey(KEY_INPUT_O) == 1;
	performed_[static_cast<int>(KeyboardKey::P)] = CheckHitKey(KEY_INPUT_P) == 1;
	performed_[static_cast<int>(KeyboardKey::Q)] = CheckHitKey(KEY_INPUT_Q) == 1;
	performed_[static_cast<int>(KeyboardKey::R)] = CheckHitKey(KEY_INPUT_R) == 1;
	performed_[static_cast<int>(KeyboardKey::S)] = CheckHitKey(KEY_INPUT_S) == 1;
	performed_[static_cast<int>(KeyboardKey::T)] = CheckHitKey(KEY_INPUT_T) == 1;
	performed_[static_cast<int>(KeyboardKey::U)] = CheckHitKey(KEY_INPUT_U) == 1;
	performed_[static_cast<int>(KeyboardKey::V)] = CheckHitKey(KEY_INPUT_V) == 1;
	performed_[static_cast<int>(KeyboardKey::W)] = CheckHitKey(KEY_INPUT_W) == 1;
	performed_[static_cast<int>(KeyboardKey::X)] = CheckHitKey(KEY_INPUT_X) == 1;
	performed_[static_cast<int>(KeyboardKey::Y)] = CheckHitKey(KEY_INPUT_Y) == 1;
	performed_[static_cast<int>(KeyboardKey::Z)] = CheckHitKey(KEY_INPUT_Z) == 1;

	performed_[static_cast<int>(KeyboardKey::Num_1)] = CheckHitKey(KEY_INPUT_1) == 1;
	performed_[static_cast<int>(KeyboardKey::Num_2)] = CheckHitKey(KEY_INPUT_2) == 1;
	performed_[static_cast<int>(KeyboardKey::Num_3)] = CheckHitKey(KEY_INPUT_3) == 1;
	performed_[static_cast<int>(KeyboardKey::Num_4)] = CheckHitKey(KEY_INPUT_4) == 1;
	performed_[static_cast<int>(KeyboardKey::Num_5)] = CheckHitKey(KEY_INPUT_5) == 1;
	performed_[static_cast<int>(KeyboardKey::Num_6)] = CheckHitKey(KEY_INPUT_6) == 1;
	performed_[static_cast<int>(KeyboardKey::Num_7)] = CheckHitKey(KEY_INPUT_7) == 1;
	performed_[static_cast<int>(KeyboardKey::Num_8)] = CheckHitKey(KEY_INPUT_8) == 1;
	performed_[static_cast<int>(KeyboardKey::Num_9)] = CheckHitKey(KEY_INPUT_9) == 1;
	performed_[static_cast<int>(KeyboardKey::Num_0)] = CheckHitKey(KEY_INPUT_0) == 1;

	performed_[static_cast<int>(KeyboardKey::Shift)] = CheckHitKey(KEY_INPUT_LSHIFT) || CheckHitKey(KEY_INPUT_RSHIFT) == 1;
	performed_[static_cast<int>(KeyboardKey::Tab)] = CheckHitKey(KEY_INPUT_TAB);
	performed_[static_cast<int>(KeyboardKey::Ctrl)] = CheckHitKey(KEY_INPUT_LCONTROL) || CheckHitKey(KEY_INPUT_RCONTROL) == 1;
	performed_[static_cast<int>(KeyboardKey::Alt)] = CheckHitKey(KEY_INPUT_LALT) || CheckHitKey(KEY_INPUT_RALT) == 1;
	performed_[static_cast<int>(KeyboardKey::Space)] = CheckHitKey(KEY_INPUT_SPACE) == 1;
}
// 入力の 立ち上がり、立下りの入力を更新する  
void const InputHandler::CheckInputStarted() {
	size_t lim = sizeof(performed_) / sizeof(bool);
	for (size_t i = 0; i < lim; i++)
	{
		if (performed_[i]) {
			performedFrames_[i]++;

			if (performedFrames_[i] < 2) {
				started_[i] = 1;
			}
			else {
				started_[i] = 0;
			}
		}
	}
}
// 入力の立下りを調べて状態の更新をする	
void const InputHandler::CheckInputCanceled() {
	size_t lim = sizeof(performed_) / sizeof(bool);
	for (size_t i = 0; i < lim; i++)
	{
		if (performed_[i] == false && performedFrames_[i] > 2) {
			canceled_[i] = true;

			performedFrames_[i] = 0;
		}
		else {
			canceled_[i] = false;
		}
	}
}

const bool const InputHandler::GetInputStarted(const KeyboardKey key)
{
	return started_[static_cast<int>(key)];
}

const bool const InputHandler::GetInputPerformed(const KeyboardKey key)
{
	return performed_[static_cast<int>(key)];
}

const bool const InputHandler::GetInputCanceled(const KeyboardKey key)
{
	return canceled_[static_cast<int>(key)];
}

void InputHandler::Begin_()
{
}

void InputHandler::Tick_(float deltaTime)
{
#pragma region -移動軸入力-
	if (CheckHitKey(KEY_INPUT_A) == 1)	// left
	{
		moveVec_.x = -1;
	}
	else if (moveVec_.x == -1)
	{
		moveVec_.x = 0;
	}

	if (CheckHitKey(KEY_INPUT_D) == 1)	// right
	{
		moveVec_.x = 1;
	}
	else if (moveVec_.x == 1)
	{
		moveVec_.x = 0;
	}

	if (CheckHitKey(KEY_INPUT_W) == 1)	// upward
	{
		moveVec_.y = -1;
	}
	else if (moveVec_.y == -1)
	{
		moveVec_.y = 0;
	}

	if (CheckHitKey(KEY_INPUT_S) == 1)	// down
	{
		moveVec_.y = 1;
	}
	else if (moveVec_.y == 1)
	{
		moveVec_.y = 0;
	}
#pragma endregion

	CheckKeyboardInput();
	CheckInputStarted();
	CheckInputCanceled();
}

void InputHandler::End_()
{
}
