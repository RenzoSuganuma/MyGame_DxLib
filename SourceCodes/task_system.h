#pragma once

namespace TaskSystem
{
	// �^�X�N�V�X�e��
	class Task
	{
	protected:
		virtual void Begin() = 0;
		virtual void Tick() = 0;
		virtual void End() = 0;

	public:
		Task();
		~Task();

		void Init();
		void Update();
		void Finalize();
	};
}
