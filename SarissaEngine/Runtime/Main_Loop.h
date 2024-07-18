#pragma once

#include "list"
#include "DxLib.h"
#include "Super.h"
	 
/* �yLayer:1�z */

/* �Q�[�����[�v ���C�� */

namespace MainLoop
{
	// ���C�����[�v : Unity�ł����V�[��
	class Main_Loop final
	{
	private:
		std::list< Framework::Super* > objects_;

	public:
		Main_Loop();
		~Main_Loop();

		void const MainLoopEntry();

		void const MainLoopUpdate();

		void const MainLoopExit();

		const std::list< Framework::Super* >::iterator
			const AddObject(Framework::Super* newTask);

		void const RemoveObject(Framework::Super* task);

		void const RemoveObject(const std::list< Framework::Super* >::iterator place);
	};
}
