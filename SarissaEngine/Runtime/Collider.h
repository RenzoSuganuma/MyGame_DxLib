#pragma once

#include "SrssEngn_RuntimeClasses.hpp"

// コライダーの基底クラス
class Collider : public SarissaEngine::Runtime::Framework::Component
{
public:
	// 判定処理の本体
	const bool virtual const IsOverlapping(const Collider& other);
	// 重なり合わせが起きた時
	void const virtual OnBeginOverlap(const Collider& other);
	// 重なり合わせの時
	void const virtual OnStillOverlap(const Collider& other);
	// 重なり合わせがなくなったとき
	void const virtual OnEndOverlap(const Collider& other);
};
