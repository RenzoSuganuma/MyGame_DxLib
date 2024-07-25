#include "DxLib.h"
#include "SrssEngn_RuntimeClasses.h"

using SarissaEngine::Runtime::Framework::Actor;
using SarissaEngine::Runtime::Framework::Component;

Component::Component()
{
	enabled_ = true;
}

Component::~Component()
{
	attachedActor_ = nullptr;
	enabled_ = false;
}

void Component::Begin_()
{

}

void Component::Tick_(float deltaTime)
{

}

void Component::End_()
{

}

void Component::Begin()
{
	Begin_();
}

void Component::Tick(float deltaTime)
{
	Tick_(deltaTime);
}

void Component::End()
{
	End_();
}

const Actor* const Component::GetActor() const
{
	return attachedActor_;
}

void const Component::SetActor(const Actor* actor)
{
	attachedActor_ = actor;
}

