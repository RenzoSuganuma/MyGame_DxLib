#include "DxLib.h"
#include "ActorSub.h"

ActorSub::ActorSub(){}
ActorSub::~ActorSub(){}

void ActorSub::Begin()
{
	DrawString(0, 60, "Begin", GetColor(255, 255, 255));
}

void ActorSub::Tick()
{
	DrawString(0, 60, "Tick", GetColor(255, 255, 255));
}

void ActorSub::End()
{
	DrawString(0, 60, "End", GetColor(255, 255, 255));
}
