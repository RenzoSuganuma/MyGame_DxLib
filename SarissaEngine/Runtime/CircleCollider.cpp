#include "CircleCollider.h"

using SarissaEngine::Runtime::Component::CircleCollider;

void CircleCollider::Begin_()
{}
void CircleCollider::Tick_(float deltaTime)
{}
void CircleCollider::End_()
{}

// ���菈���̖{��
const bool  const CircleCollider::IsOverlapping(const Collider& other)
{}
// �d�Ȃ荇�킹���N������
void const  CircleCollider::OnBeginOverlap(const Collider& other)
{}
// �d�Ȃ荇�킹�̎�
void const  CircleCollider::OnStillOverlap(const Collider& other)
{}
// �d�Ȃ荇�킹���Ȃ��Ȃ����Ƃ�
void const  CircleCollider::OnEndOverlap(const Collider& other)
{}
