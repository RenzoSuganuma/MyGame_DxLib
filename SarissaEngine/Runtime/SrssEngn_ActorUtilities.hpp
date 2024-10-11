#pragma once

#include "SrssEngn_Component.hpp"
#include "SrssEngn_Actor.hpp"

// アクタークラスのユーティリティクラス。煩雑な操作を吸収した機能を提供する。
class ActorUtilities final {
public:
	template<typename T>
	static T GetComponent(const Actor * getComponentFrom)
	{
		auto it = getComponentFrom->attachedComponents_.begin();

		while (it != getComponentFrom->attachedComponents_.end())
		{
			auto r = static_cast<T>(*it);
			if (r != nullptr)
			{
				return r;
				break;
			}
			it++;
		}

		return nullptr;
	}
};

