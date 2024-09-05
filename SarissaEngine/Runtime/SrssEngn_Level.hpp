#pragma once

#include "list"

// �O����`
class Actor;

// Unity�ł����V�[���A���ׂẴA�N�^�����݂���N���X�B�i�\���̂ł����������j
class Level final
{
private:
	std::list< Actor* > objects_;

public:
	Level();
	~Level();

	void const MainLoopEntry();

	void const MainLoopUpdate(float deltaTime);

	void const CollisionUpdate();

	void const MainLoopExit();

	const std::list< Actor* >::iterator
		const AddObject(const Actor * newObject);

	void const RemoveObject(const Actor * obj);

	void const RemoveObject(const std::list< Actor* >::iterator place);
};
