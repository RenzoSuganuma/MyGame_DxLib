#include "SarissaEngine\Globals.h"
#include "InputHandler.h"
#include "DxLib.h"

void InputHandler::Begin_()
{

}

void InputHandler::Tick_(float deltaTime)
{
	if (CheckHitKey(KEY_INPUT_A) == 1)	// left
	{
		moveVec_.x = -1;
	}

	if (CheckHitKey(KEY_INPUT_D) == 1)	// right
	{
		moveVec_.x = 1;
	}

	if (CheckHitKey(KEY_INPUT_W) == 1)	// upward
	{
		moveVec_.y = -1;
	}

	if (CheckHitKey(KEY_INPUT_S) == 1)	// down
	{
		moveVec_.y = 1;
	}	
}

void InputHandler::End_()
{

}
