#pragma once
#include "ITaskSystem.h"

// エンティティ クラス
class Entity : ITaskSystem
{
	void Setup() override ;
	void Update() override ;
	void End() override ;
	void Draw() override ;
	void MemRelease() override ;
};

