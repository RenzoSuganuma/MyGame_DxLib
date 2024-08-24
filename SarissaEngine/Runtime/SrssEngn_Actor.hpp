#pragma once

#include "list"
#include "string"
#include "DxLib.h"
#include "SrssEngn_Collider.hpp"

// 前方定義
class Level;
class Component;

// アクタークラス
class Actor
{
protected:
	VECTOR position_{ 0 }, rotation_{ 0 };
	Level* placedLevel_;

public:
	std::list< Component* > attachedComponents_;
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

	void OnBeginOverlap(const Collider* other);
	void OnStillOverlap(const Collider* other);
	void OnEndOverlap(const Collider* other);

	virtual void OnBeginOverlap_(const Collider* other);
	virtual void OnStillOverlap_(const Collider* other);
	virtual void OnEndOverlap_(const Collider* other);

	std::list< Component* >::iterator
		const AddComponent(Component* component);

	void const RemoveComponent(std::list< Component* >::iterator place);

	void const SetPosition(VECTOR newPos);

	const VECTOR const GetPosition();

	void const SetRotaton(VECTOR newRot);

	const VECTOR const GetRotaton();

	void const SetPlacedLevel(Level* level);
};
