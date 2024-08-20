#include "CircleCollider.h"

using SarissaEngine::Runtime::Component::CircleCollider;

void CircleCollider::Begin_()
{}
void CircleCollider::Tick_(float deltaTime)
{}
void CircleCollider::End_()
{}

// 判定処理の本体
const bool  const CircleCollider::IsOverlapping(const Collider& other)
{}
// 重なり合わせが起きた時
void const  CircleCollider::OnBeginOverlap(const Collider& other)
{}
// 重なり合わせの時
void const  CircleCollider::OnStillOverlap(const Collider& other)
{}
// 重なり合わせがなくなったとき
void const  CircleCollider::OnEndOverlap(const Collider& other)
{}
