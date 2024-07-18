#include "Main_Loop.h"

using MainLoop::Main_Loop;

Main_Loop::Main_Loop() {}
Main_Loop::~Main_Loop() {}

void const Main_Loop::MainLoopEntry()
{
	auto itr = tasks_.begin();

	while (itr != tasks_.end())
	{
		(*itr)->Init();
		itr++;
	}
}

void const Main_Loop::MainLoopUpdate()
{
	auto itr = tasks_.begin();

	while (itr != tasks_.end())
	{
		(*itr)->Update();
		itr++;
	}
}

void const Main_Loop::MainLoopExit()
{
	auto itr = tasks_.begin();

	while (itr != tasks_.end())
	{
		(*itr)->Finalize();
		itr++;
	}
}

const std::list< TaskSystem::Task* >::iterator
const Main_Loop::AddTask(TaskSystem::Task* newTask)
{
	tasks_.emplace_back(newTask);
	auto it = tasks_.end();
	it--;
	return it;
}


void const Main_Loop::RemoveTask(TaskSystem::Task* task)
{
	tasks_.remove(task);
}

void const Main_Loop::RemoveTask(const std::list< TaskSystem::Task* >::iterator place)
{
	tasks_.erase(place);
}
