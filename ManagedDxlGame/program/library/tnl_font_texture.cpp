#include <wrl.h>	
#include <algorithm>
#include "tnl_font_texture.h"
#include "tnl_util.h"

namespace tnl {

	std::tuple<std::shared_ptr<char>, uint32_t, uint32_t, uint32_t, uint32_t>
		CreateFontTexture(
			const uint32_t font_size,
			const wchar_t& once,
			bool is_vertical,
			int32_t thickness,
			const tnl::Vector3& thickness_color,
			const std::string& font_type,
			const tnl::Vector3& top_color,
			const tnl::Vector3& bottom_color) {


		// フォントハンドルの生成
		int fontSize = font_size;
		int fontWeight = 1000;

		std::string s = (font_type == "") ? "ＭＳ Ｐ明朝" : font_type;
		wchar_t buff[256] = { 0 };
		tnl::ToWChara(buff, s);

		// フォントの生成
		LOGFONT lf = {
			fontSize,0,0,0,fontWeight,0,0,0,
			SHIFTJIS_CHARSET,
			OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY,
			FIXED_PITCH | FF_MODERN,
			TEXT("")
		};

		memcpy(lf.lfFaceName, buff, sizeof(lf.lfFaceName));

		HFONT hFont = CreateFontIndirect(&lf);


		// 現在のウィンドウに適用
		// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
		HDC hdc = GetDC(NULL);
		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

		// フォントビットマップ取得
		const wchar_t* c = &once;
		UINT code = (UINT)*c;


		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		GLYPHMETRICS gm;
		CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };
		DWORD size = GetGlyphOutlineW(hdc, code, GGO_GRAY4_BITMAP, &gm, 0, NULL, &mat);

		BYTE* pMono = new BYTE[size];

		GetGlyphOutlineW(hdc, code, GGO_GRAY4_BITMAP, &gm, size, pMono, &mat);


		// 文字の幅と高さ
		int str_width = gm.gmCellIncX;
		int str_height = tm.tmHeight;

		// 文字が収まる最小幅と高さ
		int str_box_width = gm.gmBlackBoxX;
		int str_box_height = tm.tmHeight;

		// 文字ごとの大きさを統一されるテクスチャサイズ
		int tex_width = font_size + 1;
		int tex_height = font_size + 1;

		// ここで書き込む
		// フォント情報の書き込み
		// iOfs_x, iOfs_y : 書き出し位置(左上)
		// iBmp_w, iBmp_h : フォントビットマップの幅高
		// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
		int iOfs_x = gm.gmptGlyphOrigin.x;
		int iOfs_y = tm.tmAscent - gm.gmptGlyphOrigin.y;
		int iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
		int iBmp_h = gm.gmBlackBoxY;

		int Level = 17;
		int x, y;
		DWORD Alpha, Color;

		char* pTexBuf = new char[tex_width * tex_height * 4] ;
		memset(pTexBuf, 0, tex_width * tex_height * 4);


		// 空白対応
		wchar_t pwc[2] = { once, 0 };
		if (0 == wcscmp(pwc, L" ") || 0 == wcscmp(pwc, L"　")) {
			delete[] pMono;
			return std::make_tuple(std::shared_ptr<char>(pTexBuf), tex_width, tex_height, str_width, str_height);
		}

		// 縦書き小文字句読点対応
		if (is_vertical) {
			std::vector< std::wstring > wstr = {
				L"ぁ", L"ぃ", L"ぅ", L"ぇ", L"ぉ", L"ゃ", L"ゅ", L"ょ", L"っ",
				L"ァ", L"ィ", L"ゥ", L"ェ", L"ォ", L"ャ", L"ュ", L"ョ", L"ッ"
			};
			for (auto ws : wstr) {
				if (0 != wcscmp(pwc, ws.c_str())) continue;
				iOfs_y = (tm.tmHeight - gm.gmBlackBoxY) >> 1;
				iOfs_x = gm.gmCellIncX - gm.gmBlackBoxX;
				str_box_height = tm.tmHeight;
			}
			if (0 == wcscmp(pwc, L"、") || 0 == wcscmp(pwc, L"。")) {
				iOfs_y = (tex_height - tm.tmHeight) >> 1;
				iOfs_x = gm.gmCellIncX - gm.gmBlackBoxX;
				str_box_height = tex_height;
			}
		}

		BYTE* pBits = new BYTE[str_width * str_height * 4];
		BYTE* pFrame1 = new BYTE[str_width * str_height * 4];
		BYTE* pFrame2 = new BYTE[str_width * str_height * 4];
		BYTE* pFrame3 = new BYTE[str_width * str_height * 4];
		BYTE* pFrame4 = new BYTE[str_width * str_height * 4];

		memset(pBits, 0, str_width * str_height * 4);
		memset(pFrame1, 0, str_width * str_height * 4);
		memset(pFrame2, 0, str_width * str_height * 4);
		memset(pFrame3, 0, str_width * str_height * 4);
		memset(pFrame4, 0, str_width * str_height * 4);



		tnl::Vector3 color;
		tnl::Vector3 csub = bottom_color - top_color;

		int32_t base_thickness = (thickness > 5) ? 5 : thickness;

		for (y = iOfs_y; y < (iOfs_y + iBmp_h); y++)
		{
			color = top_color + csub * ((1.0f / float(iOfs_y + iBmp_h)) * y);

			for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
			{
				int32_t thickness_x = base_thickness;
				int32_t thickness_y = base_thickness;
				thickness_x = ((x - thickness_x) < 0) ? 0 : thickness_x;
				thickness_y = ((y - thickness_y) < 0) ? 0 : thickness_y;
				thickness_x = ((x + thickness_x) > str_width) ? 0 : thickness_x;
				thickness_y = ((y + thickness_y) > str_height) ? 0 : thickness_y;

				Alpha = (255 * pMono[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);
				int R = int((color.x * float(Alpha)));
				int G = int((color.y * float(Alpha)));
				int B = int((color.z * float(Alpha)));

				Color = (Alpha << 24) | (R << 16) | (G << 8) | (B);

				int32_t ary = (str_width * y * 4) + (x * 4);
				if (0 > ary || ary >= (str_width * str_height * 4)) {
					break;
				}
				memcpy(pBits + (str_width * y * 4) + (x * 4), &Color, sizeof(DWORD));

				R = int((thickness_color.x * float(Alpha)));
				G = int((thickness_color.y * float(Alpha)));
				B = int((thickness_color.z * float(Alpha)));
				Color = (Alpha << 24) | (R << 16) | (G << 8) | (B);

				ary = (str_width * (y - thickness_y) * 4) + ((x - thickness_x) * 4);
				if (0 < ary && ary < (str_width * str_height * 4)) {
					memcpy(pFrame1 + ary, &Color, sizeof(DWORD));
				}
				ary = (str_width * (y - thickness_y) * 4) + ((x + thickness_x) * 4);
				if (0 < ary && ary < (str_width * str_height * 4)) {
					memcpy(pFrame2 + ary, &Color, sizeof(DWORD));
				}
				ary = (str_width * (y + thickness_y) * 4) + ((x - thickness_x) * 4);
				if (0 < ary && ary < (str_width * str_height * 4)) {
					memcpy(pFrame3 + ary, &Color, sizeof(DWORD));
				}
				ary = (str_width * (y + thickness_y) * 4) + ((x + thickness_x) * 4);
				if (0 < ary && ary < (str_width * str_height * 4)) {
					memcpy(pFrame4 + ary, &Color, sizeof(DWORD));
				}
			}
		}


		uint32_t* base = (uint32_t*)pBits;
		uint32_t* p_tex_buf = (uint32_t*)pTexBuf;
		uint32_t sy = tex_height - str_height;
		uint32_t sx = (tex_width - str_width) >> 1;

		if (0 == base_thickness) {
			for (uint32_t y = sy; y < static_cast<uint32_t>(str_height); ++y) {
				for (uint32_t x = sx; x < (str_width + sx); ++x) {
					uint32_t ary1 = (sy * tex_width) + (y * tex_width) + x;
					uint32_t ary2 = ((y - sy) * str_width) + (x - sx);
					if (0 > ary1 && ary1 >= static_cast<uint32_t>((tex_width * tex_height))) {
						TNL_DEBUG_ASSERTE("CreateFontTexture error");
					}
					if (0 > ary2 && ary2 >= static_cast<uint32_t>((str_width * str_height))) {
						TNL_DEBUG_ASSERTE("CreateFontTexture error");
					}
					p_tex_buf[ary1] = base[ary2];
				}
			}

			// 縦書き特殊文字回転対応
			if (is_vertical) {
				std::vector< std::wstring > wstr = {
					L"【", L"】", L"『", L"』", L"(", L")", L"「", L"」", L"[", L"]",
					L"《", L"》", L"{", L"}", L"<", L">", L"＜", L"＞", L"＝", L"ー", L"～"
				};

				for (auto ws : wstr) {
					if (0 != wcscmp(pwc, ws.c_str())) continue;
					uint32_t* rbuf = new uint32_t[tex_width * tex_height];
					for (uint32_t y = 0; y < static_cast<uint32_t>(tex_height); ++y) {
						for (uint32_t x = 0; x < static_cast<uint32_t>(tex_width); ++x) {
							rbuf[(y * tex_width) + (tex_width - 1) - x] = p_tex_buf[(x * tex_height) + y];
						}
					}
					memcpy(p_tex_buf, rbuf, sizeof(uint32_t) * static_cast<uint32_t>(tex_width * tex_height));
					delete[] rbuf;
					break;
				}
			}

			delete[] pMono;
			delete[] pBits;
			delete[] pFrame1;
			delete[] pFrame2;
			delete[] pFrame3;
			delete[] pFrame4;
			return std::make_tuple(std::shared_ptr<char>(pTexBuf), tex_width, tex_height, str_box_width, str_box_height);
		}

		uint32_t* frame1 = (uint32_t*)pFrame1;
		uint32_t* frame2 = (uint32_t*)pFrame2;
		uint32_t* frame3 = (uint32_t*)pFrame3;
		uint32_t* frame4 = (uint32_t*)pFrame4;
		for (uint32_t y = 0; y < static_cast<uint32_t>(str_height); ++y) {
			for (uint32_t x = 0; x < static_cast<uint32_t>(str_width); ++x) {
				uint32_t* pbc = &base[(y * str_width) + x];
				uint32_t* pfc1 = &frame1[(y * str_width) + x];
				uint32_t* pfc2 = &frame2[(y * str_width) + x];
				uint32_t* pfc3 = &frame3[(y * str_width) + x];
				uint32_t* pfc4 = &frame4[(y * str_width) + x];
				uint32_t bc = *pbc;
				uint32_t fc1 = *pfc1;
				uint32_t fc2 = *pfc2;
				uint32_t fc3 = *pfc3;
				uint32_t fc4 = *pfc4;

				float ba = float((bc & 0xff000000) >> 24) / 255.0f;
				float br = float((bc & 0x00ff0000) >> 16) / 255.0f;
				float bg = float((bc & 0x0000ff00) >> 8) / 255.0f;
				float bb = float((bc & 0x000000ff) >> 0) / 255.0f;

				float fa1 = float((fc1 & 0xff000000) >> 24) / 255.0f;
				float fr1 = float((fc1 & 0x00ff0000) >> 16) / 255.0f;
				float fg1 = float((fc1 & 0x0000ff00) >> 8) / 255.0f;
				float fb1 = float((fc1 & 0x000000ff) >> 0) / 255.0f;

				float fa2 = float((fc2 & 0xff000000) >> 24) / 255.0f;
				float fr2 = float((fc2 & 0x00ff0000) >> 16) / 255.0f;
				float fg2 = float((fc2 & 0x0000ff00) >> 8) / 255.0f;
				float fb2 = float((fc2 & 0x000000ff) >> 0) / 255.0f;

				float fa3 = float((fc3 & 0xff000000) >> 24) / 255.0f;
				float fr3 = float((fc3 & 0x00ff0000) >> 16) / 255.0f;
				float fg3 = float((fc3 & 0x0000ff00) >> 8) / 255.0f;
				float fb3 = float((fc3 & 0x000000ff) >> 0) / 255.0f;

				float fa4 = float((fc4 & 0xff000000) >> 24) / 255.0f;
				float fr4 = float((fc4 & 0x00ff0000) >> 16) / 255.0f;
				float fg4 = float((fc4 & 0x0000ff00) >> 8) / 255.0f;
				float fb4 = float((fc4 & 0x000000ff) >> 0) / 255.0f;

				float ca = std::clamp<float>(fa1 + fa2 + fa3 + fa4, 0, 1.0f);
				float cr = std::clamp<float>((fr1 * (1.0f - ba)) + (fr2 * (1.0f - ba)) + (fr3 * (1.0f - ba)) + (fr4 * (1.0f - ba)) + (br * ba), 0, 1.0f);
				float cg = std::clamp<float>((fg1 * (1.0f - ba)) + (fg2 * (1.0f - ba)) + (fg3 * (1.0f - ba)) + (fg4 * (1.0f - ba)) + (bg * ba), 0, 1.0f);
				float cb = std::clamp<float>((fb1 * (1.0f - ba)) + (fb2 * (1.0f - ba)) + (fb3 * (1.0f - ba)) + (fb4 * (1.0f - ba)) + (bb * ba), 0, 1.0f);

				//float ca = std::clamp<float>(fa1, 0, 1.0f);
				//float cr = std::clamp<float>((fr1 * (1.0f - ba)) + (br * ba), 0, 1.0f);
				//float cg = std::clamp<float>((fg1 * (1.0f - ba)) + (bg * ba), 0, 1.0f);
				//float cb = std::clamp<float>((fb1 * (1.0f - ba)) + (bb * ba), 0, 1.0f);

				uint32_t a = static_cast<uint32_t>(ca * 255.0f) << 24;
				uint32_t r = static_cast<uint32_t>(cr * 255.0f) << 16;
				uint32_t g = static_cast<uint32_t>(cg * 255.0f) << 8;
				uint32_t b = static_cast<uint32_t>(cb * 255.0f) << 0;
				*pbc = a | r | g | b;
			}
		}

		for (uint32_t y = sy; y < static_cast<uint32_t>(str_height); ++y) {
			for (uint32_t x = sx; x < (static_cast<uint32_t>(str_width) + sx); ++x) {
				p_tex_buf[(sy * tex_width) + (y * tex_width) + x] = base[((y - sy) * str_width) + (x - sx)];
			}
		}

		// 縦書き特殊文字回転対応
		if (is_vertical) {
			std::vector< std::wstring > wstr = {
				L"【", L"】", L"『", L"』", L"(", L")", L"「", L"」", L"[", L"]",
				L"《", L"》", L"{", L"}", L"<", L">", L"＜", L"＞", L"＝", L"ー", L"～"
			};

			for (auto ws : wstr) {
				if (0 != wcscmp(pwc, ws.c_str())) continue;
				uint32_t* rbuf = new uint32_t[tex_width * tex_height];
				for (uint32_t y = 0; y < static_cast<uint32_t>(tex_height); ++y) {
					for (uint32_t x = 0; x < static_cast<uint32_t>(tex_width); ++x) {
						rbuf[(y * tex_width) + (tex_width - 1) - x] = p_tex_buf[(x * tex_height) + y];
					}
				}
				memcpy(p_tex_buf, rbuf, sizeof(uint32_t) * tex_width * tex_height);
				delete[] rbuf;
				break;
			}
		}

		delete[] pMono;
		delete[] pBits;
		delete[] pFrame1;
		delete[] pFrame2;
		delete[] pFrame3;
		delete[] pFrame4;
		return std::make_tuple(std::shared_ptr<char>(pTexBuf), tex_width, tex_height, str_box_width, str_box_height);

	}


}
