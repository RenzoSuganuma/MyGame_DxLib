#pragma once

#include "list"
#include "string"
#include "DxLib.h"
#include "SrssEngn_Vector3.h"

// 前方定義
class Level;
class Component;
class CircleCollider;

// ゲーム内のオブジェクトとして扱うクラスの基底クラス
class Actor
{
protected:
	Vector3 position_{ 0 };
	Vector3 rotation_{ 0 };
	Level* placedLevel_;

	virtual void Begin_();
	virtual void Tick_(float deltaTime);
	virtual void End_();
	virtual void Draw_();
	// 呼び出しを未実装
	virtual void OnBeginOverlap_(const CircleCollider* other);
	// 実装済み
	virtual void OnStillOverlap_(const CircleCollider* other);
	// 呼び出しを未実装
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
	void Draw();

	void OnBeginOverlap(const CircleCollider* other);
	void OnStillOverlap(const CircleCollider* other);
	void OnEndOverlap(const CircleCollider* other);

	std::list< Component* >::iterator
		const AddComponent(Component * component);

	void const RemoveComponent(const std::list<Component*>::iterator place);

	void const SetPosition(Vector3 newPos);

	const Vector3 const GetPosition();

	void const SetRotation(Vector3 newRot);

	const Vector3 const GetRotation();

	void const SetPlacedLevel(const Level* level);
};
