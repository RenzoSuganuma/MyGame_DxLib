#pragma once

#include "DxLib.h"
#include "Collider.h"

namespace SarissaEngine::Runtime::Component
{
	// �~�̃R���C�_�[
	class CircleCollider final : public Collider
	{
	public:
		float radian_ = 1.0f;
		int color_ = GetColor(255, 255, 255);

		void Begin_() override;
		void Tick_(float deltaTime) override;
		void End_() override;

		// ���菈���̖{��
		const bool  const IsOverlapping(const Collider& other) override;
		// �d�Ȃ荇�킹���N������
		void const  OnBeginOverlap(const Collider& other) override;
		// �d�Ȃ荇�킹�̎�
		void const  OnStillOverlap(const Collider& other) override;
		// �d�Ȃ荇�킹���Ȃ��Ȃ����Ƃ�
		void const  OnEndOverlap(const Collider& other) override;
	};
}
