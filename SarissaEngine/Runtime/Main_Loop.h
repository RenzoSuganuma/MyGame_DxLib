#pragma once

#include "list"
#include "DxLib.h"
#include "Actor.h"

/* �yLayer:1�z */

/* �Q�[�����[�v ���C�� */

// ���C�����[�v : Unity�ł����V�[��
class Main_Loop final
{
private:
	std::list< Actor* > objects_;

public:
	Main_Loop();
	~Main_Loop();

	void const MainLoopEntry();

	void const MainLoopUpdate();

	void const MainLoopExit();

	const std::list< Actor* >::iterator
		const AddObject(Actor* newObject);

	void const RemoveObject(Actor* obj);

	void const RemoveObject(const std::list< Actor* >::iterator place);
};

