#include "DxLib.h"
#include "SrssEngn_InputHandler.hpp"

const bool InputHandler::GetMouseLTrigger()
{
	return mouse_l_trigger;
}

void InputHandler::Begin_()
{

}

void InputHandler::Tick_(float deltaTime)
{
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

	if (GetMouseInput() == MOUSE_INPUT_LEFT)
	{
		mouse_L_pressing_frame_count++;
		if (mouse_L_pressing_frame_count < 2)
		{
			mouse_l_trigger = true;
		}
		else
		{
			mouse_l_trigger = false;
		}
	}
	else
	{
		mouse_L_pressing_frame_count = 0;

		mouse_l_trigger = false;
	}
}

void InputHandler::End_()
{

}
