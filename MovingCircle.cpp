#include "SarissaEngine\Runtime\SrssEngn_ActorUtilities.hpp"
#include "SarissaEngine\Runtime\SrssEngn_CircleCollider.hpp"
#include "SarissaEngine\Engine\SrssEngn_WindowHandler.hpp"
#include "MovingCircle.h"
#include "DxLib.h"

void MovingCircle::Begin_()
{
	name_ = "_OBTSACLE_";

	using namespace SarissaEngine::Runtime::System;
	position_.x = windowWidth_ * (3.0f / 4.0f);
	position_.y = windowHeigth_ * .5f;
}

void MovingCircle::Tick_(float deltaTime)
{
	ActorUtilities::GetComponent< CircleCollider* >(this)->SetRadius(100);
}

void MovingCircle::End_()
{
}

void MovingCircle::Draw_()
{
	DrawCircle(position_.x, position_.y,
		ActorUtilities::GetComponent< CircleCollider* >(this)->GetRadius(), c);
}

void MovingCircle::OnBeginOverlap_(const CircleCollider* other)
{
}

void MovingCircle::OnStillOverlap_(const CircleCollider* other)
{
	c = GetColor(128, 0, 128);
}

void MovingCircle::OnEndOverlap_(const CircleCollider* other)
{
}

