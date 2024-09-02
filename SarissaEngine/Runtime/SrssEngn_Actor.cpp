#include "DxLib.h"
#include "typeinfo"
#include "SrssEngn_Actor.hpp"
#include "SrssEngn_Component.hpp"
#include "SrssEngn_Level.hpp"
#include "SrssEngn_CircleCollider.hpp"

Actor::Actor() {}
Actor::~Actor() {}

#pragma region virtual-funcitons
void Actor::Begin_() {}
void Actor::Tick_(float deltaTime) {}
void Actor::End_() {}
void Actor::Draw_() {}
void Actor::OnBeginOverlap_(const CircleCollider* other) {}
void Actor::OnStillOverlap_(const CircleCollider* other) {}
void Actor::OnEndOverlap_(const CircleCollider* other) {}
#pragma endregion

void Actor::Begin()
{
	auto it = attachedComponents_.begin();
	while (it != attachedComponents_.end())
	{
		(*it)->Begin();
		it++;
	}

	enabled_ = true;

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

	attachedComponents_.clear();
	enabled_ = false;
	placedLevel_ = nullptr;

	End_();
}

void Actor::Draw()
{
	auto it = attachedComponents_.begin();
	while (it != attachedComponents_.end())
	{
		(*it)->Draw();
		it++;
	}

	Draw_();
}

void Actor::OnBeginOverlap(const CircleCollider* other)
{
	OnBeginOverlap_(other);
}

void Actor::OnStillOverlap(const CircleCollider* other)
{
	OnStillOverlap_(other);
}

void Actor::OnEndOverlap(const CircleCollider* other)
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

void const Actor::RemoveComponent(const std::list<Component*>::iterator place)
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

void const Actor::SetRotation(VECTOR newRot)
{
	rotation_ = newRot;
}

const VECTOR const Actor::GetRotation()
{
	return rotation_;
}

void const Actor::SetPlacedLevel(const Level* level)
{
	placedLevel_ = const_cast<Level*>(level);
}

