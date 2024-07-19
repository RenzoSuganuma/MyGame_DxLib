#include "Actor.h"
#include "DxLib.h"
#include "Component.h"

Component::Component()
{}
Component::~Component()
{}

void Component::Begin()
{
	if (comp_debug)
		DrawString(0, 50, " Cbegin ", GetColor(255, 255, 255));
}

void Component::Tick()
{
	if (comp_debug)
		DrawString(0, 50, " Ctick ", GetColor(255, 255, 255));
}

void Component::End()
{
	if (comp_debug)
		DrawString(0, 50, " Cend ", GetColor(255, 255, 255));
}

const Actor* const Component::GetActor() const
{
	return attachedActor_;
}

void const Component::SetActor(const Actor* actor)
{
	attachedActor_ = actor;
}

