#pragma once
#include "../tnl_build_switch.h"
#ifdef TNL_BUILD_SWITCH_DIRECTX12
#include <wrl.h>
template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#endif

#include <memory>
template<class T> using Shared = std::shared_ptr<T>;
template<class T> using Weak = std::weak_ptr<T>;

