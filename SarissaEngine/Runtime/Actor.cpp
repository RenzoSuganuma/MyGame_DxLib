#include "Actor.h"
#include "DxLib.h"
#include "Component.h"

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
	if(actor_debug)
	DrawString(0, 10, " Abegin ", GetColor(255, 255, 255));

	auto it = attachedComponents_.begin();
	while (it != attachedComponents_.end())
	{
		it->Begin();
		it++;
	}

}

void Actor::Tick()
{
	if(actor_debug)
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
	if(actor_debug)
	DrawString(0, 10, " Aend ", GetColor(255, 255, 255));

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

