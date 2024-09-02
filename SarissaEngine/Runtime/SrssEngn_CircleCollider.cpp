#include "SrssEngn_CircleCollider.hpp"
#include "SrssEngn_ActorUtilities.hpp"
#include "SrssEngn_Actor.hpp"
#include "math.h"
#include "DxLib.h"

#pragma region virtual-functions
void CircleCollider::Begin_() {}
void CircleCollider::Tick_(float deltaTime) {}
void CircleCollider::End_() {}
void CircleCollider::Draw_() {}
#pragma endregion

const bool CircleCollider::IsIntersectedWith(const CircleCollider* other)
{
	auto c1 = ActorUtilities::GetComponent< CircleCollider* >
		(const_cast<Actor*>(attachedActor_));
	auto c2 = ActorUtilities::GetComponent< CircleCollider* >
		(const_cast<Actor*>(other->GetActor()));

	auto p_c1 = const_cast<Actor*>(attachedActor_)->GetPosition();
	auto p_c2 = const_cast<Actor*>(other->GetActor())->GetPosition();

	double dx = p_c1.x - p_c2.x;
	double dy = p_c1.y - p_c2.y;
	double r1 = c1->GetRadius();
	double r2 = c2->GetRadius();

	double rr = (r1 + r2) * (r1 + r2);

	return  (dx * dx) + (dy * dy) <= (r1 + r2) * (r1 + r2);
}

