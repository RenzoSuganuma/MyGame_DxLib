#include "SrssEngn_ActorUtilities.hpp"
#include "SrssEngn_CircleCollider.hpp"
#include "SrssEngn_Actor.hpp"
#include "SrssEngn_Level.hpp"
#include "DxLib.h"

Level::Level()
{
}

Level::~Level()
{
	auto it = objects_.begin();

	while (it != objects_.end())
	{
		delete (*it);
		++it;
	}

	objects_.clear();
}

void const Level::MainLoopEntry()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->Begin();
		itr++;
	}
}

void const Level::MainLoopUpdate(float deltaTime)
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->Tick(deltaTime);
		itr++;
	}
}

inline void const Task(std::list< CircleCollider* > colliders)
{
	for (auto item1 : colliders) // i
	{
		auto list = colliders;
		// i のitem以外のコライダーのリスト
		list.remove(item1);

		for (auto item2 : list)
		{
			auto a1 = const_cast<Actor*>(item1->GetActor());
			auto a2 = const_cast<Actor*>(item2->GetActor());

			if (item1->IsIntersectedWith(item2) || item2->IsIntersectedWith(item1))
			{
				a1->OnStillOverlap(item2);
				a2->OnStillOverlap(item1);
			}
		}
	}
}

void const Level::CollisionUpdate()
{
	auto itr = objects_.begin();
	std::list< CircleCollider* > colliders;

	while (itr != objects_.end())
	{
		auto c = ActorUtilities::GetComponent<CircleCollider*>(*itr);
		if (c != nullptr)
			colliders.emplace_back(c);

		itr++;
	}
	Task(colliders);
}

void const Level::MainLoopExit()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->End();
		itr++;
	}
}

const std::list< Actor* >::iterator
const Level::AddObject(Actor* newObj)
{
	objects_.emplace_back(newObj);
	auto it = objects_.end();
	it--;
	return it;
}

void const Level::RemoveObject(Actor* obj)
{
	objects_.remove(obj);
}

void const Level::RemoveObject(const std::list< Actor* >::iterator place)
{
	objects_.erase(place);
}
