#pragma once

#include "list"
#include "DxLib.h"
#include "Actor.h"
	 
/* 【Layer:1】 */

/* ゲームループ レイヤ */

namespace MainLoop
{
	// メインループ : Unityでいうシーン
	class Main_Loop final
	{
	private:
		std::list< Framework::Actor* > objects_;

	public:
		Main_Loop();
		~Main_Loop();

		void const MainLoopEntry();

		void const MainLoopUpdate();

		void const MainLoopExit();

		const std::list< Framework::Actor* >::iterator
			const AddObject(Framework::Actor* newObject);

		void const RemoveObject(Framework::Actor* obj);

		void const RemoveObject(const std::list< Framework::Actor* >::iterator place);
	};
}
