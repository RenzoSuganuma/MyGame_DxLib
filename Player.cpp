#include "SarissaEngine\Engine\SrssEngn_WindowHandler.hpp"
#include "InputHandler.h"
#include "Player.h"
#include "DxLib.h"

#include "MovingCircle.h"

void Player::Begin_()
{
	using namespace SarissaEngine::Runtime::System;
	name_ = "Player";
	position_.x = windowWidth_ / 2;
	position_.y = windowHeigth_ / 2;

	placedLevel_->AddObject(new MovingCircle);
}

void Player::Tick_(float deltaTime)
{
	elapsedTime += deltaTime;
	DrawCircle(position_.x, position_.y, 100, color_);
	if (elapsedTime > 1.0f)
	{
		color_ = GetColor(0, 255, 0);
		elapsedTime = 0;
	}
	else
	{
		color_ = GetColor(255, 0, 0);
	}

	auto input = static_cast<InputHandler*>(*(attachedComponents_.begin()));

	position_.x += input->moveVec_.x * 1000 * deltaTime;
	position_.y += input->moveVec_.y * 1000 * deltaTime;

	DrawFormatString(position_.x, position_.y, -1, "%s", name_.c_str());
}

void Player::End_()
{

}
