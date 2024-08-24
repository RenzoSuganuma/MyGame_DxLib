#include "DxLib.h"
#include "typeinfo"
#include "SrssEngn_Actor.hpp"
#include "SrssEngn_Component.hpp"
#include "SrssEngn_Level.hpp"

Actor::Actor()
{
	enabled_ = true;
}

Actor::~Actor()
{
	enabled_ = false;

	auto it = attachedComponents_.begin();

	while (it != attachedComponents_.end())
	{
		delete (*it);
		++it;
	}

	attachedComponents_.clear();
	placedLevel_ = nullptr;
}

void Actor::Begin_() {}
void Actor::Tick_(float deltaTime) {}
void Actor::End_() {}

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

void Actor::OnBeginOverlap_(const Collider* other) {}
void Actor::OnStillOverlap_(const Collider* other) {}
void Actor::OnEndOverlap_(const Collider* other) {}

void Actor::OnBeginOverlap(const Collider* other)
{
	OnBeginOverlap_(other);
}

void Actor::OnStillOverlap(const Collider* other)
{
	OnStillOverlap_(other);
}

void Actor::OnEndOverlap(const Collider* other)
{
	OnEndOverlap_(other);
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

void const Actor::SetPlacedLevel(Level* level)
{
	placedLevel_ = level;
}

