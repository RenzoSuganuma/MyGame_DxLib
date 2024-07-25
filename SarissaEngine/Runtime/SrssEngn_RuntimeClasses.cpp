#include "DxLib.h"
#include "SrssEngn_RuntimeClasses.h"

using SarissaEngine::Runtime::Framework::Actor;
using SarissaEngine::Runtime::Framework::Component;

Actor::Actor()
{
	enabled_ = true;
}

Actor::~Actor()
{
	enabled_ = false;
	attachedComponents_.clear();
}

void Actor::Begin_()
{

}

void Actor::Tick_(float deltaTime)
{

}

void Actor::End_()
{

}

void Actor::Begin()
{
	auto it = attachedComponents_.begin();
	while (it != attachedComponents_.end())
	{
		(*it)->Begin();
		it++;
	}

	Begin_();
}

void Actor::Tick(float deltaTime)
{
	auto it = attachedComponents_.begin();
	while (it != attachedComponents_.end())
	{
		(*it)->Tick(deltaTime);
		it++;
	}

	Tick_(deltaTime);
}

void Actor::End()
{
	auto it = attachedComponents_.begin();
	while (it != attachedComponents_.end())
	{
		(*it)->End();
		it++;
	}

	End_();
}

std::list< Component* >::iterator
const Actor::AddComponent(Component* component)
{
	component->SetActor(this);
	attachedComponents_.emplace_back(component);
	auto it = attachedComponents_.end();
	it--;
	return it;
}

void const Actor::RemoveComponent(std::list< Component* >::iterator place)
{
	attachedComponents_.erase(place);
}

void const Actor::SetPosition(VECTOR newPos)
{
	position_ = newPos;
}

const VECTOR const Actor::GetPosition()
{
	return position_;
}

void const Actor::SetRotaton(VECTOR newRot)
{
	rotation_ = newRot;
}

const VECTOR const Actor::GetRotaton()
{
	return rotation_;
}

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

