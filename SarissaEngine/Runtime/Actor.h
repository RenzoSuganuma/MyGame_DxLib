#pragma once

#include "Super.h"
#include "DxLib.h"

namespace Framework
{
	class Actor : private Super
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
}

