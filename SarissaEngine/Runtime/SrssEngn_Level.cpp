#include "SrssEngn_ActorUtilities.hpp"
#include "SrssEngn_CircleCollider.hpp"
#include "SrssEngn_Actor.hpp"
#include "SrssEngn_Level.hpp"

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

void const Level::CollisionUpdate()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		// auto c = ActorUtilities::GetComponent< CircleCollider >((*itr));

		itr++;
	}
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
