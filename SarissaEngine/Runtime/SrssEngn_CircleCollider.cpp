#include "SrssEngn_CircleCollider.hpp"
#include "SrssEngn_ActorUtilities.hpp"
#include "SrssEngn_Actor.hpp"
#include "DxLib.h"

const bool CircleCollider::IsIntersectedWith(const CircleCollider* other)
{
	auto actorThis = const_cast<Actor*>(this->attachedActor_);
	auto pos1 = actorThis->GetPosition();
	auto actorOther = const_cast<Actor*>(other->GetActor());
	auto pos2 = actorOther->GetPosition();

	float r1 = ActorUtilities::GetComponent<CircleCollider*>(actorThis)->radius_
		, r2 = ActorUtilities::GetComponent<CircleCollider*>(actorOther)->radius_;

	float dx = pos1.x - pos2.x
		, dy = pos1.y - pos2.y;

	float dx2 = dx * dx
		, dy2 = dy * dy;

	float rr1 = r1 * r1
		, rr2 = r2 * r2;

	float ll = dx2 + dy2;

	return (rr1 + rr2) <= ll;
}

void CircleCollider::Begin_()
{}
void CircleCollider::Tick_(float deltaTime)
{
	auto actor = const_cast<Actor*>(attachedActor_);
	auto pos = actor->GetPosition();
	DrawCircle(pos.x, pos.y, radius_, GetColor(0, 255, 0), 1, 5);
}
void CircleCollider::End_()
{}
