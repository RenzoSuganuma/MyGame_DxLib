#pragma once

#include "list"

class Actor;

// メインループ : Unityでいうシーン
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
