#pragma once
#include "tnl_util.h"
#include "tnl_vector2i.h"
#include "tnl_vector2f.h"

namespace tnl {

	class Rect {
	public :

		enum class eLocation {
			TNL_ENUM_BIT_FLAG(__IN__)
			, TNL_ENUM_BIT_FLAG(__OUT__)
			, TNL_ENUM_BIT_FLAG(__LEFT__)
			, TNL_ENUM_BIT_FLAG(__RIGHT__)
			, TNL_ENUM_BIT_FLAG(__TOP__)
			, TNL_ENUM_BIT_FLAG(__BOTTOM__)
			, IN_LEFT			= __IN__ | __LEFT__
			, IN_RIGHT			= __IN__ | __RIGHT__
			, IN_TOP			= __IN__ | __TOP__
			, IN_BOTTOM			= __IN__ | __BOTTOM__
			, IN_LEFT_TOP		= __IN__ | __LEFT__ | __TOP__
			, IN_LEFT_BOTTOM	= __IN__ | __LEFT__ | __BOTTOM__
			, IN_RIGHT_TOP		= __IN__ | __RIGHT__ | __TOP__
			, IN_RIGHT_BOTTOM	= __IN__ | __RIGHT__ | __BOTTOM__
			, OUT_LEFT			= __OUT__ | __LEFT__
			, OUT_RIGHT			= __OUT__ | __RIGHT__
			, OUT_TOP			= __OUT__ | __TOP__
			, OUT_BOTTOM		= __OUT__ | __BOTTOM__
			, OUT_LEFT_TOP		= __OUT__ | __LEFT__ | __TOP__
			, OUT_LEFT_BOTTOM	= __OUT__ | __LEFT__ | __BOTTOM__
			, OUT_RIGHT_TOP		= __OUT__ | __RIGHT__ | __TOP__
			, OUT_RIGHT_BOTTOM	= __OUT__ | __RIGHT__ | __BOTTOM__
		};

		Rect() = default;
		Rect( const Vector2i& position, const Vector2i& size ) : position_(position), size_(size) {}
		Rect( const uint32_t w, const uint32_t h ) : size_((int32_t)w, (int32_t)h) {};
		explicit Rect( const Vector2i& size ) : size_(size){}
		explicit Rect( const Rect& rect) : size_(rect.size_), position_(rect.position_) {}

		tnl::Vector2i getOtherPosition(eLocation location, const Rect& other);

		void setScaleFromSize(uint32_t size_max);
		TNL_PROPERTY(float, Scale, scale_);
		TNL_PROPERTY(tnl::Vector2i, Size, size_);
		TNL_PROPERTY(tnl::Vector2i, Position, position_);
	private :
		float scale_ = 1.0f;
		tnl::Vector2i size_;
		tnl::Vector2i position_;
	};
	TNL_ENUM_CLASS_BIT_MASK_OPERATOR(Rect::eLocation);

}

