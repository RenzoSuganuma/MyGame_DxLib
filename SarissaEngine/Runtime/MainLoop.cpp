#include "Actor.h"
#include "MainLoop.h"

MainLoop::MainLoop()
{
}

MainLoop::~MainLoop() 
{
	objects_.clear();
}

void const MainLoop::MainLoopEntry()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->Begin();
		itr++;
	}
}

void const MainLoop::MainLoopUpdate()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->Tick();
		itr++;
	}
}

void const MainLoop::MainLoopExit()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->End();
		itr++;
	}
}

const std::list< Actor* >::iterator
const MainLoop::AddObject(Actor* newObj)
{
	objects_.emplace_back(newObj);
	auto it = objects_.end();
	it--;
	return it;
}

void const MainLoop::RemoveObject(Actor* obj)
{
	objects_.remove(obj);
}

void const MainLoop::RemoveObject(const std::list< Actor* >::iterator place)
{
	objects_.erase(place);
}
