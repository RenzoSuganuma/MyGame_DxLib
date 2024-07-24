#include "SarissaEngine\Globals.h"
#include "InputHandler.h"
#include "Player.h"
#include "DxLib.h"

void Player::Begin_()
{
	name_ = "Player";
}

void Player::Tick_(float deltaTime)
{
	elapsedTime += deltaTime;
	DrawCircle(windowWidth_ / 2, windowHeigth_ / 2, 100, color_);
	if (elapsedTime > 1.0f)
	{
		PlaySoundMem(pigeon_se, DX_PLAYTYPE_BACK);
		color_ = GetColor(0, 255, 0);
		elapsedTime = 0;
	}
	else
	{
		color_ = GetColor(255, 0, 0);
	}

	auto input = static_cast<InputHandler*>(*(attachedComponents_.begin()));

	DrawFormatString(0, 0, -1, "%d", input->moveVec_.x);
}

void Player::End_()
{

}
