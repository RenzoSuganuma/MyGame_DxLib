#pragma once

#include "list"
#include "SarissaEngine_RuntimeClasses.h"

/* �yLayer:1�z */

/* �Q�[�����[�v ���C�� */

namespace SarissaEngine::Runtime::System
{

	// ���C�����[�v : Unity�ł����V�[��
	class MainLoop final
	{
	private:
		std::list< SarissaEngine::Runtime::Framework::Actor* > objects_;

	public:
		MainLoop();
		~MainLoop();

		void const
			MainLoopEntry();

		void const
			MainLoopUpdate();

		void const
			MainLoopExit();

		const std::list< SarissaEngine::Runtime::Framework::Actor* >::iterator
			const
			AddObject(SarissaEngine::Runtime::Framework::Actor* newObject);

		void const
			RemoveObject(SarissaEngine::Runtime::Framework::Actor* obj);

		void const
			RemoveObject(const std::list< SarissaEngine::Runtime::Framework::Actor* >::iterator place);
	};

}

