#pragma once
#include <string>

namespace dxe {
	extern std::string g_x_file_hedder_str;
	extern tnl::Rect WINDOW_RECT ;

	enum class eDepthStenclil {
		DEPTH_W_ON_T_ON
		, DEPTH_W_ON_T_OFF
		, DEPTH_W_OFF_T_ON
		, DEPTH_W_OFF_T_OFF
		, MAX
	};
	enum class eBlendState {
		ALPHA
		, ADD
		, SUB
		, MUL
		, NORMAL
		, MAX
	};
	enum class eSamplerState {
		ANISOTROPIC
		, BILINEAR
		, NEAREST
		, MAX
	};
	enum class eRasterizerState {
		CULL_NONE
		, CULL_FRONT
		, CULL_BACK
		, WIREFRAME
		, MAX
	};

}

