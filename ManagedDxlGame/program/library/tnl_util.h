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


	// work.... ����������}�N��
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
	// index �t�� for( auto n : xxx )
	// ���p�@
	// std::list<int> xxx ;
	// for_with_index( it, xxx, i, 0, ++ ){
	// }
	#define for_with_index( it, data, idx, start_idx, fluct ) for( auto[it, idx] = std::pair{ (data).begin(), (start_idx) }; (it) != (data).end(); (it)++, (idx)fluct )

	//----------------------------------------------------------------------------------------------
	// �x�N�g���W�J
	#define TNL_DEP_V2(v) v.x, v.y
	#define TNL_DEP_V3(v) v.x, v.y, v.z
	#define TNL_DEP_V4(v) v.x, v.y, v.z, v.w

	//----------------------------------------------------------------------------------------------
	// �񋓌^�r�b�g�t���O
	// enum class Test{
	//		TNL_ENUM_BIT_FLAG(A),	// Test::A == 1 << 0
	//		TNL_ENUM_BIT_FLAG(B),	// Test::B == 1 << 1
	//		TNL_ENUM_BIT_FLAG(C),	// Test::C == 1 << 2
	//		TNL_ENUM_BIT_FLAG(D),	// Test::D == 1 << 3
	// } ;
	#define TNL_ENUM_BIT_FLAG(f) prefix_##f, f = (prefix_##f < 1)? 1 : ((prefix_##f)-1) << 1


	//----------------------------------------------------------------------------------------------
	// �v���v���Z�b�T�ɂ�镶���񉻂ƌ���
	#define TNL_INL_STR( str ) #str
	#define TNL_INL_MERGE_STR( left, right ) TNL_INL_STR(left##right)


	//----------------------------------------------------------------------------------------------
	// �񋓌^�r�b�g�t���O�p �r�b�g�}�X�N�I�y���[�^7��ꊇ��`
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
	// getter setter �ȈՋL�q
	#define TNL_PROPERTY( type, FuncName, field ) \
	type get##FuncName(){ return field ; } \
	void set##FuncName(const type& value){ field = value ; } \

	//----------------------------------------------------------------------------------------------------------------------------------------
	// getter setter( cpp �� ���Ȓ�`�p ) �ȈՋL�q
	#define TNL_SET_SELF_PROPERTY( type, FuncName, field ) \
	type get##FuncName(){ return field ; } \
	void set##FuncName(const type& value) \

	//----------------------------------------------------------------------------------------------------------------------------------------
	// getter( cpp �� ���Ȓ�`�p ) setter �ȈՋL�q
	#define TNL_GET_SELF_PROPERTY( type, FuncName, field ) \
	type get##FuncName(); \
	void set##FuncName(const type& value){ field = value ; } \

	//----------------------------------------------------------------------------------------------------------------------------------------
	// getter setter �� (+=) (*=) (/=) �̊ȈՋL�q
	#define TNL_CALC_PROPERTY( type, FuncName, field ) \
	type get##FuncName(){ return field ; } \
	void set##FuncName(const type& value){ field = value ; } \
	void add##FuncName(const type& value){ field += value ; } \
	void mul##FuncName(const type& value){ field *= value ; } \
	void div##FuncName(const type& value){ field /= value ; } \

	//----------------------------------------------------------------------------------------------------------------------------------------
	// getter setter �� (*=) �̊ȈՋL�q ( quaternion �� )
	#define TNL_MUL_PROPERTY( type, FuncName, field ) \
	type get##FuncName(){ return field ; } \
	void set##FuncName(const type& value){ field = value ; } \
	void mul##FuncName(const type& value){ field *= value ; } \

	//----------------------------------------------------------------------------------------------
	// �t�@�C���T�C�Y�̎擾
	uint64_t GetFileSize(const char* fileName);

	//----------------------------------------------------------------------------------------------
	// �t�@�C���p�X���p�X�ƃt�@�C�����Ɗg���q�ɕ�������
	// ret.... tuple [ 0 : �p�X ] [ 1 : �t�@�C���� ] [ 2 : �g���q ]
	std::tuple<std::string, std::string, std::string> DetachmentFilePath( const std::string& file_path );

	//----------------------------------------------------------------------------------------------
	// ttf �t�H���g�̒ǉ�
	void AddFontTTF( const std::string& ttf_file_path );


	//----------------------------------------------------------------------------------------------
	// ���\�[�X�t�@�C������J���[�o�b�t�@�����[�h����
	// arg1... �t�@�C���p�X
	// ret.... <0> �J���[�o�b�t�@ (rgba �e8bit)
	// ....... <1> ��
	// ....... <2> ����
	// ....... <3> �f�[�^�T�C�Y
	std::tuple<std::unique_ptr<uint8_t>, uint32_t, uint32_t, uint32_t> LoadGraphicColorBuffer( const std::string& file_path );

	//----------------------------------------------------------------------------------------------
	// �t�@�C������������J���[�o�b�t�@�����[�h����
	// arg1... �t�@�C���������ւ̃|�C���^
	// arg2... �������f�[�^�T�C�Y
	// ret.... <0> �J���[�o�b�t�@ (rgba �e8bit)
	// ....... <1> ��
	// ....... <2> ����
	// ....... <3> �f�[�^�T�C�Y
	std::tuple<std::unique_ptr<uint8_t>, uint32_t, uint32_t, uint32_t> LoadGraphicColorBufferFromMemory(const unsigned char* file_data, int file_size);

	//----------------------------------------------------------------------------------------------
	// �J���[�o�b�t�@����BMP�t�H�[�}�b�g�̃o�b�t�@���쐬
	// arg1... �J���[�o�b�t�@ (rgba �e8bit)
	// arg2... ��
	// arg3... ����
	// arg4... �������ꂽ�f�[�^�T�C�Y( byte ) �ȗ���
	// ret.... 24bit BMP �t�H�[�}�b�g�o�b�t�@
	// tips1.. �A���t�@�͓K�p����܂���
	std::unique_ptr<int8_t> CreateFormatBmp24(unsigned char* color_buff_rgba8, uint32_t width, uint32_t height, uint32_t* data_size = nullptr);

	//----------------------------------------------------------------------------------------------
	// �J���[�o�b�t�@����TGA�t�H�[�}�b�g�̃o�b�t�@���쐬
	// arg1... �J���[�o�b�t�@ (rgba �e8bit)
	// arg2... ��
	// arg3... ����
	// arg4... �������ꂽ�f�[�^�T�C�Y( byte ) �ȗ���
	// ret.... 32bit TGA �t�H�[�}�b�g�o�b�t�@
	// tips1.. �A���t�@���K�p����܂�
	std::unique_ptr<int8_t> CreateFormatTga32(unsigned char* color_buff_rgba8, uint32_t width, uint32_t height, uint32_t* data_size = nullptr);


	//----------------------------------------------------------------------------------------------
	std::chrono::system_clock::time_point BeginClock();
	float EndClock(const std::chrono::system_clock::time_point& clock_start);

	//----------------------------------------------------------------------------------------------
	// char ���� wchar �ւ̕ϊ�
	void ToWChara(wchar_t* wstrDest, const char* strSrc, int length);
	void ToWChara(wchar_t* wstrDest, const std::string& src);
	wchar_t ToOnceWChara(const char* strSrc);
	// Sjis UTF8 ���ݕϊ�
	std::string UTF8toSjis(const std::string& srcUTF8);
	std::string SjistoUTF8(const std::string& srcSjis);

	// float -> string �ϊ� ( �t�H�[�}�b�g�w��t�� )
	std::string FloatToString(float value, const std::string& fmt = "%f");

	//----------------------------------------------------------------------------------------------
	// �f�o�b�O�R���\�[���ւ̏o��
	void DebugTrace(const char* _str, ...);


	//----------------------------------------------------------------------------------------------
	// ASSERT
#define TNL_DEBUG_ASSERTE( msg ) {}

	//----------------------------------------------------------------------------------------------
	// �x���o��
	void WarningMassage(const char* _str, ...);

	//----------------------------------------------------------------------------------------------
	// ���ʃr�b�g 0 ���� �w�茅�̃r�b�g���L���Ȑ��l�����
	inline int OnBit(int n) { return (1 << n); }

	//----------------------------------------------------------------------------------------------
	// �P�r�b�g�ÂԊu���J����
	int SpaceBit32(int n);

	//----------------------------------------------------------------------------------------------
	// �L���ȃr�b�g���J�E���g����
	int CountBit8(const uint8_t v);
	int CountBit16(const uint16_t v);
	int CountBit32(const uint32_t v);
	int CountBit64(const uint64_t v);

	//----------------------------------------------------------------------------------------------
	// �ő�L���r�b�g���iMSB�FMost Significant Bit�j
	// �S�Ẵr�b�g�� 0 �Ȃ� false ���A��
	// ��) value �� 0xFF �Ȃ� out �ɂ� value �� Bit��-1 ������ uint8_t �Ȃ� 7
	bool MostBit8(const uint8_t value, int& out);
	bool MostBit16(const uint16_t value, int& out);
	bool MostBit32(const uint32_t value, int& out);
	bool MostBit64(const uint64_t value, int& out);

	//----------------------------------------------------------------------------------------------
	// �ŏ��L���r�b�g���iLSB�FLeast Significant Bit�j
	// �S�Ẵr�b�g�� 0 �Ȃ� false ���A��
	// ��) value �� 0x01 �Ȃ� out �ɂ� 0 ������
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


