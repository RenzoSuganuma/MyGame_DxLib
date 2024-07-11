#pragma once
#include "tnl_instance.h"

namespace tnl {
	std::unordered_map<std::string, std::function<Shared<Instance>()>>* Instance::generator_register_ = nullptr ;
	std::unordered_map<std::string, std::function<Shared<Instance>(const std::any&)>>* Instance::generator_desc_register_ = nullptr ;
}
