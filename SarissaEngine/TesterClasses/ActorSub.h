#pragma once

#include "..\Runtime\Actor.h"

class ActorSub : public Framework::Actor
{
public:
	ActorSub();
	~ActorSub();

	void Begin() override;

	void Tick() override;

	void End() override;
};

