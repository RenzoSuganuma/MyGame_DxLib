#pragma once

#include "list"
#include "DxLib.h"

class Component;

#define ACTOR_DEBUG true;

class Actor
{
protected:
	VECTOR position_{ 0 }, rotation_{ 0 };
	std::list< Component > attachedComponents_;
	bool enabled_;

public:
	Actor();
	~Actor();

	virtual void Begin();
	virtual void Tick();
	virtual void End();
};

