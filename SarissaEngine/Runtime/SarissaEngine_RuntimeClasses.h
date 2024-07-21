#pragma once

#include "list"
#include "string"
#include "DxLib.h"

namespace SarissaEngine::Runtime::Framework
{

	class Component;

	class Actor
	{
	protected:
		VECTOR position_{ 0 }, rotation_{ 0 };
		std::list< Component > attachedComponents_;

	public:
		std::string name_ = " Actor_ ";
		bool enabled_ = true;

		Actor();
		~Actor();

		virtual void Begin();
		virtual void Tick(float deltaTime);
		virtual void End();

		std::list< Component >::iterator
			const
			AddComponent(Component component);
		void const
			RemoveComponent(std::list< Component >::iterator place);
	};

}

namespace SarissaEngine::Runtime::Framework
{

	class Actor;

	class Component
	{
	protected:
		const Actor* attachedActor_ = nullptr;

	public:
		bool enabled_ = true;

		Component();
		~Component();

		virtual void Begin();
		virtual void Tick(float deltaTime);
		virtual void End();

		const Actor*
			const
			GetActor() const;
		void
			const
			SetActor(const Actor* actor);
	};

}

