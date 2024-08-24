#include "CircleCollider.h"
#include "SrssEngn_Actor.hpp"
#include "ActorUtilities.hpp"

const bool const CircleCollider::IsOverlapping(const CircleCollider& other)
{
	auto pos_this = const_cast<Actor*>(GetActor())->GetPosition();
	auto pos_other = const_cast<Actor*>(other.GetActor())->GetPosition();

	float dx = (pos_other.x - pos_this.x);
	float dy = (pos_other.y - pos_this.y);
	float dz = (pos_other.z - pos_this.z);

	float xx = dx * dx;
	float yy = dy * dy;
	float zz = dz * dz;

	float dd = xx + yy + zz;

	float r_this = radian_;
	float r_other = ActorUtilities::GetComponent< CircleCollider* >(const_cast<Actor*>(GetActor()))->radian_;

	float rr = (r_this + r_other) * (r_this + r_other);

	return dd < rr;
}

const VECTOR const CircleCollider::Center()
{
	return const_cast<Actor*>(GetActor())->GetPosition();
}
