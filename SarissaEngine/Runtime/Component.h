#pragma once

#include "Super.h"

namespace Framework
{
	class Component : private Super
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
}

