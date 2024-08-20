#pragma once

#include "DxLib.h"
#include "Collider.h"

namespace SarissaEngine::Runtime::Component
{
	// 円のコライダー
	class CircleCollider final : public Collider
	{
	public:
		float radian_ = 1.0f;
		int color_ = GetColor(255, 255, 255);

		void Begin_() override;
		void Tick_(float deltaTime) override;
		void End_() override;

		// 判定処理の本体
		const bool  const IsOverlapping(const Collider& other) override;
		// 重なり合わせが起きた時
		void const  OnBeginOverlap(const Collider& other) override;
		// 重なり合わせの時
		void const  OnStillOverlap(const Collider& other) override;
		// 重なり合わせがなくなったとき
		void const  OnEndOverlap(const Collider& other) override;
	};
}
