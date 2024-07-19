#pragma once

#include "list"
#include "DxLib.h"
#include "Component.h"

class Actor
{
protected:
	VECTOR position_{ 0 }, rotation_{ 0 };
	bool enabled_;

public:
	Actor();
	~Actor();

	virtual void Begin();
	virtual void Tick();
	virtual void End();
};

