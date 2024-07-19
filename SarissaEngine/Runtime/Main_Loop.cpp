#include "Main_Loop.h"

Main_Loop::Main_Loop() {}
Main_Loop::~Main_Loop() {}

void const Main_Loop::MainLoopEntry()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->Begin();
		itr++;
	}
}

void const Main_Loop::MainLoopUpdate()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->Tick();
		itr++;
	}
}

void const Main_Loop::MainLoopExit()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->End();
		itr++;
	}
}

const std::list< Actor* >::iterator
const Main_Loop::AddObject(Actor* newObj)
{
	objects_.emplace_back(newObj);
	auto it = objects_.end();
	it--;
	return it;
}

void const Main_Loop::RemoveObject(Actor* obj)
{
	objects_.remove(obj);
}

void const Main_Loop::RemoveObject(const std::list< Actor* >::iterator place)
{
	objects_.erase(place);
}
