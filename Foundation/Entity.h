#pragma once
#include "ITaskSystem.h"

// �G���e�B�e�B �N���X
class Entity : ITaskSystem
{
	void Setup() override ;
	void Update() override ;
	void End() override ;
	void Draw() override ;
	void MemRelease() override ;
};

