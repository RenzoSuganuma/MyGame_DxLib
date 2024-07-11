#include "tnl_math.h"
#include "tnl_vector3.h"
#include "tnl_vector2i.h"
#include "tnl_quaternion.h"
#include "tnl_matrix.h"

namespace tnl {

	using namespace DirectX;

	const Vector3 Vector3::forward	= { 0,  0,  1 };
	const Vector3 Vector3::back		= { 0,  0, -1 };
	const Vector3 Vector3::left		= {-1,  0,  0 };
	const Vector3 Vector3::right	= { 1,  0,  0 };
	const Vector3 Vector3::up		= { 0,  1,  0 };
	const Vector3 Vector3::down		= { 0, -1,  0 };

	//------------------------------------------------------------------------------------------------------------------------
	Vector3::Vector3(const tnl::Vector2i& v) noexcept : DirectX::XMFLOAT3((float)v.x, (float)v.y, 0) {}

	//------------------------------------------------------------------------------------------------------------------------
	Vector3& Vector3::operator = (const tnl::Vector2i& v) noexcept {
		this->x = (float)v.x;
		this->y = (float)v.y;
		return *this;
	}

	//------------------------------------------------------------------------------------------------------------------------
	const Vector3 Vector3::axis[static_cast<uint32_t>(eAxis::MAX)] = {
		Vector3::forward,
		Vector3::back,
		Vector3::left,
		Vector3::right,
		Vector3::up,
		Vector3::down
	};

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::AlongPlane(const tnl::Vector3& in, const tnl::Vector3& pn) noexcept {
		tnl::Vector3 c = tnl::Vector3::Cross(in, pn);
		tnl::Vector3 along = tnl::Vector3::Cross(pn, c);
		return Normalize(along);
	}

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::TransformCoord(const Vector3& v, const Quaternion& q) noexcept {
		XMVECTOR xv = XMVector3Rotate( XMLoadFloat3(&v), XMLoadFloat4(&q));
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, xv); 
		return static_cast<Vector3>(f3);
	}
	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::InverseTransformCoord(const Vector3& v, const Quaternion& q) noexcept {
		XMVECTOR xv = XMVector3InverseRotate(XMLoadFloat3(&v), XMLoadFloat4(&q));
		XMFLOAT3 f3;
		XMStoreFloat3(&f3, xv);
		return static_cast<Vector3>(f3);
	}

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::TransformCoord(const Vector3& v, const tnl::Matrix& m) noexcept {
		XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&v), DirectX::XMLoadFloat4x4(&m)));
		return static_cast<Vector3>(f3);
	}

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::Transform(const tnl::Vector3& v, const tnl::Matrix& m) noexcept {
		XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&v), DirectX::XMLoadFloat4x4(&m)));
		return static_cast<Vector3>(f3);
	}
	Vector3 Vector3::TransformNormal(const tnl::Vector3& v, const tnl::Matrix& m) noexcept {
		XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&v), DirectX::XMLoadFloat4x4(&m)));
		return static_cast<Vector3>(f3);
	}


	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::CreateScreenRay(const int screen_x, const int screen_y, const int screen_w, const int screen_h, const tnl::Matrix& view, const tnl::Matrix& proj) noexcept {

		Matrix mv, mp;
		mv = view;
		mp = proj;

		mv._41 = 0.0f;
		mv._42 = 0.0f;
		mv._43 = 0.0f;

		// ビューポート行列を作成
		Matrix view_mat;

		view_mat._11 = (float)(screen_w >> 1);
		view_mat._22 = -1.0f * (float)(screen_h >> 1);
		view_mat._33 = 1.0f;
		view_mat._41 = (float)(screen_w >> 1);
		view_mat._42 = (float)(screen_h >> 1);
		view_mat._43 = 0.0f;

		// スクリーン位置をワールドへ
		Vector3 v;
		v.x = (float)screen_x;
		v.y = (float)screen_y;
		v.z = 0.0f;

		Matrix inv, inv_proj, inv_view;
		inv = Matrix::Inverse(view_mat);
		inv_proj = Matrix::Inverse(mp);
		inv_view = Matrix::Inverse(mv);
		//inv = inv_view * inv_proj * inv;
		inv = inv * inv_proj * inv_view;

		return Vector3::Normalize( Vector3::TransformCoord(v, inv) );
	}

	//-----------------------------------------------------------------------------------------------------
	Vector3 Vector3::ConvertToScreen(const Vector3& v, const float screen_w, const float screen_h, const Matrix& view, const Matrix& proj) noexcept {

		Vector3 rv(0, 0, 0);
		float w = screen_w * 0.5f;
		float h = screen_h * 0.5f;
		Matrix wkMtx, tm;

		tm = Matrix::Translation(v);
		wkMtx = view * proj;
		wkMtx = tm * wkMtx;

		wkMtx._41 /= wkMtx._44;
		wkMtx._42 /= wkMtx._44;
		rv.x = (w + (w * wkMtx._41));
		rv.y = (h + (-h * wkMtx._42));
		return rv;
	}


	//------------------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::Random(
		const float min_x, const float max_x,
		const float min_y, const float max_y,
		const float min_z, const float max_z) noexcept{
		Vector3 v = {
			tnl::GetRandomDistributionFloat(min_x, max_x),
			tnl::GetRandomDistributionFloat(min_y, max_y),
			tnl::GetRandomDistributionFloat(min_z, max_z)
		};
		return v;
	}


	//-----------------------------------------------------------------------------------------------------------
	tnl::Vector3 Vector3::RandomAxis(const tnl::Vector3& base_axis, float limit_angle) noexcept {
		tnl::Vector3 cv = tnl::Vector3::Cross(base_axis, tnl::Vector3::left);
		if (cv.length() <= FLT_EPSILON) cv = tnl::Vector3::Cross(base_axis, tnl::Vector3::up);
		if (cv.length() <= FLT_EPSILON) cv = tnl::Vector3::Cross(base_axis, tnl::Vector3::forward);
		if (cv.length() <= FLT_EPSILON) return { 0, 0, 0 };

		float angle = tnl::GetRandomDistributionFloat(-limit_angle, limit_angle);
		tnl::Quaternion rot = tnl::Quaternion::RotationAxis(cv, tnl::ToRadian(angle));

		tnl::Vector3 random_v = tnl::Vector3::TransformCoord(base_axis, rot);
		angle = tnl::GetRandomDistributionFloat(-90, 90);
		rot = tnl::Quaternion::RotationAxis(base_axis, tnl::ToRadian(angle));
		random_v = tnl::Vector3::TransformCoord(random_v, rot);
		return random_v;
	}


	//------------------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::UniformLerp(const Vector3& s, const Vector3& e, float time_limit, float ct) noexcept  {
		if (time_limit <= 0 || (e - s).length() <= FLT_EPSILON) return s;
		if (ct >= time_limit) return e;
		return s + (e - s) * (ct / time_limit);
	}


	//------------------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::SmoothLerp(const Vector3& s, const Vector3& e, float time_limit, float ct, int strength) noexcept {
		if (time_limit <= 0 || (e - s).length() <= FLT_EPSILON) return s;
		if (ct >= time_limit) return e;
		strength = (strength > 5) ? 5 : strength;

		float t = 0;
		float si = 0;
		float cct = ct;

		for (int i = 0; i < strength; ++i) {
			float ss = 0;
			float ee = time_limit;

			t = cct / time_limit * 180;
			si = 0.5f + sinf(tnl::ToRadian(-90) + tnl::ToRadian(t)) * 0.5f;
			cct = ss + si * (ee - ss);
		}

		t = cct / time_limit * 180;
		si = 0.5f + sinf(tnl::ToRadian(-90) + tnl::ToRadian(t)) * 0.5f;
		return s + (e - s) * si;

	}

	//------------------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::AccelLerp(const Vector3& s, const Vector3& e, float time_limit, float ct, int strength) noexcept {
		// ゼロまたは負の時間制限、または初期位置と終了位置が同じ場合
		if (time_limit <= 0 || (e - s).length() <= FLT_EPSILON) return s;
		if (ct >= time_limit) return e;
		strength = (strength > 5) ? 5 : strength;

		float distance = 0;
		float fluctuation = 0;
		float a = 0;
		float n = 0;
		float cct = ct;
		Vector3 vn ;

		for (int i = 0; i < strength; ++i) {
			float ss = 0;
			float ee = time_limit;

			distance = ee - ss;
			fluctuation = (2.0f * distance) / powf(time_limit, 2.0f);

			a = fluctuation;
			n = (ee - ss) / fabs(ee - ss);
			cct = n * ((fluctuation * powf(cct, 2)) * 0.5f);
		}

		// 自由落下の公式を利用して加速度を計算 
		// 公式 t = sqrt( 2h / g )
		// 変形して g = 2h / t^2
		// [ t:時間  h:高さ  g:重力加速度 ]
		distance = (e - s).length() ;
		fluctuation = (2.0f * distance) / powf(time_limit, 2.0f);

		// 等加速直線運動の公式 x = x0 + ( v0 * t ) + ( 1/2 * a * t^2 )
		// ( v0 * t ) は初速だが今回は使わないので省略
		// [ x:現在位置  x0:初期位置  t:時間  a:加速度 ]
		a = fluctuation;
		vn = Normalize( e - s );
		return s + vn * ((fluctuation * powf(cct, 2)) * 0.5f);

	}

	//------------------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::DecelLerp(const Vector3& s, const Vector3& e, float time_limit, float ct) noexcept {
		// ゼロまたは負の時間制限、または初期位置と終了位置が同じ場合
		if (time_limit <= 0 || (e - s).length() <= FLT_EPSILON) return s;
		if (ct >= time_limit) return e;

		float distance = ( e - s ).length() ;
		float fluctuation = (2.0f * distance) / powf(time_limit, 2.0f);

		float a = fluctuation;
		float t = ct;
		Vector3 vn = Normalize(e-s);
		return e - vn * ((a * powf(time_limit - t, 2)) * 0.5f);
	}


}

