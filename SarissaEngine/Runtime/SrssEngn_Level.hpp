﻿#pragma once

#include "list"

// 前方定義
class Actor;

// Unityでいうシーン、すべてのアクタが存在するクラス。（構造体でもいいかも）
class Level final
{
private:
	std::list< std::shared_ptr< Actor > > objects_;

public:
	Level();
	~Level();

	void const MainLoopEntry();

	void const MainLoopUpdate(float deltaTime);

	void const CollisionUpdate();

	void const MainLoopExit();

	const std::list< std::shared_ptr< Actor > >::iterator
		const AddObject(const Actor * newObject);

	void const RemoveObject(const Actor * obj);

	void const RemoveObject(const std::list< std::shared_ptr< Actor > >::iterator place);
};
