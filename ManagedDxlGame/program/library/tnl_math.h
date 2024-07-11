#pragma once

#include <vector>
#include <tuple>
#include <numbers>
#include "tnl_vector2i.h"
#include "tnl_vector3.h"

namespace tnl {


	//----------------------------------------------------------------------------------------------
	// �~����
	const float PI = 3.14159265358979f;

	//----------------------------------------------------------------------------------------------
	// ���W�A������f�O���[�ւ̕ϊ�
	inline float ToDegree(float radian) { return (radian / PI * 180.0f); }

	//----------------------------------------------------------------------------------------------
	// �f�O���[���烉�W�A���ւ̕ϊ�
	inline float ToRadian(float degree) { return (PI / 180.0f * degree); }

	//----------------------------------------------------------------------------------------------
	// ���W�ƃT�C�Y���� AABB �� max min ���擾
	inline tnl::Vector3 ToMaxAABB(const tnl::Vector3& pos, const tnl::Vector3& size) { return pos + (size * 0.5f); }
	inline tnl::Vector3 ToMinAABB(const tnl::Vector3& pos, const tnl::Vector3& size) { return pos - (size * 0.5f); }

	//----------------------------------------------------------------------------------------------
	// �����Z���k�c�C�X�^�[�����V�[�h�ݒ�
	void SetSeedMersenneTwister32(int seed);	

	//----------------------------------------------------------------------------------------------
	// �������������̎擾
	// tips... ���̊֐����g�p����ꍇ SetSeedMersenneTwister32 �ŗ����V�[�h��ݒ肵�Ă�������
	float GetRandomDistributionFloat(float min, float max);


	//----------------------------------------------------------------------------------------------
	// work... �w����W�����ʂ̕\���ǂ���ɑ��݂��邩���肷��
	// arg1... �w����W
	// arg2... ���ʂ̖@��
	// arg3... ���ʏ�ɑ��݂���ǂ����̍��W
	// ret.... �\...1 	��...-1		���ʏ�...0
	// tips... none
	int GetSidesPointAndPlane(const Vector3& v, const Vector3& pn, const Vector3& pv);

	//----------------------------------------------------------------------------------------------
	enum class eRegionPtRect { UP, RIGHT, DOWN, LEFT, MAX };
	// work... ��`�̒��S����Q�{�̖����ɐL�т�Ίp������������
	//         �w�肵�����W���Ίp������㉺���E�ǂ��ɑ��݂��邩�擾����
	// arg1... �w����W
	// arg2... ��`�̒��S���W
	// arg3... ��`�̕�
	// arg4... ��`�̍���
	eRegionPtRect GetRegionPointAndRect(const Vector3& p, const Vector3& rp, const int rect_w, const int rect_h);

	//----------------------------------------------------------------------------------------------
	enum class eRegionPtAABB { DOWN, RIGHT, UP, LEFT, BACK, FORWORD, MAX };
	// work... AABB�̒��S����S�Ă̑Ίp�ӂ֖����ɐL�т镽�ʂ��l�����Ƃ�
	//         �w�肵�����W���Ίp�ӗ̈�̏㉺���E�O��ǂ��ɑ��݂��邩�擾����
	// arg1... �w����W
	// arg2... AABB max
	// arg3... AABB min
	eRegionPtAABB GetRegionPointAndAABB( const Vector3& p, const Vector3& max, const Vector3& min );

	//----------------------------------------------------------------------------------------------
	enum class eRegionPtXzOBB{ FORWORD, RIGHT, BACK, LEFT, MAX };
	// work... OBB �̃��[�J����Ԃɑ΂���Y���ɐ����ȕ��ʂ� OBB �̑Ίp�ɂQ�� �N���X������
	//         �w�肵�����W���Ίp�ʂ���O�㍶�E�ǂ��ɑ��݂��邩�擾����
	// arg1... �w����W
	// arg2... OBB �̒��S���W
	// arg3... OBB �̕�, ����, ���s
	// arg4... OBB �̎p��
	eRegionPtXzOBB GetXzRegionPointAndOBB(const Vector3& p, const Vector3& op, const Vector3& size, const Quaternion& q);

	//----------------------------------------------------------------------------------------------
	// work... �_��AABB��AABB���ɑ��݂���_�ւ̍ŋߐړ_���擾
	// arg1... �_���W
	// arg2... AABB max
	// arg3... AABB min
	// ret... �ŋߐړ_
	tnl::Vector3 GetNearestPointAABB(const tnl::Vector3& point, const tnl::Vector3& aabb_max, const tnl::Vector3& aabb_min );

	//----------------------------------------------------------------------------------------------
	// work... �_�Ƌ�`�̋�`���ɑ��݂���_�ւ̍ŋߐړ_���擾
	// arg1... ��`���W
	// arg2... ��`�̕�
	// arg3... ��`�̍���
	// arg4... �_���W
	// ret... �ŋߐړ_( 2D )
	tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, float w, float h, const tnl::Vector3 &point);

	//----------------------------------------------------------------------------------------------
	// name... GetNearestRectPoint
	// work... �_�ƒ����`�̒����`���ɑ��݂���_�ւ̍ŋߐړ_���擾
	// arg1... �����`�̒��S
	// arg2... �����`��X���̒P�ʃx�N�g��
	// arg3... �����`��Y���̒P�ʃx�N�g��
	// arg4... �����`�̕ӂ̒����̔���
	// arg5... �_���W
	// ret... �ŋߐړ_( 3D )
	tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, const tnl::Vector3& axis_x, const tnl::Vector3& axis_y, const tnl::Vector3& rect_side_length, const tnl::Vector3& point);

	//----------------------------------------------------------------------------------------------
	// work... �_�ƎO�p�`�̎O�p�`��ɑ��݂���ŋߓ_
	// arg1... �_���W
	// arg234. �O�p�`�̒��_���W
	// arg5... �O�p�`��̍��W
	tnl::Vector3 GetNearestPointTriangle(const tnl::Vector3& p, const tnl::Vector3& a, const tnl::Vector3& b, const tnl::Vector3& c);

	//----------------------------------------------------------------------------------------------
	// work... �_�ƕ��ʂ̕��ʏ�ɑ��݂���ŋߓ_
	// arg1... �_���W
	// arg2... ���ʂ̖@��
	// arg3... ���ʏ�̍��W
	tnl::Vector3 GetNearestPointPlane(const tnl::Vector3& v, const tnl::Vector3& pn, const tnl::Vector3& pv);

	//----------------------------------------------------------------------------------------------
	// work... �_�Ɛ����̐�����ɑ��݂���ŋߓ_
	// arg1... �_���W
	// arg2... �n�_
	// arg3... �I�_
	tnl::Vector3 GetNearestPointLine(const tnl::Vector3& v, const tnl::Vector3& a, const tnl::Vector3& b);

	//----------------------------------------------------------------------------------------------
	// work... �������m�̍ŋߓ_
	// arg1... �n�_A
	// arg2... �n�_A
	// arg3... �n�_B
	// arg4... �n�_B
	// ret.... <0> ����A�̍ŋߓ_
	// ....... <1> ����B�̍ŋߓ_
	std::tuple<tnl::Vector3, tnl::Vector3> GetNearestLines(const tnl::Vector3& s1, const tnl::Vector3& e1, const tnl::Vector3& s2, const tnl::Vector3& e2);

	//----------------------------------------------------------------------------------------------
	// ���ʂɑ��݂��郉���_���ȓ_���擾����
	// arg1... ���ʂ��\�����镽�ʓ��̓_
	// arg2... ���ʂ̖@��
	// arg3... arg1 ���畽�ʓ��ɑ��݂���~�̔��a ( ���̔��a���Ń����_���ȓ_�������� )
	// ret.... �����_���ȓ_���W
	tnl::Vector3 GetRandomPointInsidePlane(const tnl::Vector3& plane_position, const tnl::Vector3& plane_normal, float radius);

	//----------------------------------------------------------------------------------------------
	enum class eCorrTypeRect { PWRFL_A, PWRFL_B, BOTH, MAX };
	enum class eCorrResRect { LEFT, RIGHT, UP, DOWN, MAX };
	// ��`���m�̍��W�␳
	// arg1.2. ��` A, B �̈ړ��O�̍��W
	// arg3.4. ��` A, B �̃T�C�Y
	// arg5.6. ��` A, B �̈ړ���̍��W ( ���̊֐��ɂ���ĕ␳��̍��W�ɍX�V )
	// arg7.8. ��, �c �����̕␳�^�C�v 
	// ....... [ PWRFL_A : A �� ���� ] [ PWRFL_B : B �� ���� ] [ BOTH : A �� B �͉������� ]
	// arg9... �␳��� A �� B �̊Ԃɐ݂����� ( �f�t�H���g 1.0f )
	// ret.... ��` A �� B �ɑ΂��Ăǂ̕����֕␳���ꂽ�� 
	eCorrResRect CorrectPositionRect(
		const tnl::Vector3& a_before
		, const tnl::Vector3& b_before
		, const tnl::Vector2i& a_size
		, const tnl::Vector2i& b_size
		, tnl::Vector3& a_out
		, tnl::Vector3& b_out
		, eCorrTypeRect correct_type_x
		, eCorrTypeRect correct_type_y
		, float correct_space = 1.0f);


	//----------------------------------------------------------------------------------------------
	enum class eCorrTypeAABB { PWRFL_A, PWRFL_B, BOTH, MAX };
	enum class eCorrResAABB { LEFT, RIGHT, UP, DOWN, FORWORD, BACK, MAX };
	// AABB ���m�̍��W�␳
	// arg1.2.		A, B �̈ړ��O�̍��W
	// arg3.4.		A, B �̃T�C�Y
	// arg5.6.		A, B �̈ړ���̍��W ( ���̊֐��ɂ���ĕ␳��̍��W�ɍX�V )
	// arg7.8.9.	��, �c, �� �����̕␳�^�C�v
	// .......		[ PWRFL_A : A �� ���� ] [ PWRFL_B : B �� ���� ] [ BOTH : A �� B �͉������� ]
	// arg10..		�␳��� A �� B �̊Ԃɐ݂����� ( �f�t�H���g 1.0f )
	// ret....		��` A �� B �ɑ΂��Ăǂ̕����֕␳���ꂽ��
	eCorrResAABB CorrectPositionAABB(
		const tnl::Vector3& a_before
		, const tnl::Vector3& b_before
		, const tnl::Vector3& a_size
		, const tnl::Vector3& b_size
		, tnl::Vector3& a_out
		, tnl::Vector3& b_out
		, eCorrTypeAABB correct_type_x
		, eCorrTypeAABB correct_type_y
		, eCorrTypeAABB correct_type_z
		, float correct_space = 1.0f);


	//----------------------------------------------------------------------------------------------
	// ��`�Ɖ~�̍��W�␳ ( �~�̂ݕ␳ )
	// arg1...		��`�̒��S���W
	// arg2...		��`�T�C�Y
	// arg3...		�~�̈ړ��O���W
	// arg4...		�~�̔��a
	// arg5...		�~�̌��ݍ��W ( �␳���W�ŏ㏑������܂� )
	// arg6...		�␳��Ԃɐ݂����� ( �f�t�H���g 1.0f )
	// ret....		�~����`��̍ŋߓ_����ǂ̕����֕␳���ꂽ�� ( ���K���x�N�g�� )
	tnl::Vector3 CorrectPositionRectCircle(
		const tnl::Vector3& rect_center
		, const tnl::Vector2i& rect_size
		, const tnl::Vector3& circle_before
		, const float circle_radius
		, tnl::Vector3& circle_out
		, float correct_space = 1.0f);


	//----------------------------------------------------------------------------------------------
	// �����Ɖ~�̍��W�␳ ( �~�̂ݕ␳ )
	// arg1...		�����̎n�_���W
	// arg2...		�����̏I�_���W
	// arg3...		�~�̈ړ��O���W
	// arg4...		�~�̔��a
	// arg5...		�~�̌��ݍ��W ( �␳���W�ŏ㏑������܂� )
	// arg6...		�␳��Ԃɐ݂����� ( �f�t�H���g 1.0f )
	// ret....		�~�������̍ŋߓ_����ǂ̕����֕␳���ꂽ�� ( ���K���x�N�g�� )
	tnl::Vector3 CorrectPositionLineCircle(
		const tnl::Vector3& line_s
		, const tnl::Vector3& line_e
		, const tnl::Vector3& circle_before
		, const float circle_radius
		, tnl::Vector3& circle_out
		, float correct_space = 1.0f);


	//----------------------------------------------------------------------------------------------
	// ���Z�p�`�̕ӂ̒����𕝂����Ɏ擾
	// arg1... ���Z�p�`�̕�
	// ret.... �ӂ̒���
	float GetHexagonEdgeLenght(float width) ;


	//----------------------------------------------------------------------------------------------
	 // ���Z�p�`�̍����𕝂���擾
	 // arg1... ���Z�p�`�̕�
	 // ret.... �ӂ̒���
	float GetHexagonHeight(float width);


	//----------------------------------------------------------------------------------------------
	// ���Z�p�`�̒��_���W���擾
	// arg1... ���Z�p�`�̕�
	// ret.... ���_���W�z��
	std::vector<tnl::Vector3> GetHexagonVertices(float width);


	//----------------------------------------------------------------------------------------------
	// �O�p�`�̒��_����O�ډ~�̒��S���W�Ɣ��a�����߂�
	// arg... �O�p�`�̒��_���W a, b, c
	// ret... <0>...���S���W  <1>... ���a
	std::tuple<tnl::Vector3, float> CircumscribedCircle(const tnl::Vector3& a, const tnl::Vector3& b, const tnl::Vector3& c);

	//----------------------------------------------------------------------------------------------
	// ���p�O�p�`�ɂ����ė^����ꂽ�����Ɗp�x����Εӂ��v�Z����
	// arg1... ��ӂƎΕӂ���Ȃ�p�x (���W�A��)
	// arg2... ����
	// ret.... �Εӂ̒���
	inline float RightTriangleHypotByHeight(float theta, float height) {
		if (height <= 0.0 || theta <= 0.0 || theta >= std::numbers::pi / 2.0f) return -1;
		return height / sinf(theta);
	}
	//----------------------------------------------------------------------------------------------
	// ���p�O�p�`�ɂ����ė^����ꂽ��ӂƊp�x����Εӂ̒������v�Z����
	// arg1... ��ӂƎΕӂ���Ȃ�p�x (���W�A��)
	// arg2... ��ӂ̒���
	// ret.... �Εӂ̒���
	inline float RightTriangleHypotByBase(float theta, float base) {
		if (base <= 0.0 || theta <= 0.0 || theta >= std::numbers::pi / 2.0f) return -1;
		return base / cosf(theta);
	}	
	//----------------------------------------------------------------------------------------------
	// ���p�O�p�`�ɂ����ė^����ꂽ�����Ɗp�x�����ӂ̒������v�Z����
	// arg1... ��ӂƎΕӂ���Ȃ�p�x (���W�A��)
	// arg2... ����
	// ret.... ��ӂ̒���
	inline float RightTriangleBaseByHeight(float theta, float height) {
		if (height <= 0.0 || theta <= 0.0 || theta >= std::numbers::pi / 2.0f) return -1;
		return height / tanf(theta);
	}
	//----------------------------------------------------------------------------------------------
	// ���p�O�p�`�ɂ����ė^����ꂽ�p�x�ƒ�ӂ̒������獂�����v�Z����
	// arg1... ��ӂƎΕӂ���Ȃ�p�x (���W�A��)
	// arg2... ��ӂ̒���
	// ret.... ����
	inline float RightTriangleHeightByBase(float theta, float base) {
		if (base <= 0.0 || theta <= 0.0 || theta >= std::numbers::pi / 2.0f) return -1;
		return base * tanf(theta);
	}

	//----------------------------------------------------------------------------------------------
	// �u���Z���n���̐������W�e�[�u���擾
	// arg1... �n�_
	// arg2... �I�_
	// ret.... �������W�e�[�u��
	std::vector<tnl::Vector2i> GetBresenhamsLine(const tnl::Vector2i& st, const tnl::Vector2i& en);


	//----------------------------------------------------------------------------------------------
	// �_���W���������L�΂������A���p�`���\������ӂɑ΂��Č��������񐔂��擾
	// arg1... ���p�`�̒��_
	// arg2... �_���W
	// ret.... ������
	uint32_t CountPolygonIntersections(const std::vector<tnl::Vector2i>& polygon_vertexs, const tnl::Vector2i& point);


	//----------------------------------------------------------------------------------------------
	// �q���������̏W���ɑ΂��ė]�����������J�b�g����
	// arg1... �����\������_���W�Q
	// arg2... �[�_���q�����Ă��Ȃ��ꍇ�Ɍq���邩�ǂ���
	void CutOverLineLoop( std::vector<tnl::Vector2i>& line_points, bool is_add_loop_point = true );


	//----------------------------------------------------------------------------------------------
	// ���ʂƖ��C����d�S�ƈړ��x�N�g���̍X�V���s���܂�
	// arg1... �d�S�~�̔��a
	// arg2... ���� ( 0 �` 100.0f )
	// arg3... ���C�W�� ( 0 �` 1.0f )
	// arg4... �O��̈ړ��x�N�g�� ( ���̊֐��ōX�V����܂� )
	// arg5... ����̈ړ��x�N�g�� ( ���̊֐��ōX�V����܂� )
	// arg6... �d�S���W
	// tips... �ȈՌv�Z�ł��蕨���w�ɏ����������̂ł͂���܂���
	void EasyAdjustObjectVelocity( float centroid_radius, float mass, float friction,
									tnl::Vector3& past_move_vel, tnl::Vector3& move_vel, tnl::Vector3& center_of_gravity);

	//----------------------------------------------------------------------------------------------
	enum class eOscStart { STOK, CENTER };
	// �P�U���ɂ��U���ʒu�̌v�Z
	// arg1... �U���J�n�ʒu [ STOK : �[ ] [ CENTER : ���� ]
	// arg2... ���� : �傫���قǋ�����������
	// arg3... �U�����x : �傫���قǑ����U������
	// arg4... �o�ߎ���
	// ret.... �U���ʒu
	// tips... STOK ���w�肵���ꍇ : 0 �` 2 �̊ԂŐU���� 1 �Ɏ���
	// ....... CENTER ���w�肵���ꍇ : -1 �` +1 �̊ԂŐU���� 0 �Ɏ���
	// ....... STOK CENTER ���� 0 ���琳�����֒l���U��
	float SingleOscillationy( eOscStart osc_start, float decay, float period, float progress_time);

	//----------------------------------------------------------------------------------------------
	// ���Ԋu�Ȓl�̕��
	// arg1... �n�l
	// arg2... �I�l
	// arg3... ��ԕ�Ԋ����܂ł̎���
	// arg4... �o�ߎ���
	// ret.... ��Ԓl
	float UniformLerp(float s, float e, float time_limit, float ct);


	//----------------------------------------------------------------------------------------------
	 // sin �g -90 ~ +90�x�܂ł𗘗p�����l�̕��
	 // arg1... �n�l
	 // arg2... �I�l
	 // arg3... ��Ԋ����܂ł̎���
	 // arg4... �o�ߎ���
	 // arg5... �����Ŏ��Ԃɑ΂����Ԃ��s���� ( �f�t�H���g��0�� �ő�5�� )
	 // ret.... ��Ԓl
	 // tips... �ʏ�� sin �g�ɂ���Ԃł͊Ԋu�����P�����Ǝv���ꍇ
	 // ....... arg5 �̈����ŕ�Ԃ̊Ԋu���L���A���G�b�W�̌�������Ԃ��s�����Ƃ��ł��܂�
	float SmoothLerp(float s, float e, float time_limit, float ct, int strength = 0);


	//----------------------------------------------------------------------------------------------
	// �����������^���𗘗p�����l�̕��
	// arg1... �n�l
	// arg2... �I�l
	// arg3... ��Ԋ����܂ł̎���
	// arg4... �o�ߎ���
	// arg5... �����Ŏ��Ԃɑ΂����Ԃ��s���� ( �f�t�H���g��0�� �ő�5�� )
	// ret.... ��Ԓl
	// tips... �ʏ�� �������x�ɂ���Ԃł͊Ԋu�����P�����Ǝv���ꍇ
	// ....... arg5 �̈����Ŏw���֐��I�ɑ��x���オ��悤�����ł��܂�
	float AccelLerp(float s, float e, float time_limit, float ct, int strength = 0);

	//----------------------------------------------------------------------------------------------
	// �����������^���𗘗p�����l�̕��
	// arg1... �n�l
	// arg2... �I�l
	// arg3... ��Ԋ����܂ł̎���
	// arg4... �o�ߎ���
	// ret.... ��Ԓl
	float DecelLerp(float s, float e, float time_limit, float ct);


	//----------------------------------------------------------------------------------------------
	// �x�W�F�X�v���C�����
	// arg1... ��Ԏn�_
	// arg2... anker
	// arg3... anker
	// arg4... ��ԏI�_
	// arg5... 0.0f �` 1.0f
	// ret.... ��ԍ��W
	Vector3 BezierSpline(const Vector3& _a1, const Vector3& _a2, const Vector3& _a3, const Vector3& _a4, float t);

	//----------------------------------------------------------------------------------------------
	//
	// �O���X�v���C�����
	//
	class CubicSpline final {
	private:
		std::vector<tnl::Vector3> a_;
		std::vector<tnl::Vector3> b_;
		std::vector<tnl::Vector3> c_;
		std::vector<tnl::Vector3> d_;
		std::vector<tnl::Vector3> w_;

	public:
		CubicSpline() {};
		CubicSpline(const std::vector<tnl::Vector3>& v);

		//---------------------------------------------------------------------------
		// ��ԍ��W�̎擾
		// arg1... 0.0f �` 1.0f
		// ret.... ��ԍ��W
		inline tnl::Vector3 getPosition(float t) {
			float rt = float(a_.size() - 1) * t;
			int p = int(floor(rt));
			float dt = rt - p;
			return a_[p] + (b_[p] + (c_[p] + d_[p] * dt) * dt) * dt;
		}

	};

	//----------------------------------------------------------------------------------------------
	//
	// ���W�Q���`���
	//
	class PointsLerp final {
	private:
		float all_length_ = 0;
		std::vector< tnl::Vector3 > points_;
		std::vector< float > section_lengths_;
		float getLengthRate(float t) const noexcept;
		float getLengthPoint(int pn) const noexcept;
		int getPrevPoint(float t) const noexcept;

	public:

		PointsLerp(const std::vector< tnl::Vector3 >& points);

		//---------------------------------------------------------------------------
		// ��ԍ��W�̎擾
		// arg1... 0.0f �` 1.0f
		// ret.... ��ԍ��W
		inline tnl::Vector3 getPosition(const float t) {
			int n = getPrevPoint(t);
			float len = getLengthRate(t) - getLengthPoint(n);
			tnl::Vector3 v = tnl::Vector3::Normalize((points_[n + 1] - points_[n]));
			return points_[n] + (v * len);
		}

	};


	//epsilon surrounding for near zero values 
	const double EQN_EPS = 1e-9;
	inline double IsZeroD(double x) { return ((x) > -EQN_EPS && (x) < EQN_EPS); }
	std::vector<double> Solve2(std::vector<double>& c);
	std::vector<double> Solve3(std::vector<double>& c);
	std::vector<double> Solve4(std::vector<double>& c);

}