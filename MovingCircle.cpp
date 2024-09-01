#include "SarissaEngine\Runtime\SrssEngn_ActorUtilities.hpp"
#include "SarissaEngine\Runtime\SrssEngn_CircleCollider.hpp"
#include "SarissaEngine\Engine\SrssEngn_WindowHandler.hpp"
#include "MovingCircle.h"
#include "DxLib.h"

void MovingCircle::Begin_()
{
	name_ = "_OBTSACLE_";
	auto c = ActorUtilities::GetComponent< CircleCollider* >(this); 
	c->SetRadius(r);

	using namespace SarissaEngine::Runtime::System;
	position_.x = windowWidth_ * (3.0f / 4.0f);
	position_.y = windowHeigth_ / 2;
}

void MovingCircle::Tick_(float deltaTime)
{
}

void MovingCircle::End_()
{

}

