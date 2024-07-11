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

// �E�B���h�E�̕�
const int DXE_WINDOW_WIDTH = 1280;
// �E�B���h�E�̍���
const int DXE_WINDOW_HEIGHT = 720;
// �Œ�t���[�����[�g
const float DXE_FIX_FPS = 60.0f;

// �����`��
void DrawStringEx(float x, float y, int color, const char* _str, ...);
void DrawStringEx(int x, int y, int color, const char* _str, ...);
void DrawStringToHandleEx(const float x, const float y, const int color, const int hdl, const char* _str, ...);

// ���C���`��
void DrawLineEx(const tnl::Vector3& p, const tnl::Vector3& v, const float length, const int color = -1);
// ��`�`��
void DrawBoxEx(const tnl::Vector3& p, const float width, const float height, const bool fill = true, const int color = -1);
void DrawRotaBox(const tnl::Vector3& pos, float w, float h, float rad, int color = -1);


// FPS �C���W�P�[�^�`��
void DrawFpsIndicator(const tnl::Vector3& pos, float delta_time);


//------------------------------------------------------------------------------------------------------------
// ���Z�p�`��`��
// arg1... ���Z�p�̒��S���W
// arg2... ���Z�p�̕�
// arg3... �F ( 16�i���w�� 0xffffffff �Ŕ� )
void DrawHexagon(const tnl::Vector3 pos, float width, int color = -1);

//----------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�p�̃O���b�h�n��
// arg1... �J����
// arg2... �O���b�h�}�X�̃T�C�Y
// arg3... �O���b�h����ׂ�� ( �����Ŏw�� )
// arg4... 16�i���J���[
// tips... ���̊֐����g�p����O�ɃJ�����𐶐����� update �֐������s���Ă�������
void DrawGridGround( const Shared<dxe::Camera> camera, const float square_size, int row_num, int color = 0x99999999 );

//----------------------------------------------------------------------------------------------------------------------------------------------------
// ���Z�p�`��~���l�߂��n�ʂ�`��
// arg1... �J����
// arg2... ���Z�p�`�̕�
// arg3... �c���ɉ����ׂ邩
// arg4... XYZ ����`�悷�邩
// arg5... �F
// tips... ���̊֐����g�p����O�ɃJ�����𐶐����� update �֐������s���Ă�������
// tips... �y�������ł͂Ȃ��̂Ńf�o�b�O�p�r�ɂ��Ă�������
void DrawHexagonGround(const Shared<dxe::Camera> camera, const float hex_width, int row_num, bool is_draw_center_line = true, int color = 0x99999999);

// �f�t�H���g���C�g�p�����[�^�����\�[�X����ݒ�
// arg1... DrawDefaultLightGuiController �ŏo�͂����o�C�i���t�@�C���p�X
void SetDefaultLightParameter(const std::string& file_path);

// �f�t�H���g���C�g�p�����[�^���R���g���[������GUI��`��
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


