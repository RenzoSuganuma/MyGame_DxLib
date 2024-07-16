#pragma once

#include "list"
#include "DxLib.h"
#include "task_system.h"

/* 【Layer:1】 */

/* ゲームループ レイヤ */

namespace MainLoop
{
	// メインループ
	class Main_Loop final
	{
	private:
		std::list< TaskSystem::Task_System* > tasks_;

	public:
		Main_Loop();
		~Main_Loop();

		void const MainLoopEntry();

		void const MainLoopUpdate();

		void const MainLoopExit();

		const std::list< TaskSystem::Task_System* >::iterator
			const AddTask(TaskSystem::Task_System* newTask);

		void const RemoveTask(TaskSystem::Task_System* task);

		void const RemoveTask(const std::list< TaskSystem::Task_System* >::iterator place);
	};
}
