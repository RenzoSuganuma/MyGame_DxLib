#pragma once
#include <DirectXMath.h>
#include "tnl_matrix.h"
namespace tnl {
	class Vector3;
	class Matrix;
	class Quaternion final : public DirectX::XMFLOAT4 {
	public:
		Quaternion() noexcept : DirectX::XMFLOAT4(0, 0, 0, 1) {}
		Quaternion(const float xx, const float yy, const float zz, const float ww) noexcept : DirectX::XMFLOAT4(xx, yy, zz, ww) {}
		explicit Quaternion(DirectX::XMVECTOR& v) noexcept { DirectX::XMStoreFloat4(this, v); }
		explicit Quaternion(DirectX::XMFLOAT4& v) noexcept { this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w; }

		//-----------------------------------------------------------------------------------------------------
		//
		// operator
		//
		const Quaternion& operator = (DirectX::XMVECTOR& other) noexcept;
		const Quaternion operator * (const Quaternion& other) const noexcept;
		const Quaternion& operator *= (const Quaternion& other) noexcept;

		//-----------------------------------------------------------------------------------------------------
		//
		// inline function
		//
		inline Matrix getMatrix() const noexcept {
			DirectX::XMFLOAT4X4 m4x4;
			DirectX::XMStoreFloat4x4(&m4x4, DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(this)));
			return static_cast<Matrix>(m4x4);
		}

		//-----------------------------------------------------------------------------------------------------
		//
		// static function
		//		
		static Quaternion RotationAxis(const Vector3& axis, const float rotate) noexcept;
		static Quaternion RotationRollPitchYawFromVector(const Vector3& angles) noexcept;
		static Quaternion Subtract(const Quaternion& q1, const Quaternion& q2) noexcept;
		static Quaternion LookAt(const Vector3& eye, const Vector3& look, const Vector3& vup);
		static Quaternion LookAtAxisY(const Vector3& eye, const Vector3& look);

		//-----------------------------------------------------------------------------------------------------
		//
		// function
		//
		Vector3 getEuler() const noexcept;

		void slerp(const Quaternion& q, const float t);
		void normalize();
	};


	//-----------------------------------------------------------------------------------------------------
	inline const Quaternion& Quaternion::operator = (DirectX::XMVECTOR& other) noexcept {
		DirectX::XMStoreFloat4(this, other);
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline const Quaternion Quaternion::operator * (const Quaternion& other) const noexcept {
		DirectX::XMVECTOR l = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR r = DirectX::XMLoadFloat4(&other);
		DirectX::XMVECTOR v = DirectX::XMQuaternionMultiply(l, r);
		DirectX::XMFLOAT4 f4;
		DirectX::XMStoreFloat4(&f4, v);
		return static_cast<Quaternion>(f4);
	}
	//-----------------------------------------------------------------------------------------------------
	inline const Quaternion& Quaternion::operator *= (const Quaternion& other) noexcept {
		*this = *this * other;
		return *this;
	}

} 
