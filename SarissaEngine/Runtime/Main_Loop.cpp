#include "Main_Loop.h"

using MainLoop::Main_Loop;

Main_Loop::Main_Loop() {}
Main_Loop::~Main_Loop() {}

void const Main_Loop::MainLoopEntry()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->Init();
		itr++;
	}
}

void const Main_Loop::MainLoopUpdate()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->Update();
		itr++;
	}
}

void const Main_Loop::MainLoopExit()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->Finalize();
		itr++;
	}
}

const std::list< Framework::Super* >::iterator
const Main_Loop::AddObject(Framework::Super* newObj)
{
	objects_.emplace_back(newObj);
	auto it = objects_.end();
	it--;
	return it;
}

void const Main_Loop::RemoveObject(Framework::Super* obj)
{
	objects_.remove(obj);
}

void const Main_Loop::RemoveObject(const std::list< Framework::Super* >::iterator place)
{
	objects_.erase(place);
}
