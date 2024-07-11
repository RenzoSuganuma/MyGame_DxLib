#pragma once
#include "../dxlib_ext.h"
#include "../dxlib_ext_texture.h"
#include "dxlib_ext_mesh_base.h"
#include "dxlib_ext_mesh_desc.h"

namespace dxe {


	class MeshVbo final{
	public :
		~MeshVbo() {
			if (vb_hdl_) DeleteVertexBuffer(vb_hdl_);
			if (ib_hdl_) DeleteIndexBuffer(ib_hdl_);
		}
		int vb_hdl_ = 0;
		int ib_hdl_ = 0;
	};

	class Camera;
	class Mesh : public MeshBase {
	public :
		enum class eMeshFormat {
			MESH_FMT_PG,	// �v���O���}�u�����b�V��
			MESH_FMT_MV		// DxLib �p MV ���b�V��
		};
		enum class eShapeType {
			NONE
			,PLANE
			,CUBE
			,BOX
			,SPHERE
			,TRIANGLE_ISOSCELES
			,TRIANGLE_RIGHT
			,TRIANGLE_EQUILATERAL
			,DOME
			,CONE
			,CYLINDER
			,CONE_CYLINDER
			,DISK
			,DISK_RING
			,HOLLOW_OUT_DISK_PLANE
			,BARREL
			,HALF_BARREL
			,TORUS
			,HEIGHT_MAP
			,GROUP
			,RESOUCE
		};
		enum eTextureSlot {
			DIFFUSE
			, AMIBENT
			, SPECULAR
			, BUMP
			, HEIGHT
		};
		enum class fDebugLine {
			TNL_ENUM_BIT_FLAG(FLG_NONE),
			TNL_ENUM_BIT_FLAG(FLG_AXIS),
			TNL_ENUM_BIT_FLAG(FLG_OBB),			
			TNL_ENUM_BIT_FLAG(FLG_AABB),
			TNL_ENUM_BIT_FLAG(FLG_BD_SPHERE),
			TNL_ENUM_BIT_FLAG(FLG_VTX_NML) // pg mesh only
		};
	public :
		~Mesh() {
			if (mv_hdl_) MV1DeleteModel(mv_hdl_);
		}

		TNL_CALC_PROPERTY(tnl::Vector3, Position, pos_);
		TNL_CALC_PROPERTY(tnl::Vector3, Scaling, scl_);
		TNL_MUL_PROPERTY(tnl::Quaternion, Rotation, rot_);


		//==========================================================================================================================
		// 
		//
		// function
		//
		//
		void copySetting( Shared<Mesh> other );

		//------------------------------------------------------------------------------------------------------------------------
		// �N���[���̐���
		Shared<Mesh> createClone();


		//-----------------------------------------------------------------------------------------------------
		// Create*** �Ő��������v���O���}�u�����b�V���� X�t�@�C���t�H�[�}�b�g�Ń��\�[�X�Ƃ��ĕۑ�����
		// arg1... �ۑ���̃p�X
		// ret.... [ true : �ۑ����� ] [ false : �ۑ����s ]
		// tips... ���̋@�\���K�v�ɂȂ����o��
		//         DxLib �̋@�\�Ƃ��� MV*** �֐��̒��ɂ� DxLib ���Ή����Ă���t�H�[�}�b�g��
		//         ���f���f�[�^�����[�h���Ďg�p���Ȃ��Ƌ@�\���Ȃ��������邽�� ��.�V���h�E�}�b�v, �T���v���X�e�[�g�@�u�����h�X�e�[�g��
		bool exportForFileFormatX(const std::string& file_path);

		//-----------------------------------------------------------------------------------------------------
		// Create*** �Ő��������v���O���}�u�����b�V���� MV�t�@�C���t�H�[�}�b�g�Ń��\�[�X�Ƃ��ĕۑ�����
		// arg1... �ۑ���̃p�X
		// ret.... [ true : �ۑ����� ] [ false : �ۑ����s ]
		// tips... MV �̓o�C�i���`���Ȃ̂Ń��[�h�̍������[�u�Ƃ��ėp��
		bool exportForFileFormatMV(const std::string& file_path);


		//-----------------------------------------------------------------------------------------------------
		// �`��
		// arg1... �J����
		void render(const Shared<dxe::Camera> camera);

		//-----------------------------------------------------------------------------------------------------
		// �V���h�E�}�b�v����( �V���h�E�}�b�v�ւ̏������� )
		void reserveShadow();


		//==========================================================================================================================
		// 
		//
		// inline function
		//
		//

		inline int getDxMvHdl() { return mv_hdl_; }

		// �e�N�X�`���̐ݒ�
		inline void setTexture(std::shared_ptr<Texture> texture, uint32_t slot_num = DIFFUSE) {
			if (textures_.size() <= slot_num) {
				std::vector<std::shared_ptr<Texture>> new_buffer(slot_num+1);
				memcpy(new_buffer.data(), textures_.data(), sizeof(std::shared_ptr<Texture>) * textures_.size() );
				new_buffer[slot_num] = texture;
				textures_ = std::move(new_buffer);
			}
			else {
				textures_[slot_num] = texture;
			}
			if (eMeshFormat::MESH_FMT_MV == mesh_format_) MV1SetTextureGraphHandle(mv_hdl_, slot_num, texture->getDxLibGraphHandle(), true);
		}
		inline std::shared_ptr<Texture> getTexture(uint32_t slot_num = DIFFUSE) {
			if (textures_.empty()) return nullptr;
			return textures_[slot_num];
		}
		inline uint32_t getTextureSlotNum() { return (uint32_t)textures_.size(); }


		// ���O�̎擾
		inline void setName(const std::string& name) noexcept { mesh_name_ = name; }
		inline const std::string& getName() const noexcept { return mesh_name_; }

		// �f�o�b�O���C���\���ݒ�
		inline void setDebugLine(const fDebugLine f) { flg_dbg_line_ = f; }
		inline fDebugLine getDebugLine() { return flg_dbg_line_; }


		// �J�����O���[�h�ݒ�
		// arg1... DX_CULLING_***
		inline void setCullingMode(const int mode) noexcept { render_param_.setCullingMode(mode); }
		inline int getCullingMode() const noexcept { return render_param_.getCullingMode(); }

		// �u�����h���[�h�ݒ�
		// arg1... DX_BLENDMODE_***
		// tips... MV ���b�V���̂ݑΉ� ( dxlib �̎d�l )
		inline void setBlendMode(const int mode) noexcept { render_param_.setBlendMode(mode); }
		inline int getBlendMode() const noexcept { return render_param_.getBlendMode(); }

		// �e�N�X�`���T���v���t�B���^�ݒ�
		// arg1... DX_DRAWMODE_***
		inline void setSampleFilterMode(const int mode) noexcept { render_param_.setSampleFilterMode(mode); }
		inline int getSampleFilterMode() const noexcept { return render_param_.getSampleFilterMode(); }

		// �f�t�H���g���C�g�̗L����
		inline void setDefaultLightEnable(const bool f) noexcept { render_param_.setDefaultLightEnable(f); }
		inline int getDefaultLightEnable() const noexcept { return render_param_.getDefaultLightEnable(); }

		// �[�x�e�X�g�̗L����
		inline void setDepthTestEnable(const bool f) noexcept { render_param_.setDepthTestEnable(f); }
		inline bool getDepthTestEnable() const noexcept { return render_param_.getDepthTestEnable(); }

		// �[�x�o�b�t�@�ւ̏������ݗL����
		inline void setWriteDepthBufferEnable(const bool f) noexcept { render_param_.setWriteDepthBufferEnable(f); }
		inline bool getWriteDepthBufferEnable() const noexcept { return render_param_.getWriteDepthBufferEnable(); }

		// ���C���[�t���[���L����
		inline void setWireFrameEnable(const bool f) noexcept { render_param_.setWireFrameEnable(f); }
		inline bool getWireFrameEnable() const noexcept { return render_param_.getWireFrameEnable(); }

		// �A���r�G���g�J���[�ݒ�
		void setMtrlAmibent(const tnl::Vector3& col) noexcept override {
			render_param_.setMtrlAmbient(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialAmbColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Ambient);
		}

		// �f�B�t���[�Y�J���[�ݒ�
		void setMtrlDiffuse(const tnl::Vector3& col) noexcept override {
			render_param_.setMtrlDiffuse(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialDifColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Diffuse);
		}

		// �X�y�L�����J���[�ݒ�
		void setMtrlSpecular(const tnl::Vector3& col) noexcept override {
			render_param_.setMtrlSpecular(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Specular);
		}

		// �G�~�b�V���J���[�ݒ�
		void setMtrlEmissive(const tnl::Vector3& col) noexcept override {
			render_param_.setMtrlEmissive(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialEmiColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Emissive);
		}

		// �X�y�L�����p���[�ݒ�
		void setMtrlSpecPower(const float pow) noexcept override {
			render_param_.setMtrlSpecPower( pow );
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcPower(mv_hdl_, 0, render_param_.dxlib_mtrl_.Power);
		}

		// �ꊇ�ݒ�
		void setRenderParam( const RenderParam& param ) override {
			render_param_ = param;
			if (mesh_format_ != eMeshFormat::MESH_FMT_MV) return;
			MV1SetMaterialAmbColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Ambient);
			MV1SetMaterialDifColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Diffuse);
			MV1SetMaterialSpcColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Specular);
			MV1SetMaterialEmiColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Emissive);
			MV1SetMaterialSpcPower(mv_hdl_, 0, render_param_.dxlib_mtrl_.Power);
		}

		// ���_�C���f�b�N�X�̎擾
		inline const std::vector<uint32_t>& getIndexs() { return idxs_; }

		// ���_���W�̎擾
		// tips... [idx[0]] [idx[1]] [idx[2]] �ŎO�p�`�P���̒��_���W
		inline const std::vector<VERTEX3D>& getVertexs() { return vtxs_; }

		// �������̎擾
		inline const std::shared_ptr<MeshDesc> getCreateDesc() { return desc_; }

		// �`��̎擾
		inline eShapeType getShapeType() { return shape_type_; }

		// �o�E���f�B���O�X�t�B�A�̔��a���擾
		inline float getBoundingSphereRadius() { return bd_sphere_radius_; }

		// �o�E���f�B���O�{�b�N�X�T�C�Y���擾
		inline tnl::Vector3 getBoundingBoxSize() { return bd_box_size_; }

		// ���[���h�s��Ńg�����X�t�H�[���������[���h��Ԓ��_���W�̎擾
		// tips... [0][1][2] �ŎO�p�`�P���̒��_���W
		std::vector<tnl::Vector3> createWorldVertexs();


		//==========================================================================================================================
		//
		// 
		// static function
		// 
		// tips... �􉽊w�`��� Create *** MV �֐��� MV ���t���Ă�����̂Ɋւ���
		//         �����I�ɐ�������郁�b�V���� MV �Ή��ɂȂ�܂����A�������Ɉꎞ�I��
		// �@�@�@�@���t�@�C���t�H�[�}�b�g�ŕۑ������Ă���ă��[�h���s���Ă���̂Ő����ɂ͑����̃R�X�g���|����܂��B
		//         �����`��̃��b�V���𕡐���������ꍇ createClone �֐��𗘗p���Ă������� 
		//

		//-----------------------------------------------------------------------------------------------------
		// MV �`���̃��b�V���e�[�u���� MV�t�@�C���̏W���Ƃ��ĕۑ�����
		// arg1... �ۑ���̃p�X
		// arg2... MV���b�V���e�[�u��
		// ret.... [ true : �ۑ����� ] [ false : �ۑ����s ]
		// tips... ���[�h�̍������[�u�Ƃ��ėp��
		static bool ExportForFileFormatMVT(const std::string& file_path, std::vector<Mesh*>& mesh_tbl);

		//------------------------------------------------------------------------------------------------------------------------
		// �l�p�`
		// arg1... ���E���� ( z �s�g�p )  
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreatePlaneMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false, const tnl::Vector3& ltop_uv = { 0, 0, 0 }, const tnl::Vector3& rbottom_uv = { 1, 1, 0 }) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// ������
		// arg1... �T�C�Y
		// arg2... ��������
		// arg3... �c������
		// arg4... uv �덷�����p
		// arg5... �����C���f�b�N�X( �@�����]�p )
		// tips... UV �͊e�ʂւ̃e�N�X�`���������тɂȂ��Ă��鎖��z�肵�����ɂȂ��Ă��܂��B
		//         ���ʁE�E�E���E���E��E���̉����тɂȂ��Ă���e�N�X�`����p�ӂ��Ă�������
		static Shared<Mesh> CreateCubeMV(const float size, const int div_w = 5, const int div_h = 5, const float uv_adjusted = 0.005f, const bool is_left_cycle = false) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// �{�b�N�X 
		// arg1... �T�C�Y ( ��, ����, ���s )
		// arg2... ���ʃe�N�X�`��
		// arg3... �E�ʃe�N�X�`��
		// arg4... ��ʃe�N�X�`��
		// arg5... ���ʃe�N�X�`��
		// arg6... ���ʃe�N�X�`��
		// arg7... ��O�ʃe�N�X�`��
		// arg8... ��������
		// arg9... �c������
		// arg10... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateBoxMV(const tnl::Vector3& size, Shared<Texture> tx_left, Shared<Texture> tx_right, Shared<Texture> tx_up, Shared<Texture> tx_down, Shared<Texture> tx_forword, Shared<Texture> tx_back, const int div_w = 5, const int div_h = 5, const bool is_left_cycle = false) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// ��
		// arg1... ���a
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateSphereMV(const float radius, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// �񓙕ӎO�p�`
		// arg1... ���E���� ( z �s�g�p )  
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateTriangleIsoscelesMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// ���p�O�p�`
		// arg1... ���E���� ( z �s�g�p )  
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateTriangleRightMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// ���O�p�`
		// arg1... 1 �ӂ̒���
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateTriangleEquilateralMV(const float edge_lenght, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �h�[��
		// arg1... ���a
		// arg2... ��������
		// arg3... �c������
		// arg4... 360 �x �Ŋ��S�Ȕ���
		// arg5... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateDomeMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// ��ʂȂ��̉~��
		// arg1... ���a
		// arg2... ����
		// arg3... ��������
		// arg4... �c������
		// arg5... 360 �x �Œ�ʂ����S�ȉ~
		// arg6... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateConeMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �㉺�Ɍ��̋󂢂��~��
		// arg1... �㕔���a
		// arg2... �������a
		// arg3... ����
		// arg4... ��������
		// arg5... �c������
		// arg6... 360 �x �Œ�ʂ����S�ȉ~
		// arg7... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateConeCylinderMV(const float top_radius, const float bottom_radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �~��
		// arg1... ���a
		// arg2... ��������
		// arg3... �c������
		// arg4... 360 �x �Ŋ��S�ȉ~
		// arg5... �����C���f�b�N�X( �@�����]�p )
		// tips... 360�x 180�x 90�x �̂�
		static Shared<Mesh> CreateDiskMV(const float radius, const int div_w = 10, const int div_h = 10, const int angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �~�Տ�̃����O 
		// arg1... ���a
		// arg2... �����O�̑���
		// arg2... ��������
		// arg3... �c������
		// arg4... 360 �x �Ŋ��S�ȉ~
		// arg5... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateDiskRingMV(const float radius, const float thickness, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �~��ɂ��蔲�����l�p�`
		// arg1... ���a
		// arg2... ��������
		// arg3... �c������
		// arg4... 360 �x �ŉ~�����蔲���ꂽ�l�p�`
		// arg5... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateHollowOutDiskPlaneMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �~��
		// arg1... ���a
		// arg2... ����
		// arg3... ��������
		// arg4... �c������
		// arg5... 360 �x �ŉ~
		// arg6... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateCylinderMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �M
		// arg1... �㉺�̔��a
		// arg2... ����
		// arg3... �㉺�̔��a�ɑ΂���c��ݗ�
		// arg3... ��������
		// arg4... �c������
		// arg5... 360 �x �ŉ~
		// arg6... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �㔼���̒M
		// arg1... ��̔��a
		// arg2... ����
		// arg3... ��̔��a�ɑ΂���c��ݗ�
		// arg3... ��������
		// arg4... �c������
		// arg5... 360 �x �ŉ~
		// arg6... �����C���f�b�N�X( �@�����]�p )
		static Shared<Mesh> CreateHalfBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;


		//-----------------------------------------------------------------------------------------------------
		// �g�[���X
		// arg1... ���a
		// arg2... �����O�̑���
		// arg3... �ւ̕�����
		// arg4... 360 �x �ŉ~
		// tips... 360�x 180�x 90�x 45�x �̂�
		static Shared<Mesh> CreateTorusMV(const float radius, const float thickness, const int tessellation = 20, const int angle = 360) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �`��^�C�v���琶��
		// arg1... �`��^�C�v
		// arg2... �����p�f�[�^�N���X
		static Shared<Mesh> CreateFromTypeDesc(dxe::Mesh::eShapeType shpe_type, dxe::MeshDesc* desc) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �����}�b�v����쐬
		// arg1... �����Q�Ɨp�摜�̃p�X
		// arg2... ��
		// arg3... ���s��
		// arg4... �����̏���l
		// arg5... ��������
		// arg6... �c������
		static Shared<Mesh> CreateFromHeightMapMV(const std::string& file_path, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);
		static Shared<Mesh> CreateFromHeightMapMV(unsigned char* pixels, const int t_width, const int t_height, const int t_bpp, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);

		//-----------------------------------------------------------------------------------------------------
		// OBJ �`���t�@�C�����烁�b�V�����쐬
		// arg1... �t�B���p�X
		// arg2... �X�P�[��( �f�t�H���g1.0f )
		static std::vector<Shared<Mesh>> CreateFromFileObjMV(const std::string& file_path, const float scl = 1.0f);

		//-----------------------------------------------------------------------------------------------------
		// DXLib ���Ή����Ă���`���t�@�C�����烁�b�V�����쐬
		// arg1... �t�B���p�X
		// arg2... �X�P�[��( �f�t�H���g1.0f )
		static Shared<Mesh> CreateFromFileMV(const std::string& file_path, const float scl = 1.0f);

		//-----------------------------------------------------------------------------------------------------
		// �������b�V�����w��������Z�b�g�̃��b�V���C���X�^���X�ɂ���
		// arg1... ���₵�������b�V��
		// arg2... ���ꂼ��̃��[���h�s��
		// tips... CreateFromFileMV �֐��ɂ���Đ������ꂽ���b�V���͑ΏۊO�ł�
		static Shared<Mesh> CreateStaticMeshGroupMV(const Shared<Mesh>& increase_mesh, const std::vector<tnl::Matrix>& mesh_matrixs);

		//-----------------------------------------------------------------------------------------------------
		// �Ǝ��`���� MVT �t�@�C�����琶��
		// arg1... �t�B���p�X
		static std::vector<Shared<Mesh>> CreateFromFileMVT(const std::string& file_path);

	private:
		Mesh(){}

		int mv_hdl_ = 0;
		tnl::Vector3	pos_;
		tnl::Vector3	scl_ = { 1,1,1 };
		tnl::Quaternion rot_;

		std::shared_ptr<MeshVbo> vbo_			= nullptr;
		std::shared_ptr<MeshDesc> desc_			= nullptr;
		std::string mesh_name_;
		std::vector<std::shared_ptr<Texture>> textures_;

		float						bd_sphere_radius_ = 0;
		tnl::Vector3				bd_box_size_;
		std::vector<uint32_t>		idxs_;
		std::vector<VERTEX3D>		vtxs_;
		eMeshFormat					mesh_format_ = eMeshFormat::MESH_FMT_PG;
		eShapeType					shape_type_ = eShapeType::NONE;
		fDebugLine					flg_dbg_line_ = fDebugLine::FLG_NONE;

		void calcVertexNoraml();
		void createPlaneIndex(const int div_w, const int div_h, const bool is_left_cycle);
		void createVBO();
		void drawVtxNormal();
		std::string getXFormatString() noexcept ;

		static Shared<Mesh> CreateConvertMV(Shared<Mesh> mesh);
		static Shared<Mesh> CreatePlane(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false, const tnl::Vector3& ltop_uv = { 0, 0, 0 }, const tnl::Vector3& rbottom_uv = { 1, 1, 0 }) noexcept;
		static Shared<Mesh> CreateBox(const tnl::Vector3& size, Shared<Texture> tx_left, Shared<Texture> tx_right, Shared<Texture> tx_up, Shared<Texture> tx_down, Shared<Texture> tx_forword, Shared<Texture> tx_back, const int div_w = 5, const int div_h = 5, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateCube(const float size, const int div_w = 5, const int div_h = 5, const float uv_adjusted = 0.005f, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateSphere(const float radius, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateTriangleIsosceles(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateTriangleRight(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateTriangleEquilateral(const float edge_lenght, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateDome(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateCone(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateConeCylinder(const float top_radius, const float bottom_radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateDisk(const float radius, const int div_w = 10, const int div_h = 10, const int angle = 360, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateDiskRing(const float radius, const float thickness, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateHollowOutDiskPlane(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateCylinder(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateBarrel(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateHalfBarrel(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;
		static Shared<Mesh> CreateTorus(const float radius, const float thickness, const int tessellation = 20, const int angle = 360) noexcept;
		static Shared<Mesh> CreateFromHeightMap(const std::string& file_path, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);
		static Shared<Mesh> CreateFromHeightMap(unsigned char* pixels, const int t_width, const int t_height, const int t_bpp, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);
		static std::vector<Shared<Mesh>> CreateFromFileObj(const std::string& file_path, const float scl = 1.0f);
		friend class InstMeshPool ;
	} ;


	constexpr Mesh::fDebugLine operator | (Mesh::fDebugLine left, Mesh::fDebugLine right) {
		return static_cast<Mesh::fDebugLine>(static_cast<int32_t>(left) | static_cast<int32_t>(right));
	}
	constexpr bool operator & (Mesh::fDebugLine left, Mesh::fDebugLine right) {
		return static_cast<bool>(static_cast<int32_t>(left) & static_cast<int32_t>(right));
	}


	//----------------------------------------------------------------------------------------------------------------------
	/*
	*  MVT data format
	* 3      byte : "mvt"
	* 4      byte : sector num
	* 4      byte : all texture num
	* (a) 4  byte : tex name size
	* (b) 4  byte : tex data size
	* a      byte : tex name
	* b      byte : tex data
	* ... loop tex num  ( tex name size -> tex data )
	* 4       byte : sector size
	* (c) 4   byte : mesh name size
	* (d) 4   byte : mesh data size
	* c      byte : mesh name
	* d	     byte : mesh data
	* 4      byte : texture num
	* 4	     byte : tex slot
	* (e) 4  byte : tex name size
	* e      byte : tex name
	* ... loop texture num  ( tex slot -> tex name )
	* 12  byte : position xyz
	* 12  byte : scale xyz
	* 16  byte : rotation xyzw
	* ... loop sector num
	*/

}

