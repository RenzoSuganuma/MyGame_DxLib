#pragma once

#include "SrssEngn_RuntimeClasses.hpp"

// �R���C�_�[�̊��N���X
class Collider : public SarissaEngine::Runtime::Framework::Component
{
public:
	// ���菈���̖{��
	const bool virtual const IsOverlapping(const Collider& other);
	// �d�Ȃ荇�킹���N������
	void const virtual OnBeginOverlap(const Collider& other);
	// �d�Ȃ荇�킹�̎�
	void const virtual OnStillOverlap(const Collider& other);
	// �d�Ȃ荇�킹���Ȃ��Ȃ����Ƃ�
	void const virtual OnEndOverlap(const Collider& other);
};
