#pragma once

// タスクシステムのインターフェイス
class ITaskSystem
{
public:
	virtual void Setup() = 0 ;
	virtual void Update() = 0 ;
	virtual void End() = 0 ;
	virtual void Draw() = 0 ;
	virtual void MemRelease() = 0 ;
};

