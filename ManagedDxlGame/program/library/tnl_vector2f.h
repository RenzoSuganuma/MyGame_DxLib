#pragma once
#include <DirectXMath.h>
#include <string>

namespace tnl {

	class Vector2f final : public DirectX::XMFLOAT2 {
	public :
		Vector2f() noexcept : DirectX::XMFLOAT2(0, 0) {}
		Vector2f(const float xx, const float yy) noexcept : DirectX::XMFLOAT2(xx, yy) {}
		explicit Vector2f(const float v) noexcept : DirectX::XMFLOAT2(v, v) {}
		explicit Vector2f(DirectX::XMVECTOR& v) noexcept { DirectX::XMStoreFloat2(this, v); }
		explicit Vector2f(DirectX::XMFLOAT2& v) noexcept : DirectX::XMFLOAT2(v.x, v.y) {}

		static const Vector2f left;
		static const Vector2f right;
		static const Vector2f up;
		static const Vector2f down;

		//-----------------------------------------------------------------------------------------------------
		//
		// operator
		//

		bool operator == (const Vector2f& other) noexcept;
		bool operator != (const Vector2f& other) noexcept;

		Vector2f& operator = (DirectX::XMVECTOR& other) noexcept;
		Vector2f& operator = (const float other) noexcept;

		Vector2f operator * (const float other) const noexcept;
		Vector2f operator * (const Vector2f& other) const noexcept;
		Vector2f& operator *= (const float other) noexcept;
		Vector2f& operator *= (const Vector2f& other) noexcept;

		Vector2f operator / (const float other) const noexcept;
		Vector2f operator / (const Vector2f& other) const noexcept;
		Vector2f& operator /= (const float other) noexcept;
		Vector2f& operator /= (const Vector2f& other) noexcept;

		Vector2f operator + (const Vector2f& other) const noexcept;
		Vector2f& operator += (const Vector2f& other) noexcept;
		Vector2f operator - (const Vector2f& other) const noexcept;
		Vector2f& operator -= (const Vector2f& other) noexcept;
		Vector2f operator - () const noexcept;



		//-----------------------------------------------------------------------------------------------------
		//
		// inline function
		//
		void abs() noexcept;
		std::string toString() const noexcept;

		//-----------------------------------------------------------------------------------------------------
		//
		// static inline function
		//
		static Vector2f Abs( const Vector2f& v ) noexcept;
		static float Distance(const Vector2f& v1, const Vector2f& v2) noexcept;


	};

	//-----------------------------------------------------------------------------------------------------
	inline bool Vector2f::operator == (const Vector2f& other) noexcept {
		return ( this->x == other.x && this->y == other.y  );
	}

	//-----------------------------------------------------------------------------------------------------
	inline bool Vector2f::operator != (const Vector2f& other) noexcept {
		return (this->x != other.x || this->y != other.y);
	}

	//-----------------------------------------------------------------------------------------------------
	inline Vector2f& Vector2f::operator = (DirectX::XMVECTOR &other) noexcept {
		DirectX::XMStoreFloat2(this, other);
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f& Vector2f::operator = (const float other) noexcept {
		this->x = other;
		this->y = other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f Vector2f::operator * (const Vector2f& other) const noexcept {
		return { x * other.x, y * other.y };
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f Vector2f::operator * (const float other) const noexcept {
		return { x * other, y * other };
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f& Vector2f::operator *= (const float other) noexcept {
		*this = *this * other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f& Vector2f::operator *= (const Vector2f& other) noexcept {
		Vector2f v = other;
		*this = *this * v;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f Vector2f::operator / (const float other) const noexcept {
		return { x / other, y / other };
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f Vector2f::operator / (const Vector2f& other) const noexcept {
		return { x / other.x, y / other.y };
	}

	//-----------------------------------------------------------------------------------------------------
	inline Vector2f& Vector2f::operator /= (const float other) noexcept {
		*this = *this / other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f& Vector2f::operator /= (const Vector2f& other) noexcept {
		*this = *this / other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f Vector2f::operator + (const Vector2f& other) const noexcept {
		DirectX::XMFLOAT2 v2(this->x + other.x, this->y + other.y);
		return static_cast<Vector2f>(v2);
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f& Vector2f::operator += (const Vector2f& other) noexcept {
		*this = *this + other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f Vector2f::operator - (const Vector2f& other) const noexcept {
		DirectX::XMFLOAT2 v2(this->x - other.x, this->y - other.y);
		return static_cast<Vector2f>(v2);
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f& Vector2f::operator -= (const Vector2f& other) noexcept {
		*this = *this - other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2f Vector2f::operator-() const noexcept {
		Vector2f v = { -x, -y };
		return v;
	}

	//-----------------------------------------------------------------------------------------------------
	inline void Vector2f::abs() noexcept { x = (0 > x) ? -x : x; y = (0 > y) ? -y : y; }

	//-----------------------------------------------------------------------------------------------------
	inline std::string Vector2f::toString() const noexcept {
		std::string str = "[ x : ";
		str += std::to_string(this->x);
		str += " ] [ y : ";
		str += std::to_string(this->y);
		str += " ] ";
		return str;
	}

	//-----------------------------------------------------------------------------------------------------
	inline Vector2f Vector2f::Abs(const Vector2f& v) noexcept {
		Vector2f r = v;
		r.abs();
		return v;
	}


}
