#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include <functional>
#include <chrono>
#include "tnl_using.h"

namespace tnl{


	// work.... メモリ解放マクロ
	#define TNL_SAFE_FREE( _p )			{ if( nullptr != (_p) ){ free( (_p) ) ; (_p) = nullptr ;  } } 
	#define TNL_SAFE_DELETE( _p )		{ if( (_p) ){ delete (_p) ; (_p) = nullptr ; } }
	#define TNL_SAFE_DELETE_ARY( _p )	{ if( (_p) ){ delete[] (_p) ; (_p) = nullptr ; } }
	#define TNL_SAFE_RELEASE( _p )		{ if( nullptr != (_p) ){ (_p)->Release() ; (_p) = nullptr ;  } } 

	inline int ARGB8( uint8_t a, uint8_t r, uint8_t g, uint8_t b ) { return ( a << 24 | r << 16 | g << 8 | b ); }
	inline int RGBA8( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 ) { return ( r << 24 | g << 16 | b << 8 | a ); }
	inline int ABGR8( uint8_t a, uint8_t b, uint8_t g, uint8_t r ) { return (a << 24 | b << 16 | g << 8 | r); }

	inline int GetR(uint32_t rgba8) { return (rgba8 & 0xff000000) >> 24; }
	inline int GetG(uint32_t rgba8) { return (rgba8 & 0x00ff0000) >> 16; }
	inline int GetB(uint32_t rgba8) { return (rgba8 & 0x0000ff00) >> 8; }
	inline int GetA(uint32_t rgba8) { return (rgba8 & 0x000000ff) ; }

	//----------------------------------------------------------------------------------------------
	// index 付き for( auto n : xxx )
	// 利用法
	// std::list<int> xxx ;
	// for_with_index( it, xxx, i, 0, ++ ){
	// }
	#define for_with_index( it, data, idx, start_idx, fluct ) for( auto[it, idx] = std::pair{ (data).begin(), (start_idx) }; (it) != (data).end(); (it)++, (idx)fluct )

	//----------------------------------------------------------------------------------------------
	// ベクトル展開
	#define TNL_DEP_V2(v) v.x, v.y
	#define TNL_DEP_V3(v) v.x, v.y, v.z
	#define TNL_DEP_V4(v) v.x, v.y, v.z, v.w

	//----------------------------------------------------------------------------------------------
	// 列挙型ビットフラグ
	// enum class Test{
	//		TNL_ENUM_BIT_FLAG(A),	// Test::A == 1 << 0
	//		TNL_ENUM_BIT_FLAG(B),	// Test::B == 1 << 1
	//		TNL_ENUM_BIT_FLAG(C),	// Test::C == 1 << 2
	//		TNL_ENUM_BIT_FLAG(D),	// Test::D == 1 << 3
	// } ;
	#define TNL_ENUM_BIT_FLAG(f) prefix_##f, f = (prefix_##f < 1)? 1 : ((prefix_##f)-1) << 1


	//----------------------------------------------------------------------------------------------
	// プリプロセッサによる文字列化と結合
	#define TNL_INL_STR( str ) #str
	#define TNL_INL_MERGE_STR( left, right ) TNL_INL_STR(left##right)


	//----------------------------------------------------------------------------------------------
	// 列挙型ビットフラグ用 ビットマスクオペレータ7種一括定義
	#define TNL_ENUM_CLASS_BIT_MASK_OPERATOR( enum_class )                                              \
	constexpr void operator |= (enum_class& left, enum_class right) {                                   \
		left = static_cast<enum_class>(static_cast<std::underlying_type<enum_class>::type>(left) | static_cast<std::underlying_type<enum_class>::type>(right));       \
	}                                                                                                   \
	constexpr enum_class operator | (enum_class left, enum_class right) {                               \
		return static_cast<enum_class>(static_cast<std::underlying_type<enum_class>::type>(left) | static_cast<std::underlying_type<enum_class>::type>(right));       \
	}                                                                                                   \
	constexpr void operator &= (enum_class& left, enum_class right) {                                   \
		left = static_cast<enum_class>(static_cast<std::underlying_type<enum_class>::type>(left) & static_cast<std::underlying_type<enum_class>::type>(right));       \
	}                                                                                                   \
	constexpr enum_class operator & (enum_class left, enum_class right) {                               \
		return static_cast<enum_class>(static_cast<std::underlying_type<enum_class>::type>(left) & static_cast<std::underlying_type<enum_class>::type>(right));       \
	}                                                                                                   \
	constexpr void operator ^= (enum_class& left, enum_class right) {                                   \
		left = static_cast<enum_class>(static_cast<std::underlying_type<enum_class>::type>(left) ^ static_cast<std::underlying_type<enum_class>::type>(right));       \
	}                                                                                                   \
	constexpr enum_class operator ^ (enum_class left, enum_class right) {                               \
		return static_cast<enum_class>(static_cast<std::underlying_type<enum_class>::type>(left) ^ static_cast<std::underlying_type<enum_class>::type>(right));       \
	}                                                                                                   \
	constexpr enum_class operator ~ (enum_class n) {                                                    \
		return static_cast<enum_class>(~static_cast<std::underlying_type<enum_class>::type>(n));        \
	}                                                                                                   \


	//----------------------------------------------------------------------------------------------------------------------------------------
	// getter setter 簡易記述
	#define TNL_PROPERTY( type, FuncName, field ) \
	type get##FuncName(){ return field ; } \
	void set##FuncName(const type& value){ field = value ; } \

	//----------------------------------------------------------------------------------------------------------------------------------------
	// getter setter( cpp へ 自己定義用 ) 簡易記述
	#define TNL_SET_SELF_PROPERTY( type, FuncName, field ) \
	type get##FuncName(){ return field ; } \
	void set##FuncName(const type& value) \

	//----------------------------------------------------------------------------------------------------------------------------------------
	// getter( cpp へ 自己定義用 ) setter 簡易記述
	#define TNL_GET_SELF_PROPERTY( type, FuncName, field ) \
	type get##FuncName(); \
	void set##FuncName(const type& value){ field = value ; } \

	//----------------------------------------------------------------------------------------------------------------------------------------
	// getter setter と (+=) (*=) (/=) の簡易記述
	#define TNL_CALC_PROPERTY( type, FuncName, field ) \
	type get##FuncName(){ return field ; } \
	void set##FuncName(const type& value){ field = value ; } \
	void add##FuncName(const type& value){ field += value ; } \
	void mul##FuncName(const type& value){ field *= value ; } \
	void div##FuncName(const type& value){ field /= value ; } \

	//----------------------------------------------------------------------------------------------------------------------------------------
	// getter setter と (*=) の簡易記述 ( quaternion 等 )
	#define TNL_MUL_PROPERTY( type, FuncName, field ) \
	type get##FuncName(){ return field ; } \
	void set##FuncName(const type& value){ field = value ; } \
	void mul##FuncName(const type& value){ field *= value ; } \

	//----------------------------------------------------------------------------------------------
	// ファイルサイズの取得
	uint64_t GetFileSize(const char* fileName);

	//----------------------------------------------------------------------------------------------
	// ファイルパスをパスとファイル名と拡張子に分離する
	// ret.... tuple [ 0 : パス ] [ 1 : ファイル名 ] [ 2 : 拡張子 ]
	std::tuple<std::string, std::string, std::string> DetachmentFilePath( const std::string& file_path );

	//----------------------------------------------------------------------------------------------
	// ttf フォントの追加
	void AddFontTTF( const std::string& ttf_file_path );


	//----------------------------------------------------------------------------------------------
	// リソースファイルからカラーバッファをロードする
	// arg1... ファイルパス
	// ret.... <0> カラーバッファ (rgba 各8bit)
	// ....... <1> 幅
	// ....... <2> 高さ
	// ....... <3> データサイズ
	std::tuple<std::unique_ptr<uint8_t>, uint32_t, uint32_t, uint32_t> LoadGraphicColorBuffer( const std::string& file_path );

	//----------------------------------------------------------------------------------------------
	// ファイルメモリからカラーバッファをロードする
	// arg1... ファイルメモリへのポインタ
	// arg2... メモリデータサイズ
	// ret.... <0> カラーバッファ (rgba 各8bit)
	// ....... <1> 幅
	// ....... <2> 高さ
	// ....... <3> データサイズ
	std::tuple<std::unique_ptr<uint8_t>, uint32_t, uint32_t, uint32_t> LoadGraphicColorBufferFromMemory(const unsigned char* file_data, int file_size);

	//----------------------------------------------------------------------------------------------
	// カラーバッファからBMPフォーマットのバッファを作成
	// arg1... カラーバッファ (rgba 各8bit)
	// arg2... 幅
	// arg3... 高さ
	// arg4... 生成されたデータサイズ( byte ) 省略可
	// ret.... 24bit BMP フォーマットバッファ
	// tips1.. アルファは適用されません
	std::unique_ptr<int8_t> CreateFormatBmp24(unsigned char* color_buff_rgba8, uint32_t width, uint32_t height, uint32_t* data_size = nullptr);

	//----------------------------------------------------------------------------------------------
	// カラーバッファからTGAフォーマットのバッファを作成
	// arg1... カラーバッファ (rgba 各8bit)
	// arg2... 幅
	// arg3... 高さ
	// arg4... 生成されたデータサイズ( byte ) 省略可
	// ret.... 32bit TGA フォーマットバッファ
	// tips1.. アルファが適用されます
	std::unique_ptr<int8_t> CreateFormatTga32(unsigned char* color_buff_rgba8, uint32_t width, uint32_t height, uint32_t* data_size = nullptr);


	//----------------------------------------------------------------------------------------------
	std::chrono::system_clock::time_point BeginClock();
	float EndClock(const std::chrono::system_clock::time_point& clock_start);

	//----------------------------------------------------------------------------------------------
	// char から wchar への変換
	void ToWChara(wchar_t* wstrDest, const char* strSrc, int length);
	void ToWChara(wchar_t* wstrDest, const std::string& src);
	wchar_t ToOnceWChara(const char* strSrc);
	// Sjis UTF8 相互変換
	std::string UTF8toSjis(const std::string& srcUTF8);
	std::string SjistoUTF8(const std::string& srcSjis);

	// float -> string 変換 ( フォーマット指定付き )
	std::string FloatToString(float value, const std::string& fmt = "%f");

	//----------------------------------------------------------------------------------------------
	// デバッグコンソールへの出力
	void DebugTrace(const char* _str, ...);


	//----------------------------------------------------------------------------------------------
	// ASSERT
#define TNL_DEBUG_ASSERTE( msg ) {}

	//----------------------------------------------------------------------------------------------
	// 警告出力
	void WarningMassage(const char* _str, ...);

	//----------------------------------------------------------------------------------------------
	// 下位ビット 0 から 指定桁のビットが有効な数値を作る
	inline int OnBit(int n) { return (1 << n); }

	//----------------------------------------------------------------------------------------------
	// １ビットづつ間隔を開ける
	int SpaceBit32(int n);

	//----------------------------------------------------------------------------------------------
	// 有効なビットをカウントする
	int CountBit8(const uint8_t v);
	int CountBit16(const uint16_t v);
	int CountBit32(const uint32_t v);
	int CountBit64(const uint64_t v);

	//----------------------------------------------------------------------------------------------
	// 最大有効ビット数（MSB：Most Significant Bit）
	// 全てのビットが 0 なら false が帰る
	// 例) value が 0xFF なら out には value の Bit数-1 が入る uint8_t なら 7
	bool MostBit8(const uint8_t value, int& out);
	bool MostBit16(const uint16_t value, int& out);
	bool MostBit32(const uint32_t value, int& out);
	bool MostBit64(const uint64_t value, int& out);

	//----------------------------------------------------------------------------------------------
	// 最小有効ビット数（LSB：Least Significant Bit）
	// 全てのビットが 0 なら false が帰る
	// 例) value が 0x01 なら out には 0 が入る
	bool LeastBit8(const uint8_t value, int& out);
	bool LeastBit16(const uint16_t value, int& out);
	bool LeastBit32(const uint32_t value, int& out);
	bool LeastBit64(const uint64_t value, int& out);


	//----------------------------------------------------------------------------------------------
	template<class T, class U>
	class FieldAccessor {
	public:
		FieldAccessor(T* obj
			, const std::function<U(T*)>& getter
			, const std::function<void(T*, const U&)>& setter)
			: obj_(obj)
			, getter_(getter)
			, setter_(setter)
		{}
		U get() { return getter_(obj_); }
		void set(const U& u) { return setter_(obj_, u); }
	private:
		T* obj_;
		std::function<U(T*)> getter_;
		std::function<void(T*, const U&)> setter_;
	};

}


