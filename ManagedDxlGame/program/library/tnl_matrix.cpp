#include "tnl_matrix.h"
#include "tnl_quaternion.h"

namespace tnl {

	using namespace DirectX;

	void Matrix::rotationQuaternion(const tnl::Quaternion& q) noexcept {
		DirectX::XMVECTOR v = DirectX::XMLoadFloat4(&q);
		DirectX::XMMATRIX xm = DirectX::XMMatrixRotationQuaternion(v);
		XMStoreFloat4x4(this, xm);
	}
	Matrix Matrix::RotationQuaternion(const tnl::Quaternion& q) noexcept {
		DirectX::XMVECTOR v = DirectX::XMLoadFloat4(&q);
		DirectX::XMMATRIX xm = DirectX::XMMatrixRotationQuaternion(v);
		DirectX::XMFLOAT4X4 f4x4;
		XMStoreFloat4x4(&f4x4, xm);
		return static_cast<Matrix>(f4x4);
	}

	Matrix Matrix::AffineTransformation(const Vector3& pos, const Vector3& scl, const Quaternion& rot, const Vector3& origin) noexcept {
		DirectX::XMMATRIX xm = DirectX::XMMatrixAffineTransformation(
			{ TNL_DEP_V3(scl) },
			{ TNL_DEP_V3(origin) },
			{ TNL_DEP_V4(rot) },
			{ TNL_DEP_V3(pos) });
		DirectX::XMFLOAT4X4 f4x4;
		XMStoreFloat4x4(&f4x4, xm);
		return static_cast<Matrix>(f4x4);
	}



}