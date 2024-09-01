#pragma once

// 当たり判定の判定がとれるエンティティが継承すべきクラス
class ICollidable
{
protected:
	float radius_ = 10.0f;

public:
	const bool const IsOverlapWith(const ICollidable& other);

	virtual void OnBeginCollide() = 0 ;
	virtual void OnStillCollide() = 0 ;
	virtual void OnEndCollide() = 0 ;
};

