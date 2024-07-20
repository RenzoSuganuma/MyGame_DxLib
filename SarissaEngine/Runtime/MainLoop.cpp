#include "MainLoop.h"

using SarissaEngine::Runtime::System::MainLoop;

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

const std::list< SarissaEngine::Runtime::Framework::Actor* >::iterator
const MainLoop::AddObject(SarissaEngine::Runtime::Framework::Actor* newObj)
{
	objects_.emplace_back(newObj);
	auto it = objects_.end();
	it--;
	return it;
}

void const MainLoop::RemoveObject(SarissaEngine::Runtime::Framework::Actor* obj)
{
	objects_.remove(obj);
}

void const MainLoop::RemoveObject(const std::list< SarissaEngine::Runtime::Framework::Actor* >::iterator place)
{
	objects_.erase(place);
}
