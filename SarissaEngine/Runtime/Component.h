#pragma once

#include "Actor.h"

namespace Framework
{
	class Component
	{
	protected:
		bool enabled_;
		Actor* attachedActor;

	public:
		Component();
		~Component();

		virtual void Begin();
		virtual void Tick();
		virtual void End();
	};
}

