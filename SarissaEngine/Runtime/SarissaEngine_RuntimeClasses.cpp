#include "SarissaEngine_RuntimeClasses.h"
#include "Actor.h"
#include "Component.h"

using SarissaEngine::Actor;
using SarissaEngine::Component;

Actor::Actor()
{
	enabled_ = true;
}

Actor::~Actor()
{
	enabled_ = false;
	attachedComponents_.clear();
}

void Actor::Begin()
{
	auto it = attachedComponents_.begin();
	while (it != attachedComponents_.end())
	{
		it->Begin();
		it++;
	}

}

void Actor::Tick()
{
	if (actor_debug)
		DrawString(0, 10, " Atick ", GetColor(255, 255, 255));

	auto it = attachedComponents_.begin();
	while (it != attachedComponents_.end())
	{
		it->Tick();
		it++;
	}

}

void Actor::End()
{
	auto it = attachedComponents_.begin();
	while (it != attachedComponents_.end())
	{
		it->End();
		it++;
	}

}

std::list< Component >::iterator
const Actor::AddComponent(Component component)
{
	component.SetActor(this);
	attachedComponents_.emplace_back(component);
	auto it = attachedComponents_.end();
	it--;
	return it;
}

void const Actor::RemoveComponent(std::list< Component >::iterator place)
{
	attachedComponents_.erase(place);
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

