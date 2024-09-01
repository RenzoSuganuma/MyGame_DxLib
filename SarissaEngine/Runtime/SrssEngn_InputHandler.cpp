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
	performed_.insert_or_assign(KeyboardKey::A, CheckHitKey(KEY_INPUT_A));
	performed_.insert_or_assign(KeyboardKey::B, CheckHitKey(KEY_INPUT_B));
	performed_.insert_or_assign(KeyboardKey::C, CheckHitKey(KEY_INPUT_C));
	performed_.insert_or_assign(KeyboardKey::D, CheckHitKey(KEY_INPUT_D));
	performed_.insert_or_assign(KeyboardKey::E, CheckHitKey(KEY_INPUT_E));
	performed_.insert_or_assign(KeyboardKey::F, CheckHitKey(KEY_INPUT_F));
	performed_.insert_or_assign(KeyboardKey::G, CheckHitKey(KEY_INPUT_G));
	performed_.insert_or_assign(KeyboardKey::H, CheckHitKey(KEY_INPUT_H));
	performed_.insert_or_assign(KeyboardKey::I, CheckHitKey(KEY_INPUT_I));
	performed_.insert_or_assign(KeyboardKey::J, CheckHitKey(KEY_INPUT_J));
	performed_.insert_or_assign(KeyboardKey::K, CheckHitKey(KEY_INPUT_K));
	performed_.insert_or_assign(KeyboardKey::L, CheckHitKey(KEY_INPUT_L));
	performed_.insert_or_assign(KeyboardKey::M, CheckHitKey(KEY_INPUT_M));
	performed_.insert_or_assign(KeyboardKey::N, CheckHitKey(KEY_INPUT_N));
	performed_.insert_or_assign(KeyboardKey::O, CheckHitKey(KEY_INPUT_O));
	performed_.insert_or_assign(KeyboardKey::P, CheckHitKey(KEY_INPUT_P));
	performed_.insert_or_assign(KeyboardKey::Q, CheckHitKey(KEY_INPUT_Q));
	performed_.insert_or_assign(KeyboardKey::R, CheckHitKey(KEY_INPUT_R));
	performed_.insert_or_assign(KeyboardKey::S, CheckHitKey(KEY_INPUT_S));
	performed_.insert_or_assign(KeyboardKey::T, CheckHitKey(KEY_INPUT_T));
	performed_.insert_or_assign(KeyboardKey::U, CheckHitKey(KEY_INPUT_U));
	performed_.insert_or_assign(KeyboardKey::V, CheckHitKey(KEY_INPUT_V));
	performed_.insert_or_assign(KeyboardKey::W, CheckHitKey(KEY_INPUT_W));
	performed_.insert_or_assign(KeyboardKey::X, CheckHitKey(KEY_INPUT_X));
	performed_.insert_or_assign(KeyboardKey::Y, CheckHitKey(KEY_INPUT_Y));
	performed_.insert_or_assign(KeyboardKey::Z, CheckHitKey(KEY_INPUT_Z));

	performed_.insert_or_assign(KeyboardKey::Num_1, CheckHitKey(KEY_INPUT_1));
	performed_.insert_or_assign(KeyboardKey::Num_2, CheckHitKey(KEY_INPUT_2));
	performed_.insert_or_assign(KeyboardKey::Num_3, CheckHitKey(KEY_INPUT_3));
	performed_.insert_or_assign(KeyboardKey::Num_4, CheckHitKey(KEY_INPUT_4));
	performed_.insert_or_assign(KeyboardKey::Num_5, CheckHitKey(KEY_INPUT_5));
	performed_.insert_or_assign(KeyboardKey::Num_6, CheckHitKey(KEY_INPUT_6));
	performed_.insert_or_assign(KeyboardKey::Num_7, CheckHitKey(KEY_INPUT_7));
	performed_.insert_or_assign(KeyboardKey::Num_8, CheckHitKey(KEY_INPUT_8));
	performed_.insert_or_assign(KeyboardKey::Num_9, CheckHitKey(KEY_INPUT_9));
	performed_.insert_or_assign(KeyboardKey::Num_0, CheckHitKey(KEY_INPUT_0));

	performed_.insert_or_assign(KeyboardKey::Shift, CheckHitKey(KEY_INPUT_LSHIFT) || CheckHitKey(KEY_INPUT_RSHIFT));
	performed_.insert_or_assign(KeyboardKey::Tab, CheckHitKey(KEY_INPUT_TAB));
	performed_.insert_or_assign(KeyboardKey::Ctrl, CheckHitKey(KEY_INPUT_LCONTROL) || CheckHitKey(KEY_INPUT_RCONTROL));
	performed_.insert_or_assign(KeyboardKey::Alt, CheckHitKey(KEY_INPUT_LALT) || CheckHitKey(KEY_INPUT_RALT));
	performed_.insert_or_assign(KeyboardKey::Space, CheckHitKey(KEY_INPUT_SPACE));
}
// 入力の 立ち上がり、立下りの入力を更新する
void const InputHandler::CheckInputStarted() {
	for (auto item : performed_) {
		if (item.second) {
			performedFrames_[item.first]++;

			if (performedFrames_[item.first] < 2) {
				started_[item.first] = true;
			}	// 入力フレーム数を調べる
			else {
				started_[item.first] = false;
			}
		}
	}	// 入力中の情報をすべてなめる
}
// 入力の立下りを調べて状態の更新をする	
void const InputHandler::CheckInputCanceled() {
	auto past_it = performedConditionPastFrame_.begin(); // 前フレーム
	auto current_it = performed_.begin();				 // 現在のフレーム
	while (current_it != performed_.end()) {
		if ((*current_it).second == false && (*past_it).second == true) {
			canceled_[(*current_it).first] = true;

			performedFrames_[(*current_it).first] = 0;
		}
		else {
			canceled_[(*current_it).first] = false;
		}

		past_it++;
		current_it++;
	}
}
// 前フレームの入力値を更新
void const InputHandler::UpdatePastInputValues() {
	performedConditionPastFrame_ = performed_;	// 値をコピー
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
