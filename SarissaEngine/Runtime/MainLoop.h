#pragma once

#include "list"
#include "DxLib.h"

/* �yLayer:1�z */

/* �Q�[�����[�v ���C�� */

class Actor;

// ���C�����[�v : Unity�ł����V�[��
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

