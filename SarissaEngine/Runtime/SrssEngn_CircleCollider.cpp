#include "SrssEngn_CircleCollider.hpp"
#include "SrssEngn_ActorUtilities.hpp"
#include "SrssEngn_Actor.hpp"
#include "math.h"
#include "DxLib.h"

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

void CircleCollider::Begin_()
{}
void CircleCollider::Tick_(float deltaTime)
{
	auto actor = const_cast<Actor*>(attachedActor_);
	auto pos = actor->GetPosition();
	DrawCircle(pos.x, pos.y, radius_, GetColor(0, 255, 0), true, 5);
}
void CircleCollider::End_()
{}
