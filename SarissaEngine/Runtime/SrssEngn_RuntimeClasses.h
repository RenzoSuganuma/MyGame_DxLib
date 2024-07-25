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
		std::list< Component* > attachedComponents_;

	public:
		std::string name_ = " Actor_ ";
		bool enabled_ = true;

		Actor();
		~Actor();

		void Begin();
		void Tick(float deltaTime);
		void End();

		virtual void Begin_();
		virtual void Tick_(float deltaTime);
		virtual void End_();

		std::list< Component* >::iterator
			const
			AddComponent(Component* component);

		void const
			RemoveComponent(std::list< Component* >::iterator place);

		void const SetPosition(VECTOR newPos);
		const VECTOR const GetPosition();

		void const SetRotaton(VECTOR newRot);
		const VECTOR const GetRotaton();
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

		void Begin();
		void Tick(float deltaTime);
		void End();

		virtual void Begin_();
		virtual void Tick_(float deltaTime);
		virtual void End_();

		const Actor*
			const
			GetActor() const;
		void
			const
			SetActor(const Actor* actor);
	};

}

