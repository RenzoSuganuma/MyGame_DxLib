#include "SrssEngn_ActorUtilities.hpp"
#include "SrssEngn_CircleCollider.hpp"
#include "SrssEngn_Actor.hpp"
#include "SrssEngn_Level.hpp"
#include "DxLib.h"

Level::Level() {}
Level::~Level() {}

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
		(*itr)->Draw();
		itr++;
	}
}

inline void const DetectCollisions(std::list< CircleCollider* > colliders)
{
	for (auto item1 : colliders) // (i)
	{
		// (i) のitem以外のコライダーのリスト
		auto list = colliders;
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
		auto c = ActorUtilities::GetComponent<CircleCollider*>((*itr).get());
		if (c != nullptr)
			colliders.emplace_back(c);

		itr++;
	}
	DetectCollisions(colliders);
}

void const Level::MainLoopExit()
{
	auto it = objects_.begin();

	//while (it != objects_.end())
	//{
	//	(*it).reset();
	//	++it;
	//}

	//objects_.clear();
}

const std::list< std::shared_ptr< Actor > >::iterator
const Level::AddObject(const Actor* newObject)
{
	objects_.emplace_back(const_cast<Actor*>(newObject));
	auto it = objects_.end();
	it--;
	return it;
}

void const Level::RemoveObject(const Actor* obj)
{
	auto itr = objects_.begin();
	while (itr != objects_.end())
	{
		if ((*itr).get() == const_cast<Actor*>(obj))
		{
			objects_.erase(itr);
		}

		itr++;
	}
}

void const Level::RemoveObject(const std::list< std::shared_ptr< Actor > >::iterator place)
{
	objects_.erase(place);
}
