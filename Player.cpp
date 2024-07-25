#include "SarissaEngine\Engine\SrssEngn_WindowHandler.h"
#include "InputHandler.h"
#include "Player.h"
#include "DxLib.h"

void Player::Begin_()
{
	using namespace SarissaEngine::Runtime::System;
	name_ = "Player";
	position_.x = windowWidth_ / 2;
	position_.y = windowHeigth_ / 2;
}

void Player::Tick_(float deltaTime)
{
	elapsedTime += deltaTime;
	DrawCircle(position_.x, position_.y, 100, color_);
	if (elapsedTime > 1.0f)
	{
		// PlaySoundMem(pigeon_se, DX_PLAYTYPE_BACK);
		color_ = GetColor(0, 255, 0);
		elapsedTime = 0;
	}
	else
	{
		color_ = GetColor(255, 0, 0);
	}

	auto input = static_cast<InputHandler*>(*(attachedComponents_.begin()));

	position_.x += input->moveVec_.x;
	position_.y += input->moveVec_.y;

	DrawFormatString(0, 0, -1, "%.1f, %.1f", input->moveVec_.x, input->moveVec_.y);
}

void Player::End_()
{

}
