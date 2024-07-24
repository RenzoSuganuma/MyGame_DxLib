#include "SarissaEngine\Globals.h"
#include "Player.h"
#include "DxLib.h"

void Player::Begin()
{
	name_ = "Player";
}

void Player::Tick(float deltaTime)
{
	DrawCircle(windowWidth_ / 2, windowHeigth_ / 2, 100, GetColor(255, 255, 0));
}

void Player::End()
{

}
