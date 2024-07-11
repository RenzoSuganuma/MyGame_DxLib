#pragma once
#pragma warning(disable:4005)
#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>
#include <dinputd.h>

#include <tuple>
#include <stdint.h>
#include <functional>
#include "../library/tnl_vector3.h"

namespace tnl {

	class Input {
	public :

		#define TNL_MOUSE_INPUT_LEFT	(0x01)
		#define TNL_MOUSE_INPUT_RIGHT	(0x02)
		#define TNL_MOUSE_INPUT_MIDDLE	(0x04)
		#define TNL_MOUSE_OUTPUT_LEFT	(0x08)
		#define TNL_MOUSE_OUTPUT_RIGHT	(0x10)
		#define TNL_MOUSE_OUTPUT_MIDDLE	(0x20)

		// マウス対応キー 一覧
		enum class eMouse {
			LEFT,
			RIGHT,
			CENTER,
		};
		enum class eMouseTrigger {
			IN_LEFT		= TNL_MOUSE_INPUT_LEFT,
			IN_RIGHT	= TNL_MOUSE_INPUT_RIGHT,
			IN_MIDDLE	= TNL_MOUSE_INPUT_MIDDLE,
			OUT_LEFT	= TNL_MOUSE_OUTPUT_LEFT,
			OUT_RIGHT	= TNL_MOUSE_OUTPUT_RIGHT,
			OUT_MIDDLE	= TNL_MOUSE_OUTPUT_MIDDLE
		};

		// キーボード対応キー 一覧
		enum class eKeys {
			KB_BACK		// バックスペース
			, KB_TAB		// タブ
			, KB_RETURN	// エンター
			, KB_LSHIFT	// 左シフト
			, KB_RSHIFT	// 右シフト
			, KB_LCONTROL	// 左コントロール
			, KB_RCONTROL	// 右コントロール
			, KB_ESCAPE	// エスケープ
			, KB_SPACE	    // スペース
			, KB_PGUP		// ＰａｇｅＵＰ
			, KB_PGDN		// ＰａｇｅＤｏｗｎ
			, KB_END		// エンド
			, KB_HOME		// ホーム
			, KB_LEFT		// 左
			, KB_UP		// 上
			, KB_RIGHT	    // 右
			, KB_DOWN		// 下
			, KB_INSERT	// インサート
			, KB_DELETE	// デリート

			, KB_MINUS		    // －
			, KB_YEN			// ￥
			, KB_PREVTRACK	    // ＾
			, KB_PERIOD		// ．
			, KB_SLASH		    // ／
			, KB_LALT			// 左ＡＬＴ
			, KB_RALT			// 右ＡＬＴ
			, KB_SCROLL		// ScrollLock
			, KB_SEMICOLON	    // ；
			, KB_COLON		    // ：
			, KB_LBRACKET		// ［
			, KB_RBRACKET		// ］
			, KB_AT			// ＠
			, KB_BACKSLASH	    // ＼
			, KB_COMMA		    // ，
			, KB_CAPSLOCK		// CaspLock
			, KB_PAUSE		    // PauseBreak

			, KB_NUMPAD0	// テンキー０
			, KB_NUMPAD1	// テンキー１
			, KB_NUMPAD2	// テンキー２
			, KB_NUMPAD3	// テンキー３
			, KB_NUMPAD4	// テンキー４
			, KB_NUMPAD5	// テンキー５
			, KB_NUMPAD6	// テンキー６
			, KB_NUMPAD7	// テンキー７
			, KB_NUMPAD8	// テンキー８
			, KB_NUMPAD9	// テンキー９
			, KB_MULTIPLY	// テンキー＊
			, KB_ADD		// テンキー＋
			, KB_SUBTRACT	// テンキー－
			, KB_DECIMAL	// テンキー．
			, KB_DIVIDE	// テンキー／
			, KB_NUMPADENTER	// テンキーのエンター

			, KB_F1	// Ｆ１
			, KB_F2	// Ｆ２
			, KB_F3	// Ｆ３
			, KB_F4	// Ｆ４
			, KB_F5	// Ｆ５
			, KB_F6	// Ｆ６
			, KB_F7	// Ｆ７
			, KB_F8	// Ｆ８
			, KB_F9	// Ｆ９
			, KB_F10	// Ｆ１０
			, KB_F11	// Ｆ１１
			, KB_F12	// Ｆ１２

			, KB_A	// Ａ
			, KB_B	// Ｂ
			, KB_C	// Ｃ
			, KB_D	// Ｄ
			, KB_E	// Ｅ
			, KB_F	// Ｆ
			, KB_G	// Ｇ
			, KB_H	// Ｈ
			, KB_I	// Ｉ
			, KB_J	// Ｊ
			, KB_K	// Ｋ
			, KB_L	// Ｌ
			, KB_M	// Ｍ
			, KB_N	// Ｎ
			, KB_O	// Ｏ
			, KB_P	// Ｐ
			, KB_Q	// Ｑ
			, KB_R	// Ｒ
			, KB_S	// Ｓ
			, KB_T	// Ｔ
			, KB_U	// Ｕ
			, KB_V	// Ｖ
			, KB_W	// Ｗ
			, KB_X	// Ｘ
			, KB_Y	// Ｙ
			, KB_Z	// Ｚ
			, KB_0	// ０
			, KB_1	// １
			, KB_2	// ２
			, KB_3	// ３
			, KB_4	// ４
			, KB_5	// ５
			, KB_6	// ６
			, KB_7	// ７
			, KB_8	// ８
			, KB_9	// ９
			, KB_MAX
		};

		// ジョイパッド対応キー 一覧
		// ※ パッドの種類で対応キー番号が異なる場合もあるらしい
		enum class ePad {
			KEY_0		// 多くの場合 □ ( A )
			, KEY_1		// 多くの場合 × ( B )
			, KEY_2		// 多くの場合 〇 ( X )
			, KEY_3		// 多くの場合 △ ( Y )
			, KEY_4		// 多くの場合 L1
			, KEY_5		// 多くの場合 R1
			, KEY_6		// 多くの場合 L2
			, KEY_7		// 多くの場合 R2
			, KEY_8		// 多くの場合 SHARE
			, KEY_9		// 多くの場合 OPTION
			, KEY_10	// 多くの場合 LStickDown
			, KEY_11	// 多くの場合 RStickDown
			, KEY_12	// 多くの場合 HOME
			, KEY_UP	// 十字キー 上
			, KEY_RIGHT	// 十字キー 右
			, KEY_DOWN	// 十字キー 下
			, KEY_LEFT	// 十字キー 左
			, KEY_MAX
		};
		enum class eJoyStick {
			LEFT,
			RIGHT
		};


		// 初期化
		static void Initialize(HINSTANCE hinstance, HWND hwnd, HDC hdc, uint32_t window_w, uint32_t window_h);
		// 更新
		static void Update();
		// 解放
		static void Release();

		//------------------------------------------------------------------------------------------------------------------------------
		// キーボード押下検出
		// arg.... eKey::KB_***
		// ret.... [ 押下されている : true ] [ 押下されていない : false ]
		// tips... 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static bool IsKeyDown(tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			for (eKeys elem : array) {
				if (0x80 & keys_[kb_keys[static_cast<uint32_t>(elem)]]) return true;
			}
			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// キーボード押下時にコールバック関数を実行
		// arg1... コールバック関数
		// arg2... eKey::KB_***
		// tips1.. arg1 のコールバック関数の引数は arg2 の引数のインデックスが渡される
		// tips2.. 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static void RunIndexKeyDown(const std::function<void(uint32_t index)>& func, tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			uint32_t i = 0;
			for (eKeys elem : array) {
				if (0x80 & keys_[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// キーボード押下トリガー検出
		// arg.... eKey::KB_***
		// ret.... [ 押下されている : true ] [ 押下されていない : false ]
		// tips... 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static bool IsKeyDownTrigger(tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			for (eKeys elem : array) {
				if (0 != kb_trg_down[kb_keys[static_cast<uint32_t>(elem)]]) return true;
			}
			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// キーボード押下トリガー時にコールバック関数を実行
		// arg1... コールバック関数
		// arg2... eKey::KB_***
		// tips1.. arg1 のコールバック関数の引数は arg2 の引数のインデックスが渡される
		// tips2.. 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static void RunIndexKeyDownTrigger(const std::function<void(uint32_t index)>& func, tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			uint32_t i = 0;
			for (eKeys elem : array) {
				if (0 != kb_trg_down[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// 押下されていたキーボードを離した時のトリガー検出
		// arg.... eKey::KB_***
		// ret.... [ 離した瞬間 : true ] [ それ以外 : false ]
		// tips... 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static bool IsKeyReleaseTrigger(tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			for (eKeys elem : array) {
				if (0 != kb_trg_release[kb_keys[static_cast<uint32_t>(elem)]]) return true;
			}
			return false;
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// 押下されていたキーボードを離した時のトリガー時にコールバック関数を実行
		// arg1... コールバック関数
		// arg2... eKey::KB_***
		// tips1.. arg1 のコールバック関数の引数は arg2 の引数のインデックスが渡される
		// tips2.. 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tKeys>
		static void RunIndexKeyReleaseTrigger(const std::function<void(uint32_t index)>& func, tKeys... iParams)
		{
			const eKeys array[] = { static_cast<eKeys>(iParams)... };
			uint32_t i = 0;
			for (eKeys elem : array) {
				if (0 != kb_trg_release[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// ゲームパッド押下検出
		// arg.... ePad::KEY_***
		// ret.... [ 押下されている : true ] [ 押下されていない : false ]
		// tips... 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static bool IsPadDown(tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			for (ePad elem : array) {
				if (static_cast<uint32_t>(elem) <= static_cast<uint32_t>(ePad::KEY_12)) {
					if( 0x80 & joy_state_.rgbButtons[static_cast<uint32_t>(elem)] ) return true;
				}
				else if (static_cast<uint32_t>(elem) >= static_cast<uint32_t>(ePad::KEY_UP)) {
					DWORD pov0 = joy_state_.rgdwPOV[0];
					uint32_t key = static_cast<uint32_t>(elem) - static_cast<uint32_t>(ePad::KEY_UP);
					uint32_t limit[4][2] = { {31500, 4500}, {4500, 13500}, {13500, 22500}, {22500, 31500} };
					bool check = (limit[key][0] <= pov0 && limit[key][1] >= pov0);
					if (0 == key) check = (limit[key][0] <= pov0 || limit[key][1] >= pov0);
					if (-1 != pov0 && check) { return true; }
				}
			}
			return false;
		}
		//------------------------------------------------------------------------------------------------------------------------------
		// ゲームパッド押下時にコールバック関数を実行
		// arg1... コールバック関数
		// arg2... ePad::KEY_***
		// tips1.. arg1 のコールバック関数の引数は arg2 の引数のインデックスが渡される
		// tips2.. 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static void RunIndexPadDown(const std::function<void(uint32_t index)>& func, tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			uint32_t i = 0;
			for (ePad elem : array) {
				if (static_cast<uint32_t>(elem) <= static_cast<uint32_t>(ePad::KEY_12)) {
					if (0x80 & joy_state_.rgbButtons[static_cast<uint32_t>(elem)]) {
						func(i);
					}
				}
				else if (static_cast<uint32_t>(elem) >= static_cast<uint32_t>(ePad::KEY_UP)) {
					DWORD pov0 = joy_state_.rgdwPOV[0];
					uint32_t key = static_cast<uint32_t>(elem) - static_cast<uint32_t>(ePad::KEY_UP);
					uint32_t limit[4][2] = { {31500, 4500}, {4500, 13500}, {13500, 22500}, {22500, 31500} };
					bool check = (limit[key][0] <= pov0 && limit[key][1] >= pov0);
					if (0 == key) check = (limit[key][0] <= pov0 || limit[key][1] >= pov0);
					if (-1 != pov0 && check) {
						func(i);
					}
				}
				i++;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// ゲームパッド押下トリガー検出
		// arg.... ePad::KEY_***
		// ret.... [ 押下されている : true ] [ 押下されていない : false ]
		// tips... 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static bool IsPadDownTrigger(tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			for (ePad elem : array) {
				if (0 != pad_trg_down[static_cast<uint32_t>(elem)]) return true;
			}
			return false;
		}
		//------------------------------------------------------------------------------------------------------------------------------
		// ゲームパッド押下トリガー時にコールバック関数を実行
		// arg1... コールバック関数
		// arg2... ePad::KEY_***
		// tips1.. arg1 のコールバック関数の引数は arg2 の引数のインデックスが渡される
		// tips2.. 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static void RunIndexPadDownTrigger(const std::function<void(uint32_t index)>& func, tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			uint32_t i = 0;
			for (ePad elem : array) {
				if (0 != pad_trg_down[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}
		//------------------------------------------------------------------------------------------------------------------------------
		// 押下されていたゲームパッドを離した時のトリガー検出
		// arg.... ePad::KEY_***
		// ret.... [ 離した瞬間 : true ] [ それ以外 : false ]
		// tips... 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static bool IsPadReleaseTrigger(tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			for (ePad elem : array) {
				if (0 != pad_trg_release[static_cast<uint32_t>(elem)]) return true;
			}
			return false;
		}
		//------------------------------------------------------------------------------------------------------------------------------
		// 押下されていたゲームパッドを離した時のトリガー時にコールバック関数を実行
		// arg1... コールバック関数
		// arg2... ePad::KEY_***
		// tips1.. arg1 のコールバック関数の引数は arg2 の引数のインデックスが渡される
		// tips2.. 複数個の引数に対応
		//------------------------------------------------------------------------------------------------------------------------------
		template<typename... tPads>
		static void RunIndexPadReleaseTrigger(const std::function<void(uint32_t index)>& func, tPads... iParams)
		{
			const ePad array[] = { static_cast<ePad>(iParams)... };
			uint32_t i = 0;
			for (ePad elem : array) {
				if (0 != pad_trg_release[kb_keys[static_cast<uint32_t>(elem)]]) { func(i); }
				++i;
			}
		}

		//------------------------------------------------------------------------------------------------------------------------------
		// ゲームパッドの L2 R2 入力値取得
		// ret... 0.0f ～ 1.0f
		//------------------------------------------------------------------------------------------------------------------------------
		//static float GetPadL2();
		//static float GetPadR2();

		//------------------------------------------------------------------------------------------------------------------------------
		// ゲームパッドの 左右スティック入力値取得
		// ret... 各軸に対して -1.0f ～ 1.0f
		//------------------------------------------------------------------------------------------------------------------------------
		static tnl::Vector3 GetLeftStick();
		static tnl::Vector3 GetRightStick();


		// arg... tnl::Input::eMouse::LEFT
		// arg... tnl::Input::eMouse::RIGHT
		// arg... tnl::Input::eMouse::CENTER
		static bool IsMouseDown(eMouse _mouse);

		// arg... tnl::Input::eMouseTrigger::ON_LEFT
		// arg... tnl::Input::eMouseTrigger::OUT_RIGHT
		static bool IsMouseTrigger(eMouseTrigger _mouse);

		// マウス座標の取得
		static tnl::Vector3 GetMousePosition();
		// マウスホイールの変化量を取得
		static int32_t GetMouseWheel();
		// マウスを動かした時の変化量
		static tnl::Vector3 GetMouseVelocity();

	private:
		Input() {}

		static HWND hwnd_;
		static HDC hdc_;
		static HINSTANCE hinstance_;
		static uint32_t window_w_;
		static uint32_t window_h_;
		static LPDIRECTINPUTDEVICE8 key_;
		static LPDIRECTINPUTDEVICE8 mouse_;
		static LPDIRECTINPUTDEVICE8 joystick_ ;
		static DIJOYSTATE2 joy_state_;
		static DIMOUSESTATE2 ms_;
		static constexpr uint32_t KB_TBL_ARRAY_MAX = 256;
		static BYTE keys_[KB_TBL_ARRAY_MAX];
		static int kb_xor_down[KB_TBL_ARRAY_MAX] ;
		static int kb_trg_down[KB_TBL_ARRAY_MAX] ;
		static int kb_xor_release[KB_TBL_ARRAY_MAX] ;
		static int kb_trg_release[KB_TBL_ARRAY_MAX] ;
		static unsigned char pad_trg_down[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)];
		static unsigned char pad_xor_down[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)];
		static unsigned char pad_trg_release[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)];
		static unsigned char pad_xor_release[static_cast<uint32_t>(tnl::Input::ePad::KEY_MAX)];

		static unsigned char kb_keys[static_cast<uint32_t>(Input::eKeys::KB_MAX)];
		friend BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) noexcept;
		friend BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) noexcept;
	};


}

using ePad = tnl::Input::ePad;
using eKeys = tnl::Input::eKeys;
using eMouse = tnl::Input::eMouse;
using eMouseTrigger = tnl::Input::eMouseTrigger;

