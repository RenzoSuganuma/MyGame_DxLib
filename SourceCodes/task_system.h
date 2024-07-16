#pragma once

namespace TaskSystem
{
	// タスクシステム
	class Task_System
	{
	protected:
		virtual void Begin() = 0;
		virtual void Tick() = 0;
		virtual void End() = 0;

	public:
		Task_System();
		~Task_System();

		void Init();
		void Update();
		void Finalize();
	};
}
