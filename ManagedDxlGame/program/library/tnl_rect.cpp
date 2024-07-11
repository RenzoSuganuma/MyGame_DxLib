#include "tnl_vector2i.h"
#include "tnl_rect.h"

namespace tnl {

	//---------------------------------------------------------------------------------------------------------------------------
	tnl::Vector2i Rect::getOtherPosition(eLocation location, const Rect& other) {

		tnl::Vector2i out = position_ ;

		float w1 = size_.x * scale_ * 0.5f;
		float w2 = other.size_.x * other.scale_ * 0.5f;

		float h1 = size_.y * scale_ * 0.5f;
		float h2 = other.size_.y * other.scale_ * 0.5f;

		// in side
		if (static_cast<bool>( location & eLocation::__IN__)) {
			if (static_cast<bool>(location & eLocation::__LEFT__))	 out.x = position_.x - (int32_t)w1 + (int32_t)w2;
			if (static_cast<bool>(location & eLocation::__RIGHT__))  out.x = position_.x + (int32_t)w1 - (int32_t)w2;
			if (static_cast<bool>(location & eLocation::__TOP__))	 out.y = position_.y - (int32_t)h1 + (int32_t)h2;
			if (static_cast<bool>(location & eLocation::__BOTTOM__)) out.y = position_.y + (int32_t)h1 - (int32_t)h2;
		}
		// out side
		else {
			if (static_cast<bool>(location & eLocation::__LEFT__))	 out.x = position_.x - (int32_t)w1 - (int32_t)w2;
			if (static_cast<bool>(location & eLocation::__RIGHT__))  out.x = position_.x + (int32_t)w1 + (int32_t)w2;
			if (static_cast<bool>(location & eLocation::__TOP__))	 out.y = position_.y - (int32_t)h1 - (int32_t)h2;
			if (static_cast<bool>(location & eLocation::__BOTTOM__)) out.y = position_.y + (int32_t)h1 + (int32_t)h2;
		}

		return out;
	}

	//---------------------------------------------------------------------------------------------------------------------------
	void Rect::setScaleFromSize(uint32_t size_max) {
		uint32_t more_bigger = std::max(size_.x, size_.y);
		scale_ = (float)size_max / (float)more_bigger;
	}

}