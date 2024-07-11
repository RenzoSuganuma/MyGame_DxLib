#pragma once
#include <DirectXMath.h>
#include <string>

namespace tnl {

	class Vector2i final : public DirectX::XMINT2 {
	public :
		Vector2i() noexcept : DirectX::XMINT2(0, 0) {}
		Vector2i(const int32_t xx, const int32_t yy) noexcept : DirectX::XMINT2(xx, yy) {}
		explicit Vector2i(DirectX::XMVECTOR& v) noexcept { DirectX::XMStoreSInt2(this, v); }
		explicit Vector2i(DirectX::XMINT2& v) noexcept : DirectX::XMINT2(v.x, v.y) {}

		static const Vector2i left;
		static const Vector2i right;
		static const Vector2i up;
		static const Vector2i down;

		//-----------------------------------------------------------------------------------------------------
		//
		// operator
		//

		bool operator == (const Vector2i& other) noexcept;
		bool operator != (const Vector2i& other) noexcept;

		Vector2i& operator = (DirectX::XMVECTOR& other) noexcept;
		Vector2i& operator = (const int32_t other) noexcept;

		Vector2i operator * (const int32_t other) const noexcept;
		Vector2i operator * (const Vector2i& other) const noexcept;
		Vector2i& operator *= (const int32_t other) noexcept;
		Vector2i& operator *= (const Vector2i& other) noexcept;

		Vector2i operator / (const int32_t other) const noexcept;
		Vector2i operator / (const Vector2i& other) const noexcept;
		Vector2i& operator /= (const int32_t other) noexcept;
		Vector2i& operator /= (const Vector2i& other) noexcept;

		Vector2i operator + (const Vector2i& other) const noexcept;
		Vector2i& operator += (const Vector2i& other) noexcept;
		Vector2i operator - (const Vector2i& other) const noexcept;
		Vector2i& operator -= (const Vector2i& other) noexcept;
		Vector2i operator - () const noexcept;



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
		static Vector2i Abs( const Vector2i& v ) noexcept;
		static uint32_t Distance(const Vector2i& v1, const Vector2i& v2) noexcept;


	};

	//-----------------------------------------------------------------------------------------------------
	inline bool Vector2i::operator == (const Vector2i& other) noexcept {
		return ( this->x == other.x && this->y == other.y  );
	}

	//-----------------------------------------------------------------------------------------------------
	inline bool Vector2i::operator != (const Vector2i& other) noexcept {
		return (this->x != other.x || this->y != other.y);
	}

	//-----------------------------------------------------------------------------------------------------
	inline Vector2i& Vector2i::operator = (DirectX::XMVECTOR &other) noexcept {
		DirectX::XMStoreSInt2(this, other);
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i& Vector2i::operator = (const int32_t other) noexcept {
		this->x = other;
		this->y = other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i Vector2i::operator * (const Vector2i& other) const noexcept {
		return { x * other.x, y * other.y };
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i Vector2i::operator * (const int32_t other) const noexcept {
		return { x * other, y * other };
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i& Vector2i::operator *= (const int32_t other) noexcept {
		*this = *this * other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i& Vector2i::operator *= (const Vector2i& other) noexcept {
		Vector2i v = other;
		*this = *this * v;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i Vector2i::operator / (const int32_t other) const noexcept {
		return { x / other, y / other };
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i Vector2i::operator / (const Vector2i& other) const noexcept {
		return { x / other.x, y / other.y };
	}

	//-----------------------------------------------------------------------------------------------------
	inline Vector2i& Vector2i::operator /= (const int32_t other) noexcept {
		*this = *this / other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i& Vector2i::operator /= (const Vector2i& other) noexcept {
		*this = *this / other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i Vector2i::operator + (const Vector2i& other) const noexcept {
		DirectX::XMINT2 v2(this->x + other.x, this->y + other.y);
		return static_cast<Vector2i>(v2);
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i& Vector2i::operator += (const Vector2i& other) noexcept {
		*this = *this + other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i Vector2i::operator - (const Vector2i& other) const noexcept {
		DirectX::XMINT2 v2(this->x - other.x, this->y - other.y);
		return static_cast<Vector2i>(v2);
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i& Vector2i::operator -= (const Vector2i& other) noexcept {
		*this = *this - other;
		return *this;
	}
	//-----------------------------------------------------------------------------------------------------
	inline Vector2i Vector2i::operator-() const noexcept {
		Vector2i v = { -x, -y };
		return v;
	}

	//-----------------------------------------------------------------------------------------------------
	inline void Vector2i::abs() noexcept { x = (0 > x) ? -x : x; y = (0 > y) ? -y : y; }

	//-----------------------------------------------------------------------------------------------------
	inline std::string Vector2i::toString() const noexcept {
		std::string str = "[ x : ";
		str += std::to_string(this->x);
		str += " ] [ y : ";
		str += std::to_string(this->y);
		str += " ] ";
		return str;
	}

	//-----------------------------------------------------------------------------------------------------
	inline Vector2i Vector2i::Abs(const Vector2i& v) noexcept {
		Vector2i r = v;
		r.abs();
		return v;
	}


}
