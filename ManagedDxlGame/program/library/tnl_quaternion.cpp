#include <math.h>
#include "tnl_util.h"
#include "tnl_math.h"
#include "tnl_vector3.h"
#include "tnl_quaternion.h"

namespace tnl {

	using namespace DirectX;


	Quaternion Quaternion::RotationAxis(const Vector3& axis, const float rotate) noexcept {
		XMFLOAT4 f4;		
		XMStoreFloat4( &f4, XMQuaternionRotationAxis(XMLoadFloat3(&axis), rotate) );
		return static_cast<Quaternion>(f4);
	}
	Quaternion Quaternion::RotationRollPitchYawFromVector(const Vector3& angles) noexcept {
		XMFLOAT4 f4;
		XMStoreFloat4(&f4, XMQuaternionRotationRollPitchYawFromVector( XMLoadFloat3(&angles) ) );
		return static_cast<Quaternion>(f4);
	}

	Quaternion Quaternion::Subtract(const Quaternion& q1, const Quaternion& q2) noexcept {
		DirectX::XMVECTOR q0i = DirectX::XMQuaternionInverse(DirectX::XMLoadFloat4(&q1));
		DirectX::XMVECTOR qd = DirectX::XMQuaternionMultiply(q0i, DirectX::XMLoadFloat4(&q2));
		if (qd.m128_f32[3] < 0) {
			qd.m128_f32[0] *= -1.0f;
			qd.m128_f32[1] *= -1.0f;
			qd.m128_f32[2] *= -1.0f;
			qd.m128_f32[3] *= -1.0f;
		}
		return static_cast<Quaternion>(qd);
	}

	Vector3 Quaternion::getEuler() const noexcept {

		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;

		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		// 1 - 2y^2 - 2z^2
		float m00 = 1.f - (2.f * y2) - (2.f * z2);

		// 2xy + 2wz
		float m01 = (2.f * xy) + (2.f * wz);

		// 2xy - 2wz
		float m10 = (2.f * xy) - (2.f * wz);

		// 1 - 2x^2 - 2z^2
		float m11 = 1.f - (2.f * x2) - (2.f * z2);

		// 2xz + 2wy
		float m20 = (2.f * xz) + (2.f * wy);

		// 2yz+2wx
		float m21 = (2.f * yz) - (2.f * wx);

		// 1 - 2x^2 - 2y^2
		float m22 = 1.f - (2.f * x2) - (2.f * y2);


		float tx, ty, tz;

		if ( fabsf(m21-1.0f) < FLT_EPSILON )
		{
			tx = PI / 2.f;
			ty = 0;
			tz = atan2f( m10, m00 );
		}
		else if ( fabsf(m21+1.0f) < FLT_EPSILON )
		{
			tx = -PI / 2.0f;
			ty = 0;
			tz = atan2f(m10, m00);
		}
		else
		{
			tx = asinf(-m21);
			ty = atan2f(m20, m22);
			tz = atan2f(m01, m11);
		}

		return Vector3(tx, ty, tz);

	}

	void Quaternion::slerp(const Quaternion& q, const float t) {
		DirectX::XMStoreFloat4(this, DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(this), DirectX::XMLoadFloat4(&q), t));
	}

	void Quaternion::normalize() {
		DirectX::XMStoreFloat4(this, DirectX::XMQuaternionNormalize(DirectX::XMLoadFloat4(this)));
	}

	Quaternion Quaternion::LookAt(const Vector3& eye, const Vector3& look, const Vector3& vup) {
		DirectX::XMMATRIX xm = DirectX::XMMatrixLookAtLH(
			DirectX::XMLoadFloat3(&eye),
			DirectX::XMLoadFloat3(&look),
			DirectX::XMLoadFloat3(&vup));
		DirectX::XMMATRIX inv = DirectX::XMMatrixInverse(nullptr, xm);
		XMVECTOR q = XMQuaternionRotationMatrix(inv);
		return static_cast<Quaternion>(q);
	}

	Quaternion Quaternion::LookAtAxisY(const Vector3& eye, const Vector3& look) {
		Vector3 vn = Vector3::Normalize( (look - eye).xz() );
		float angle = vn.angle({ 0, 0, 1 });
		float y = (vn.x < 0) ? -1.0f : 1.0f;
		return RotationAxis({0, y, 0}, angle);
	}

}
