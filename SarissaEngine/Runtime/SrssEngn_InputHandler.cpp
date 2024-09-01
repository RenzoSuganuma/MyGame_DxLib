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

const bool const InputHandler::GetInputStarted(const KeyboardKey key)
{
	return started_[key];
}

const bool const InputHandler::GetInputPerformed(const KeyboardKey key)
{
	return performed_[key];
}

const bool const InputHandler::GetInputCanceled(const KeyboardKey key)
{
	return canceled_[key];
}

void InputHandler::Begin_()
{
#pragma region -キーボード入力（立ち上がり）-
	started_.insert_or_assign(KeyboardKey::A, 0);
	started_.insert_or_assign(KeyboardKey::B, 0);
	started_.insert_or_assign(KeyboardKey::C, 0);
	started_.insert_or_assign(KeyboardKey::D, 0);
	started_.insert_or_assign(KeyboardKey::E, 0);
	started_.insert_or_assign(KeyboardKey::F, 0);
	started_.insert_or_assign(KeyboardKey::G, 0);
	started_.insert_or_assign(KeyboardKey::H, 0);
	started_.insert_or_assign(KeyboardKey::I, 0);
	started_.insert_or_assign(KeyboardKey::J, 0);
	started_.insert_or_assign(KeyboardKey::K, 0);
	started_.insert_or_assign(KeyboardKey::L, 0);
	started_.insert_or_assign(KeyboardKey::M, 0);
	started_.insert_or_assign(KeyboardKey::N, 0);
	started_.insert_or_assign(KeyboardKey::O, 0);
	started_.insert_or_assign(KeyboardKey::P, 0);
	started_.insert_or_assign(KeyboardKey::Q, 0);
	started_.insert_or_assign(KeyboardKey::R, 0);
	started_.insert_or_assign(KeyboardKey::S, 0);
	started_.insert_or_assign(KeyboardKey::T, 0);
	started_.insert_or_assign(KeyboardKey::U, 0);
	started_.insert_or_assign(KeyboardKey::V, 0);
	started_.insert_or_assign(KeyboardKey::W, 0);
	started_.insert_or_assign(KeyboardKey::X, 0);
	started_.insert_or_assign(KeyboardKey::Y, 0);
	started_.insert_or_assign(KeyboardKey::Z, 0);

	started_.insert_or_assign(KeyboardKey::Num_1, 0);
	started_.insert_or_assign(KeyboardKey::Num_2, 0);
	started_.insert_or_assign(KeyboardKey::Num_3, 0);
	started_.insert_or_assign(KeyboardKey::Num_4, 0);
	started_.insert_or_assign(KeyboardKey::Num_5, 0);
	started_.insert_or_assign(KeyboardKey::Num_6, 0);
	started_.insert_or_assign(KeyboardKey::Num_7, 0);
	started_.insert_or_assign(KeyboardKey::Num_8, 0);
	started_.insert_or_assign(KeyboardKey::Num_9, 0);
	started_.insert_or_assign(KeyboardKey::Num_0, 0);

	started_.insert_or_assign(KeyboardKey::Tab, 0);
	started_.insert_or_assign(KeyboardKey::Shift, 0);
	started_.insert_or_assign(KeyboardKey::Ctrl, 0);
	started_.insert_or_assign(KeyboardKey::Alt, 0);
	started_.insert_or_assign(KeyboardKey::Space, 0);
#pragma endregion

#pragma region -キーボード入力（入力中）-
	performed_.insert_or_assign(KeyboardKey::A, 0);
	performed_.insert_or_assign(KeyboardKey::B, 0);
	performed_.insert_or_assign(KeyboardKey::C, 0);
	performed_.insert_or_assign(KeyboardKey::D, 0);
	performed_.insert_or_assign(KeyboardKey::E, 0);
	performed_.insert_or_assign(KeyboardKey::F, 0);
	performed_.insert_or_assign(KeyboardKey::G, 0);
	performed_.insert_or_assign(KeyboardKey::H, 0);
	performed_.insert_or_assign(KeyboardKey::I, 0);
	performed_.insert_or_assign(KeyboardKey::J, 0);
	performed_.insert_or_assign(KeyboardKey::K, 0);
	performed_.insert_or_assign(KeyboardKey::L, 0);
	performed_.insert_or_assign(KeyboardKey::M, 0);
	performed_.insert_or_assign(KeyboardKey::N, 0);
	performed_.insert_or_assign(KeyboardKey::O, 0);
	performed_.insert_or_assign(KeyboardKey::P, 0);
	performed_.insert_or_assign(KeyboardKey::Q, 0);
	performed_.insert_or_assign(KeyboardKey::R, 0);
	performed_.insert_or_assign(KeyboardKey::S, 0);
	performed_.insert_or_assign(KeyboardKey::T, 0);
	performed_.insert_or_assign(KeyboardKey::U, 0);
	performed_.insert_or_assign(KeyboardKey::V, 0);
	performed_.insert_or_assign(KeyboardKey::W, 0);
	performed_.insert_or_assign(KeyboardKey::X, 0);
	performed_.insert_or_assign(KeyboardKey::Y, 0);
	performed_.insert_or_assign(KeyboardKey::Z, 0);

	performed_.insert_or_assign(KeyboardKey::Num_1, 0);
	performed_.insert_or_assign(KeyboardKey::Num_2, 0);
	performed_.insert_or_assign(KeyboardKey::Num_3, 0);
	performed_.insert_or_assign(KeyboardKey::Num_4, 0);
	performed_.insert_or_assign(KeyboardKey::Num_5, 0);
	performed_.insert_or_assign(KeyboardKey::Num_6, 0);
	performed_.insert_or_assign(KeyboardKey::Num_7, 0);
	performed_.insert_or_assign(KeyboardKey::Num_8, 0);
	performed_.insert_or_assign(KeyboardKey::Num_9, 0);
	performed_.insert_or_assign(KeyboardKey::Num_0, 0);

	performed_.insert_or_assign(KeyboardKey::Tab, 0);
	performed_.insert_or_assign(KeyboardKey::Shift, 0);
	performed_.insert_or_assign(KeyboardKey::Ctrl, 0);
	performed_.insert_or_assign(KeyboardKey::Alt, 0);
	performed_.insert_or_assign(KeyboardKey::Space, 0);
#pragma endregion

#pragma region -キーボード入力（立下り）-
	canceled_.insert_or_assign(KeyboardKey::A, 0);
	canceled_.insert_or_assign(KeyboardKey::B, 0);
	canceled_.insert_or_assign(KeyboardKey::C, 0);
	canceled_.insert_or_assign(KeyboardKey::D, 0);
	canceled_.insert_or_assign(KeyboardKey::E, 0);
	canceled_.insert_or_assign(KeyboardKey::F, 0);
	canceled_.insert_or_assign(KeyboardKey::G, 0);
	canceled_.insert_or_assign(KeyboardKey::H, 0);
	canceled_.insert_or_assign(KeyboardKey::I, 0);
	canceled_.insert_or_assign(KeyboardKey::J, 0);
	canceled_.insert_or_assign(KeyboardKey::K, 0);
	canceled_.insert_or_assign(KeyboardKey::L, 0);
	canceled_.insert_or_assign(KeyboardKey::M, 0);
	canceled_.insert_or_assign(KeyboardKey::N, 0);
	canceled_.insert_or_assign(KeyboardKey::O, 0);
	canceled_.insert_or_assign(KeyboardKey::P, 0);
	canceled_.insert_or_assign(KeyboardKey::Q, 0);
	canceled_.insert_or_assign(KeyboardKey::R, 0);
	canceled_.insert_or_assign(KeyboardKey::S, 0);
	canceled_.insert_or_assign(KeyboardKey::T, 0);
	canceled_.insert_or_assign(KeyboardKey::U, 0);
	canceled_.insert_or_assign(KeyboardKey::V, 0);
	canceled_.insert_or_assign(KeyboardKey::W, 0);
	canceled_.insert_or_assign(KeyboardKey::X, 0);
	canceled_.insert_or_assign(KeyboardKey::Y, 0);
	canceled_.insert_or_assign(KeyboardKey::Z, 0);

	canceled_.insert_or_assign(KeyboardKey::Num_1, 0);
	canceled_.insert_or_assign(KeyboardKey::Num_2, 0);
	canceled_.insert_or_assign(KeyboardKey::Num_3, 0);
	canceled_.insert_or_assign(KeyboardKey::Num_4, 0);
	canceled_.insert_or_assign(KeyboardKey::Num_5, 0);
	canceled_.insert_or_assign(KeyboardKey::Num_6, 0);
	canceled_.insert_or_assign(KeyboardKey::Num_7, 0);
	canceled_.insert_or_assign(KeyboardKey::Num_8, 0);
	canceled_.insert_or_assign(KeyboardKey::Num_9, 0);
	canceled_.insert_or_assign(KeyboardKey::Num_0, 0);

	canceled_.insert_or_assign(KeyboardKey::Tab, 0);
	canceled_.insert_or_assign(KeyboardKey::Shift, 0);
	canceled_.insert_or_assign(KeyboardKey::Ctrl, 0);
	canceled_.insert_or_assign(KeyboardKey::Alt, 0);
	canceled_.insert_or_assign(KeyboardKey::Space, 0);
#pragma endregion

#pragma region -キーボード入力（入力フレームカウント）-
	performedFrames_.insert_or_assign(KeyboardKey::A, 0);
	performedFrames_.insert_or_assign(KeyboardKey::B, 0);
	performedFrames_.insert_or_assign(KeyboardKey::C, 0);
	performedFrames_.insert_or_assign(KeyboardKey::D, 0);
	performedFrames_.insert_or_assign(KeyboardKey::E, 0);
	performedFrames_.insert_or_assign(KeyboardKey::F, 0);
	performedFrames_.insert_or_assign(KeyboardKey::G, 0);
	performedFrames_.insert_or_assign(KeyboardKey::H, 0);
	performedFrames_.insert_or_assign(KeyboardKey::I, 0);
	performedFrames_.insert_or_assign(KeyboardKey::J, 0);
	performedFrames_.insert_or_assign(KeyboardKey::K, 0);
	performedFrames_.insert_or_assign(KeyboardKey::L, 0);
	performedFrames_.insert_or_assign(KeyboardKey::M, 0);
	performedFrames_.insert_or_assign(KeyboardKey::N, 0);
	performedFrames_.insert_or_assign(KeyboardKey::O, 0);
	performedFrames_.insert_or_assign(KeyboardKey::P, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Q, 0);
	performedFrames_.insert_or_assign(KeyboardKey::R, 0);
	performedFrames_.insert_or_assign(KeyboardKey::S, 0);
	performedFrames_.insert_or_assign(KeyboardKey::T, 0);
	performedFrames_.insert_or_assign(KeyboardKey::U, 0);
	performedFrames_.insert_or_assign(KeyboardKey::V, 0);
	performedFrames_.insert_or_assign(KeyboardKey::W, 0);
	performedFrames_.insert_or_assign(KeyboardKey::X, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Y, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Z, 0);

	performedFrames_.insert_or_assign(KeyboardKey::Num_1, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Num_2, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Num_3, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Num_4, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Num_5, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Num_6, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Num_7, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Num_8, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Num_9, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Num_0, 0);

	performedFrames_.insert_or_assign(KeyboardKey::Tab, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Shift, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Ctrl, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Alt, 0);
	performedFrames_.insert_or_assign(KeyboardKey::Space, 0);
#pragma endregion

	performedConditionPastFrame_ = performed_;
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
	UpdatePastInputValues();
}

void InputHandler::End_()
{

}
