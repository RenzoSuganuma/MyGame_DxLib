#pragma once
#pragma warning(disable:4819)
#include "../library/tnl_util.h"
#include "../library/tnl_csv.h"
#include "../library/tnl_font_texture.h"
#include "../library/tnl_hierarchy_tree.h"
#include "../library/tnl_input.h"
#include "../library/tnl_intersect.h"
#include "../library/tnl_link_linear.h"
#include "../library/tnl_math.h"
#include "../library/tnl_matrix.h"
#include "../library/tnl_quaternion.h"
#include "../library/tnl_sequence.h"
#include "../library/tnl_shared_factory.h"
#include "../library/tnl_vector2i.h"
#include "../library/tnl_vector2f.h"
#include "../library/tnl_vector3.h"
#include "../library/tnl_seek_unit.h"
#include "../library/tnl_rect.h"
#include "../library/stb_image.h"
#include "../library/json11.hpp"
#include "DxLib.h"
#include "dxlib_ext_data.h"
#include "dxlib_ext_camera.h"
#include "mesh/dxlib_ext_mesh.h"
#include "mesh/dxlib_ext_inst_mesh.h"
#include "dxlib_ext_texture.h"
#include "gui/dxlib_ext_gui_value_slider.h"
#include "gui/dxlib_ext_gui_menu_selector.h"
#include "gui/dxlib_ext_gui_drag_box.h"
#include "gui/dxlib_ext_gui_input_box.h"
#include "gui/dxlib_ext_gui_check_box.h"
#include "effects/dxlib_ext_shadow_map.h"
#include "effects/dxlib_ext_particle.h"
#include "effects/dxlib_ext_screen_effect.h"
#include "effects/dxlib_ext_water_plane.h"

// ウィンドウの幅
const int DXE_WINDOW_WIDTH = 1280;
// ウィンドウの高さ
const int DXE_WINDOW_HEIGHT = 720;
// 固定フレームレート
const float DXE_FIX_FPS = 60.0f;

// 文字描画
void DrawStringEx(float x, float y, int color, const char* _str, ...);
void DrawStringEx(int x, int y, int color, const char* _str, ...);
void DrawStringToHandleEx(const float x, const float y, const int color, const int hdl, const char* _str, ...);

// ライン描画
void DrawLineEx(const tnl::Vector3& p, const tnl::Vector3& v, const float length, const int color = -1);
// 矩形描画
void DrawBoxEx(const tnl::Vector3& p, const float width, const float height, const bool fill = true, const int color = -1);
void DrawRotaBox(const tnl::Vector3& pos, float w, float h, float rad, int color = -1);


// FPS インジケータ描画
void DrawFpsIndicator(const tnl::Vector3& pos, float delta_time);


//------------------------------------------------------------------------------------------------------------
// 正六角形を描画
// arg1... 正六角の中心座標
// arg2... 正六角の幅
// arg3... 色 ( 16進数指定 0xffffffff で白 )
void DrawHexagon(const tnl::Vector3 pos, float width, int color = -1);

//----------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ用のグリッド地面
// arg1... カメラ
// arg2... グリッドマスのサイズ
// arg3... グリッドを並べる個数 ( 偶数個で指定 )
// arg4... 16進数カラー
// tips... この関数を使用する前にカメラを生成して update 関数を実行してください
void DrawGridGround( const Shared<dxe::Camera> camera, const float square_size, int row_num, int color = 0x99999999 );

//----------------------------------------------------------------------------------------------------------------------------------------------------
// 正六角形を敷き詰めた地面を描画
// arg1... カメラ
// arg2... 正六角形の幅
// arg3... 縦横に何個並べるか
// arg4... XYZ 軸を描画するか
// arg5... 色
// tips... この関数を使用する前にカメラを生成して update 関数を実行してください
// tips... 軽い処理ではないのでデバッグ用途にしてください
void DrawHexagonGround(const Shared<dxe::Camera> camera, const float hex_width, int row_num, bool is_draw_center_line = true, int color = 0x99999999);

// デフォルトライトパラメータをリソースから設定
// arg1... DrawDefaultLightGuiController で出力したバイナリファイルパス
void SetDefaultLightParameter(const std::string& file_path);

// デフォルトライトパラメータをコントロールするGUIを描画
void DrawDefaultLightGuiController();
































//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawAxis(const Shared<dxe::Camera> camera, const tnl::Vector3& pos, const tnl::Quaternion& rot, const float length);
void DrawOBB(const Shared<dxe::Camera> camera, const tnl::Vector3& pos, const tnl::Quaternion& rot, const tnl::Vector3& size, const int color = 0x00ff00ff);
void DrawAABB(const Shared<dxe::Camera> camera, const tnl::Vector3& pos, const tnl::Vector3& size, const int color = 0x0000ffff);
void DrawBdSphere(const Shared<dxe::Camera> camera, const tnl::Vector3& pos, const float radius);
void DrawLine3DEx(const Shared<dxe::Camera> camera, const tnl::Vector3& st, const tnl::Vector3& en, const int color = -1);
std::string GetDragFilePathReadTrigger();

void GetLookAtCameraFactor(const float axis_x_angle, const float axis_y_angle, const float distance, tnl::Vector3& out_target_to_position);
void GetFreeLookCameraFactor(const tnl::Vector3& position, const tnl::Vector3& target, float& out_axis_x_angle, float& out_axis_y_angle, float& out_distance);
void GetTransformCameraFactor(const tnl::Vector3& position, const tnl::Vector3& target, const tnl::Vector3& up, tnl::Quaternion& out_rot);

namespace dxe {
	float GetDeltaTime();
	void DirectXInitialize();
	void DirectXRenderBegin();
	void DirectXRenderEnd();
	void DirectXRelease();

	ID3D11DepthStencilState* GetDepthStencilState(const eDepthStenclil mode);
	ID3D11BlendState* GetBlendState(const eBlendState state);
	ID3D11SamplerState* GetSamplerState(const eSamplerState state);
	ID3D11RasterizerState* GetRasterizerState(const eRasterizerState state);
}

/*

class GameCamera : public dxe::Camera {
public :

	GameCamera(){};
	GameCamera(int screen_w, int screen_h) : dxe::Camera(screen_w, screen_h){}

	enum class eCtrlMode {
		LOOK_AT, FREE_LOOK, TRANSFORM
	};
	inline void setControllMode( eCtrlMode mode ) noexcept { ctrl_mode_ = mode; }
	inline eCtrlMode getControllMode()const noexcept { return ctrl_mode_; }

	void update();


	void lookAtToOther();
	void freeLookToOther();
	void transformToOhter();


	eCtrlMode ctrl_mode_ = eCtrlMode::FREE_LOOK;

	// mode freelook param
	float to_target_distance_ = 300 ;
	float axis_x_angle_ = tnl::ToRadian(45);
	float axis_y_angle_ = 0;

	// mode transform param
	tnl::Quaternion rot_;

};


void GameCamera::lookAtToOther() {
	rot_ = tnl::Quaternion::LookAt(pos_, target_, up_);

	tnl::Vector3 invlook = pos_ - target_;
	to_target_distance_ = invlook.length();
	tnl::Vector3 vx = tnl::Vector3::Normalize(invlook.xz() );
	tnl::Vector3 vy = tnl::Vector3::Normalize(invlook);
	axis_y_angle_ = (invlook.x < 0)? vx.angle({ 0, 0, -1 }) : -vx.angle({ 0, 0, -1 });
	axis_x_angle_ = (invlook.y < 0)? -vy.angle(vx) : vy.angle(vx) ;
}

void GameCamera::freeLookToOther() {
	tnl::Quaternion  q = tnl::Quaternion::RotationAxis( {0, 1, 0}, axis_y_angle_);
	tnl::Vector3 vz = tnl::Vector3::TransformCoord({ 0, 0, 1 }, q);
	q *= tnl::Quaternion::RotationAxis(tnl::Vector3::Cross({0, 1, 0}, vz), axis_x_angle_);
	pos_ = target_ + tnl::Vector3::TransformCoord({ 0, 0, -to_target_distance_ }, q);

	rot_ = tnl::Quaternion::LookAt(pos_, target_, up_);
}

void GameCamera::transformToOhter() {

	target_ = pos_ + tnl::Vector3::TransformCoord({ 0, 0, to_target_distance_ }, rot_);

	tnl::Vector3 invlook = pos_ - target_;
	to_target_distance_ = invlook.length();
	tnl::Vector3 vx = tnl::Vector3::Normalize(invlook.xz());
	tnl::Vector3 vy = tnl::Vector3::Normalize(invlook);
	axis_y_angle_ = (invlook.x < 0) ? vx.angle({ 0, 0, -1 }) : -vx.angle({ 0, 0, -1 });
	axis_x_angle_ = (invlook.y < 0) ? -vy.angle(vx) : vy.angle(vx);

}


void GameCamera::update() {
	switch ( ctrl_mode_ ) {
	case eCtrlMode::LOOK_AT :
		lookAtToOther();
		break;
	case eCtrlMode::FREE_LOOK :
		freeLookToOther();
		break;
	case eCtrlMode::TRANSFORM:
		transformToOhter();
		break;
	}
	dxe::Camera::update();
}
*/


