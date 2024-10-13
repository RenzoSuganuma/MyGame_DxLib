#pragma once
#include "../Engine/LangUtil.h"
#include "SrssEngn_Vector3.h"
#include "SrssEngn_Component.hpp"
class Collider : public Component {
	// コライダータイプ。AABB,OBB,その他
public:
	enum ColliderType {
		AABB,
		OBB,
		Other
	};

public:
	ColliderType type_ = Other;

	static const Collider* Create(ColliderType type) {
		return new Collider;
	}

protected:
	// 中心
	Vector3 center_;
	// 範囲。面積、体積のイメージ
	Vector3 bounds_;
public:
	AutoProperty( Vector3 , center , center_ )
	AutoProperty( Vector3 , bounds , bounds_ )

	virtual const bool IsIntersectedWith(const Collider* other);

	void Begin_() override;
	void Tick_(float deltaTime) override;
	void End_() override;
	void Draw_() override;
};

