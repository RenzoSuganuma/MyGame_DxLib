#pragma once

#include "list"
#include "DxLib.h"

class Component;

constexpr bool actor_debug = true;

class Actor
{
protected:
	VECTOR position_{ 0 }, rotation_{ 0 };
	std::list< Component > attachedComponents_;

public:
	bool enabled_ = true;

	Actor();
	~Actor();

	virtual void Begin();
	virtual void Tick();
	virtual void End();

	std::list< Component >::iterator
		const
		AddComponent(Component component);
	void const
		RemoveComponent(std::list< Component >::iterator place);
};

