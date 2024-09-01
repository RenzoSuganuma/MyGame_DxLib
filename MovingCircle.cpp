#include "SarissaEngine\Runtime\SrssEngn_ActorUtilities.hpp"
#include "SarissaEngine\Runtime\SrssEngn_CircleCollider.hpp"
#include "SarissaEngine\Engine\SrssEngn_WindowHandler.hpp"
#include "MovingCircle.h"
#include "DxLib.h"

void MovingCircle::Begin_()
{
	name_ = "_OBTSACLE_";
	auto c =  ActorUtilities::GetComponent< CircleCollider* >(this);
	c->SetRadius(r);

	using namespace SarissaEngine::Runtime::System;
	position_.x = windowWidth_ * (3.0f / 4.0f);
	position_.y = windowHeigth_ / 2;
}

void MovingCircle::Tick_(float deltaTime)
{
	// DrawCircle( position_.x, position_.y, r * 1.4f, GetColor(0,0,255));
	// DrawLine(position_.x, position_.y, position_.x + r, position_.y + r, -1);
}

void MovingCircle::End_()
{

}

