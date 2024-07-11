#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "tnl_vector3.h"

namespace tnl {

	//----------------------------------------------------------------------------------------------
	// フォントテクスチャの生成
	// arg1... フォントサイズ　※ テクスチャサイズではありません
	// arg2... 生成する 1 文字 
	// arg3... 縦書き用なら true
	// arg4... 枠の太さ　※最大５ピクセルですが小さいフォントサイズに大枠は無効になります
	// arg5... 枠の色
	// arg6... フォントタイプ　※指定なしの場合は ＭＳ Ｐ明朝 になります
	// arg7... 上部の色 ( x = R, y = G, z = B )
	// arg8... 下部の色 ( x = R, y = G, z = B )
	// ret....  
	// tuple [ 0 : 4 byte カラー配列 ] 
	// tuple [ 1 : テクスチャの幅 ] 
	// tuple [ 2 : テクスチャの高さ ]
	// tuple [ 3 : 文字の幅 ]
	// tuple [ 4 : 文字の高さ ]
	// tips... 使用例
	// auto tex1 = tnl::CreateFontTexture(32, tnl::ToOnceWChara("あ"));
	//
	std::tuple<std::shared_ptr<char>, uint32_t, uint32_t, uint32_t, uint32_t>
		CreateFontTexture(
			const uint32_t font_size,
			const wchar_t& once,
			bool is_vertical = false,
			int32_t thickness = 0,
			const tnl::Vector3& thickness_color = tnl::Vector3(0, 0, 0),
			const std::string& font_type = "",
			const tnl::Vector3& top_color = tnl::Vector3(1, 1, 1),
			const tnl::Vector3& bottom_color = tnl::Vector3(1, 1, 1));

}
