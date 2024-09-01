#pragma once

#include "list"
#include "string"
#include "DxLib.h"

// 前方定義
class Level;
class Component;
class CircleCollider;

// アクタークラス
class Actor
{
protected:
	VECTOR position_{ 0 }, rotation_{ 0 };
	Level* placedLevel_;

	virtual void Begin_();
	virtual void Tick_(float deltaTime);
	virtual void End_();

	virtual void OnBeginOverlap_(const CircleCollider* other);
	virtual void OnStillOverlap_(const CircleCollider* other);
	virtual void OnEndOverlap_(const CircleCollider* other);
public:
	std::list< Component* > attachedComponents_;
	std::string name_ = " Actor_ ";
	bool enabled_ = true;

	Actor();
	~Actor();

	void Begin();
	void Tick(float deltaTime);
	void End();

	void OnBeginOverlap(const CircleCollider* other);
	void OnStillOverlap(const CircleCollider* other);
	void OnEndOverlap(const CircleCollider* other);

	std::list< Component* >::iterator
		const AddComponent(Component* component);

	void const RemoveComponent(std::list< Component* >::iterator place);

	void const SetPosition(VECTOR newPos);

	const VECTOR const GetPosition();

	void const SetRotaton(VECTOR newRot);

	const VECTOR const GetRotaton();

	void const SetPlacedLevel(Level* level);
};
