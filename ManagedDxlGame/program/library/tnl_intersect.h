#pragma once
#include "tnl_matrix.h"

namespace tnl {

	//-----------------------------------------------------------------------------------------------------
	// �_�Ƌ�`(�����`)�̂̏Փˌ��m
	// arg1...�_�̂����W
	// arg2...�_�̂����W
	// arg3...��`�̒��S�����W
	// arg4...��`�̒��S�����W
	// arg5...��`�̃T�C�Y X
	// arg5...��`�̃T�C�Y Y
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	bool IsIntersectPointRect(const int point_x, const int point_y, const int rect_x, const int rect_y, const int rect_size_x, const int rect_size_y);

	//-----------------------------------------------------------------------------------------------------
	// ��`�Ƌ�`�̏Փˌ��m ����`�̍���E�E���̒��_���W�w��
	// arg1...��`A�̍��̂����W
	// arg2...��`A�̉E�̂����W
	// arg3...��`A�̏�̂����W
	// arg4...��`A�̉��̂����W
	// arg5...��`B�̍��̂����W
	// arg6...��`B�̉E�̂����W
	// arg7...��`B�̏�̂����W
	// arg8...��`B�̉��̂����W
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	bool IsIntersectRectPrimitive(const int a_left, const int a_right, const int a_top, const int a_bottom,
		const int b_left, const int b_right, const int b_top, const int b_bottom);

	//-----------------------------------------------------------------------------------------------------
	// ��`�Ƌ�`�̏Փˌ��m�@����`�̒��S�ƕ������w��
	// arg1...��`A�̂����W
	// arg2...��`A�̂����W
	// arg3...��`A�̕�
	// arg4...��`A�̍���
	// arg5...��`B�̂����W
	// arg6...��`B�̂����W
	// arg7...��`B�̕�
	// arg8...��`B�̍���
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	bool IsIntersectRect(const int a_rect_x, const int a_rect_y, const int a_rect_size_w, const int a_rect_size_h,
		const int b_rect_x, const int b_rect_y, const int b_rect_size_w, const int b_rect_size_h);

	//-----------------------------------------------------------------------------------------------------
	// ��`�Ƌ�`�̏Փˌ��m�@����`�̒��S�ƕ������w��
	// arg1...��`A�̍��W
	// arg2...��`A�̕�
	// arg3...��`A�̍���
	// arg4...��`B�̍��W
	// arg5...��`B�̕�
	// arg6...��`B�̍���
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	inline bool IsIntersectRect(const tnl::Vector3& a, const int a_rect_size_w, const int a_rect_size_h,
		const tnl::Vector3& b, const int b_rect_size_w, const int b_rect_size_h) {
		return IsIntersectRect((int)a.x, (int)a.y, a_rect_size_w, a_rect_size_h, (int)b.x, (int)b.y, b_rect_size_w, b_rect_size_h);
	}
 
	//----------------------------------------------------------------------------------------------
	// work... ��`�Ɖ~�̏Փ˔���
	// arg1... ��`�̒��S���W
	// arg2... ��`�T�C�Y
	// arg3... �~�̍��W
	// arg4... �~�̔��a
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	bool IsIntersectRectCircle(const tnl::Vector3& rect_center, const tnl::Vector2i& rect_size, const tnl::Vector3& circle_pos, float circle_radius);


	//----------------------------------------------------------------------------------------------
	// work... �����Ɖ~�̏Փ˔���
	// arg1... �����̎n�_���W
	// arg2... �����̏I�_���W
	// arg3... �~�̍��W
	// arg4... �~�̔��a
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	bool IsIntersectLineCircle(const tnl::Vector3& line_s, const tnl::Vector3& line_e, const tnl::Vector3& circle_pos, float radius);


	//----------------------------------------------------------------------------------------------
	// work... AABB ���m�̏Փ˔���
	// arg1... A ���W
	// arg2... A �̃T�C�Y
	// arg3... B �̍��W
	// arg4... B �̃T�C�Y
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	bool IsIntersectAABB( const tnl::Vector3& a, const tnl::Vector3& a_size, const tnl::Vector3& b, const tnl::Vector3& b_size );


	//----------------------------------------------------------------------------------------------
	// work... 2D��̐����̔���
	// arg1... ����A�̎n�_
	// arg2... ����A�̏I�_
	// arg3... ����B�̎n�_
	// arg4... ����B�̏I�_
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... none
	bool IsIntersectLine2D(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);

	//----------------------------------------------------------------------------------------------
	// work... �O�p�`�Ɛ����̏Փ˔���
	// arg1-3. �O�p�`���\�����钸�_
	// arg5-5. �n�_�ƏI�_ 
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... 2D �p
	bool IsIntersectLineTriangle2D(const Vector3& _v1, const Vector3& _v2, const Vector3& _v3, const Vector3& _s, const Vector3& _e);


	//-----------------------------------------------------------------------------------------------------
	// ���Ƌ��̏Փˌ��m
	// arg1...�~A�̒��S���W
	// arg2...�~A�̔��a
	// arg3...�~B�̒��S���W
	// arg4...�~B�̔��a
	// ret....[ �Փ˂��Ă��� : true ]   [ �Փ˂��Ă��Ȃ� : false ]
	bool IsIntersectSphere(const tnl::Vector3& a, float a_radius, const tnl::Vector3& b, float b_radius);

	//----------------------------------------------------------------------------------------------
	// work... �����ƕ��ʂ̏Փ˔���
	// arg1... �����̎n�_
	// arg2... �����̏I�_
	// arg3... ���ʏ�̍��W ( 0, 0, 0 �ȊO )
	// arg4... ���ʂ̖@��
	// arg5... �Փ˓_�̎󂯎��p( �K�v�Ȃ���Ώȗ��� )
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... �Փ˂��Ă��Ȃ��ꍇ arg5 �œn���������͕ω��Ȃ�
	bool IsIntersectLinePlane(const tnl::Vector3 &line_start, const tnl::Vector3 &line_end,
		const tnl::Vector3 &plane_pos, const tnl::Vector3 &plane_normal,
		tnl::Vector3* intersect_position = nullptr);

	//----------------------------------------------------------------------------------------------
	// work... �O�p�`�Ɠ_�̔���
	// arg1-3. �O�p�`�̒��_���W
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... none
	bool IsIntersectTrianglePoint(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& p);

	//----------------------------------------------------------------------------------------------
	// work... �O�p�`�Ɛ����̏Փ˔���
	// arg1-3. �O�p�`���\�����钸�_
	// arg2-4. �n�_�ƏI�_ 
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	// tips... 3D �p
	bool IsIntersectLineTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& s, const Vector3& e);


	//----------------------------------------------------------------------------------------------
	// work... ���C��AABB �̏Փ˔���
	// arg1... ���C���W
	// arg2... ���C�x�N�g��
	// arg3... ������̍��W
	// arg4... �E�O���̍��W
	// arg7... ���Z���ʂ̌�_
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	bool IsIntersectRayAABB(const Vector3& pos, const Vector3& dir, const Vector3& aabb_max, const Vector3& aabb_min, Vector3* intersect_pos = nullptr);

	//----------------------------------------------------------------------------------------------
	// work... ���C��OBB �̏Փ˔���
	// arg1... ���C���W ( �����Ȃ�n�_ )
	// arg2... ���C�x�N�g��
	// arg3... �{�b�N�X��AABB �Ƃ������� ������̍��W
	// arg4... �{�b�N�X��AABB �Ƃ������� �E�O���̍��W
	// arg5... OBB �̉�]�s��
	// arg7... ���Z���ʂ̌�_
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	bool IsIntersectRayOBB( const Vector3& pos, const Vector3& dir, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3* intersect_pos = nullptr );

	//----------------------------------------------------------------------------------------------
	// work... ������OBB �̏Փ˔���
	// arg1... �n�_
	// arg2... �I�_
	// arg3... OBB ����]���Ă��Ȃ� AABB �Ƃ������� max
	// arg4... OBB ����]���Ă��Ȃ� AABB �Ƃ������� min
	// arg5... OBB �̎p��
	// arg6... ���Z���ʂ̌�_
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	bool IsIntersectLineOBB(const Vector3& s, const Vector3& e, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3* intersect_pos = nullptr );

	//----------------------------------------------------------------------------------------------
	// work... ������OBB �̏Փ˔��� 2D�p
	// arg1... �n�_
	// arg2... �I�_
	// arg3... �{�b�N�X���W
	// arg4... OBB ����]���Ă��Ȃ� AABB �Ƃ������� max
	// arg5... OBB ����]���Ă��Ȃ� AABB �Ƃ������� min
	// arg6... OBB �̎p��
	// arg7... ���Z���ʂ̌�_
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	bool IsIntersectLineOBB2D(const Vector3& s, const Vector3& e, const Vector3& pos, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3* intersect_pos = nullptr);

	//----------------------------------------------------------------------------------------------
	// work... ���C�ƃg�[���X�̏Փ˔���
	// arg1... ���C���W
	// arg2... ���C�x�N�g��
	// arg3... �g�[���X���S���W
	// arg4... �g�[���X�p��
	// arg5... �g�[���X�̗ւ̔��a
	// arg6... �g�[���X�S�̂̔��a
	// ret.... [�Փ˂��Ă��� : true] [�Փ˂��Ă��Ȃ� : false]
	bool IsIntersectRayTorus(const Vector3& s, const Vector3& dir, const Vector3& tp, const Quaternion& tq, const float tube_r, const float swept_r);
	bool IsIntersectRayTorus(const Vector3& s, const Vector3& dir, const Vector3& tp, const Quaternion& tq, const float tube_r, const float swept_r, Vector3* intersect_pos = nullptr );

}
