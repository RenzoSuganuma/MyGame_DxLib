#include "MovingCircle.h"
#include "DxLib.h"

void MovingCircle::Begin_()
{

}

void MovingCircle::Tick_(float deltaTime)
{
	position_.x += 10;
	DrawCircle(position_.x, position_.y, -1, 0, 10);
}

void MovingCircle::End_()
{

}

