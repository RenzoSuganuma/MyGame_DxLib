#include "main_loop.h"

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

const std::list< TaskSystem::Task_System* >::iterator
const Main_Loop::AddTask(TaskSystem::Task_System* newTask)
{
	tasks_.emplace_back(newTask);
	auto it = tasks_.end();
	it--;
	return it;
}


void const Main_Loop::RemoveTask(TaskSystem::Task_System* task)
{
	tasks_.remove(task);
}

void const Main_Loop::RemoveTask(const std::list< TaskSystem::Task_System* >::iterator place)
{
	tasks_.erase(place);
}
