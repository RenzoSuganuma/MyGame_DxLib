#include "SrssEngn_RuntimeClasses.h"

using SarissaEngine::Runtime::Framework::Level;

Level::Level()
{
}

Level::~Level() 
{
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

void const Level::MainLoopExit()
{
	auto itr = objects_.begin();

	while (itr != objects_.end())
	{
		(*itr)->End();
		itr++;
	}
}

const std::list< SarissaEngine::Runtime::Framework::Actor* >::iterator
const Level::AddObject(SarissaEngine::Runtime::Framework::Actor* newObj)
{
	objects_.emplace_back(newObj);
	auto it = objects_.end();
	it--;
	return it;
}

void const Level::RemoveObject(SarissaEngine::Runtime::Framework::Actor* obj)
{
	objects_.remove(obj);
}

void const Level::RemoveObject(const std::list< SarissaEngine::Runtime::Framework::Actor* >::iterator place)
{
	objects_.erase(place);
}
