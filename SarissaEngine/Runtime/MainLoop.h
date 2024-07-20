#pragma once

#include "list"
#include "DxLib.h"

/* 【Layer:1】 */

/* ゲームループ レイヤ */

class Actor;

// メインループ : Unityでいうシーン
class MainLoop final
{
private:
	std::list< Actor* > objects_;

public:
	MainLoop();
	~MainLoop();

	void const MainLoopEntry();

	void const MainLoopUpdate();

	void const MainLoopExit();

	const std::list< Actor* >::iterator
		const AddObject(Actor* newObject);

	void const RemoveObject(Actor* obj);

	void const RemoveObject(const std::list< Actor* >::iterator place);
};

