#pragma once

#include "list"
#include "string"
#include "DxLib.h"

class Level;

class Actor
{
protected:
	VECTOR position_{ 0 }, rotation_{ 0 };
	std::list< Component* > attachedComponents_;
	Level* placedLevel_;

public:
	std::string name_ = " Actor_ ";
	bool enabled_ = true;

	Actor();
	~Actor();

	void Begin();
	void Tick(float deltaTime);
	void End();

	virtual void Begin_();
	virtual void Tick_(float deltaTime);
	virtual void End_();

	std::list< Component* >::iterator
		const
		AddComponent(Component* component);

	void const
		RemoveComponent(std::list< Component* >::iterator place);

	void const SetPosition(VECTOR newPos);
	const VECTOR const GetPosition();

	void const SetRotaton(VECTOR newRot);
	const VECTOR const GetRotaton();

	void const SetPlacedLevel(SarissaEngine::Runtime::Framework::Level* level);
};
