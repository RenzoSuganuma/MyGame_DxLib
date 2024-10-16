#include "SarissaEngine\Engine\SrssEngn_WindowHandler.hpp"
#include "SarissaEngine\Engine\SrssEngn_SoundHandler.hpp"
#include "SarissaEngine\Runtime\SrssEngn_InputHandler.hpp"
#include "SarissaEngine\Runtime\SrssEngn_ActorUtilities.hpp"
#include "SarissaEngine\Runtime\SrssEngn_CircleCollider.hpp"
#include "SarissaEngine\Runtime\SrssEngn_Actor.hpp"
#include "SarissaEngine\Runtime\SrssEngn_Vector3.h"
#include "Player.h"
#include "DxLib.h"

#include "MovingCircle.h"

void Player::Begin_()
{
	using namespace SarissaEngine::Runtime::System;
	name_ = "Player";
	position_.x = windowWidth_ * .5f;
	position_.y = windowHeigth_ * .5f;
}

void Player::Tick_(float deltaTime)
{
	ActorUtilities::GetComponent< CircleCollider* >(this)->SetRadius(100);

	elapsedTime += deltaTime;

	auto input = ActorUtilities::GetComponent<InputHandler*>(this);

	position_.x += input->moveVec_.x * 1000 * deltaTime;
	position_.y += input->moveVec_.y * 1000 * deltaTime;

	auto condition = input->GetInputCanceled(KeyboardKey::Space);

	using namespace SarissaEngine::Runtime::System;
	if (condition)
		PlaySoundFromMemory(*soundHandlers.begin(), SoundPlayMode::BackGround);

	DrawFormatString(position_.x, position_.y, -1, "%s", name_.c_str());
}

void Player::End_()
{

}

void Player::Draw_()
{
	DrawCircle(position_.x, position_.y,
		ActorUtilities::GetComponent<CircleCollider*>(this)->GetRadius(), color_);
}

void Player::OnBeginOverlap_(const CircleCollider* other)
{

}

void Player::OnStillOverlap_(const CircleCollider* other)
{
	DrawFormatString(position_.x, position_.y + 10, -1, "OverLapping");
	color_ = GetColor(0, 255, 0);
}

void Player::OnEndOverlap_(const CircleCollider* other)
{

}
