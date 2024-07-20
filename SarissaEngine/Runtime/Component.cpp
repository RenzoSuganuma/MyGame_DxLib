#include "Actor.h"
#include "DxLib.h"
#include "Component.h"

Component::Component()
{
	enabled_ = true;
}

Component::~Component()
{
	attachedActor_ = nullptr;
	enabled_ = false;
}

void Component::Begin()
{
}

void Component::Tick()
{
	if (comp_debug)
		DrawString(0, 50, " Ctick ", GetColor(255, 255, 255));
}

void Component::End()
{
}

const Actor* const Component::GetActor() const
{
	return attachedActor_;
}

void const Component::SetActor(const Actor* actor)
{
	attachedActor_ = actor;
}

