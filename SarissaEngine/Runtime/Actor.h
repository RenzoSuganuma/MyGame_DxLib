#pragma once

#include "list"
#include "DxLib.h"
#include "Component.h"

namespace Framework
{
	class Actor
	{
	protected:
		std::list< Framework::Component* > attachedComponents_;
		VECTOR position_{ 0 }, rotation_{ 0 };
		bool enabled_;

	public:
		Actor();
		~Actor();

		virtual void Begin();
		virtual void Tick();
		virtual void End();
	};
}

