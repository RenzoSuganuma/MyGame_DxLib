#pragma once

#include "SrssEngn_Component.hpp"
#include "SrssEngn_Actor.hpp"

class ActorUtilities final
{
public:
	template<typename T>
	static Component* GetComponent(Actor* getComponentFrom)
	{
		auto it = getComponentFrom->attachedComponents_.begin();

		while (it != getComponentFrom->attachedComponents_.end())
		{
			auto r = static_cast<T>(*it);
			if (r != nullptr)
			{
				return (*it);
				break;
			}
			it++;
		}

		return nullptr;
	}
};

