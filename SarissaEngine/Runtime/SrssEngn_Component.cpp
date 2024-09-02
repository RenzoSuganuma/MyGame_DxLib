#include "DxLib.h"
#include "SrssEngn_Actor.hpp"
#include "SrssEngn_Component.hpp"

Component::Component(){}
Component::~Component(){}

#pragma region virtual-functions
void Component::Begin_(){}
void Component::Tick_(float deltaTime){}
void Component::End_(){}
void Component::Draw_(){}
#pragma endregion

void Component::Begin()
{
	enabled_ = true;

	Begin_();
}

void Component::Tick(float deltaTime)
{
	Tick_(deltaTime);
}

void Component::End()
{
	attachedActor_ = nullptr;
	enabled_ = false;

	End_();
}

void Component::Draw()
{
	Draw_();
}

const Actor* const Component::GetActor() const
{
	return attachedActor_;
}

void const Component::SetActor(const Actor* actor)
{
	attachedActor_ = actor;
}

