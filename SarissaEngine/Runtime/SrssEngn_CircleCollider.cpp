#include "SrssEngn_CircleCollider.hpp"
#include "SrssEngn_ActorUtilities.hpp"
#include "SrssEngn_Actor.hpp"
#include "math.h"
#include "DxLib.h"

const bool CircleCollider::IsIntersectedWith(const CircleCollider* other)
{
	if (other == nullptr)return false;

	auto c1 = ActorUtilities::GetComponent< CircleCollider* >
		(const_cast<Actor*>(attachedActor_));
	auto c2 = ActorUtilities::GetComponent< CircleCollider* >
		(const_cast<Actor*>(other->GetActor()));

	auto p_c1 = const_cast<Actor*>(attachedActor_)->GetPosition();
	auto p_c2 = const_cast<Actor*>(other->GetActor())->GetPosition();

	float dx = p_c1.x - p_c2.x;
	float dy = p_c1.y - p_c2.y;
	float dx2 = dx * dx;
	float dy2 = dy * dy;
	float dc2 = dx2 + dy2;	// ’†S“¯Žm‚Ì‹——£‚Ì‚Qæ
	float dc = sqrt(dc2);
	float r1 = c1->GetRadius();
	float r2 = c2->GetRadius();

	float r = r1 + r2;
	float rr = r;

	// DEBUG

	DrawFormatString(0, 0, -1, "r = %f \n dc = %f", r, dc);

	// DEBUG END

	return dc <= r;
}

void CircleCollider::Begin_()
{}
void CircleCollider::Tick_(float deltaTime)
{
	auto actor = const_cast<Actor*>(attachedActor_);
	auto pos = actor->GetPosition();
	DrawCircle(pos.x, pos.y, radius_ * .5f, GetColor(0, 255, 0), 1, 5);
}
void CircleCollider::End_()
{}
