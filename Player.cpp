#include "SarissaEngine\Engine\SrssEngn_WindowHandler.hpp"
#include "SarissaEngine\Engine\SrssEngn_SoundHandler.hpp"
#include "SarissaEngine\Runtime\SrssEngn_InputHandler.hpp"
#include "SarissaEngine\Runtime\SrssEngn_ActorUtilities.hpp"
#include "typeinfo"
#include "Player.h"
#include "DxLib.h"

#include "MovingCircle.h"

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
	// DrawCircle(position_.x, position_.y, r * 1.4f, color_);
	if (elapsedTime > 1.0f)
	{
		color_ = GetColor(0, 255, 0);
		using namespace SarissaEngine::Runtime::System;
		//PlaySoundFromMemory(*soundHandlers.begin(), SoundPlayMode::BackGround);
		elapsedTime = 0;
	}
	else
	{
		color_ = GetColor(255, 0, 0);
	}

	auto input = ActorUtilities::GetComponent<InputHandler*>(this);

	position_.x += input->moveVec_.x * 1000 * deltaTime;
	position_.y += input->moveVec_.y * 1000 * deltaTime;

	auto condition = input->GetInputStarted(KeyboardKey::Space);

	using namespace SarissaEngine::Runtime::System;
	if(condition)
		PlaySoundFromMemory(*soundHandlers.begin(), SoundPlayMode::BackGround);

	DrawFormatString(position_.x, position_.y, -1, "%s", name_.c_str());
}

void Player::End_()
{

}

void Player::OnBeginOverlap_(const CircleCollider* other)
{

}

void Player::OnStillOverlap_(const CircleCollider* other)
{
	DrawFormatString(position_.x, position_.y + 10, -1, "OverLapping");
}

void Player::OnEndOverlap_(const CircleCollider* other)
{

}
