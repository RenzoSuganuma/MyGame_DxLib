#pragma once

#include "Actor.h"

class Component
{
protected:
	bool enabled_;

public:
	Component();
	~Component();

	virtual void Begin();
	virtual void Tick();
	virtual void End();
};

