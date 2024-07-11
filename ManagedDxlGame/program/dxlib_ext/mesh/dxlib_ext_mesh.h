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
			MESH_FMT_PG,	// プログラマブルメッシュ
			MESH_FMT_MV		// DxLib 用 MV メッシュ
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
		// クローンの生成
		Shared<Mesh> createClone();


		//-----------------------------------------------------------------------------------------------------
		// Create*** で生成したプログラマブルメッシュを Xファイルフォーマットでリソースとして保存する
		// arg1... 保存先のパス
		// ret.... [ true : 保存成功 ] [ false : 保存失敗 ]
		// tips... この機能が必要になった経緯
		//         DxLib の機能として MV*** 関数の中には DxLib が対応しているフォーマットの
		//         モデルデータをロードして使用しないと機能しない物があるため 例.シャドウマップ, サンプラステート　ブレンドステート等
		bool exportForFileFormatX(const std::string& file_path);

		//-----------------------------------------------------------------------------------------------------
		// Create*** で生成したプログラマブルメッシュを MVファイルフォーマットでリソースとして保存する
		// arg1... 保存先のパス
		// ret.... [ true : 保存成功 ] [ false : 保存失敗 ]
		// tips... MV はバイナリ形式なのでロードの高速化措置として用意
		bool exportForFileFormatMV(const std::string& file_path);


		//-----------------------------------------------------------------------------------------------------
		// 描画
		// arg1... カメラ
		void render(const Shared<dxe::Camera> camera);

		//-----------------------------------------------------------------------------------------------------
		// シャドウマップ準備( シャドウマップへの書き込み )
		void reserveShadow();


		//==========================================================================================================================
		// 
		//
		// inline function
		//
		//

		inline int getDxMvHdl() { return mv_hdl_; }

		// テクスチャの設定
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


		// 名前の取得
		inline void setName(const std::string& name) noexcept { mesh_name_ = name; }
		inline const std::string& getName() const noexcept { return mesh_name_; }

		// デバッグライン表示設定
		inline void setDebugLine(const fDebugLine f) { flg_dbg_line_ = f; }
		inline fDebugLine getDebugLine() { return flg_dbg_line_; }


		// カリングモード設定
		// arg1... DX_CULLING_***
		inline void setCullingMode(const int mode) noexcept { render_param_.setCullingMode(mode); }
		inline int getCullingMode() const noexcept { return render_param_.getCullingMode(); }

		// ブレンドモード設定
		// arg1... DX_BLENDMODE_***
		// tips... MV メッシュのみ対応 ( dxlib の仕様 )
		inline void setBlendMode(const int mode) noexcept { render_param_.setBlendMode(mode); }
		inline int getBlendMode() const noexcept { return render_param_.getBlendMode(); }

		// テクスチャサンプラフィルタ設定
		// arg1... DX_DRAWMODE_***
		inline void setSampleFilterMode(const int mode) noexcept { render_param_.setSampleFilterMode(mode); }
		inline int getSampleFilterMode() const noexcept { return render_param_.getSampleFilterMode(); }

		// デフォルトライトの有効化
		inline void setDefaultLightEnable(const bool f) noexcept { render_param_.setDefaultLightEnable(f); }
		inline int getDefaultLightEnable() const noexcept { return render_param_.getDefaultLightEnable(); }

		// 深度テストの有効化
		inline void setDepthTestEnable(const bool f) noexcept { render_param_.setDepthTestEnable(f); }
		inline bool getDepthTestEnable() const noexcept { return render_param_.getDepthTestEnable(); }

		// 深度バッファへの書き込み有効化
		inline void setWriteDepthBufferEnable(const bool f) noexcept { render_param_.setWriteDepthBufferEnable(f); }
		inline bool getWriteDepthBufferEnable() const noexcept { return render_param_.getWriteDepthBufferEnable(); }

		// ワイヤーフレーム有効化
		inline void setWireFrameEnable(const bool f) noexcept { render_param_.setWireFrameEnable(f); }
		inline bool getWireFrameEnable() const noexcept { return render_param_.getWireFrameEnable(); }

		// アンビエントカラー設定
		void setMtrlAmibent(const tnl::Vector3& col) noexcept override {
			render_param_.setMtrlAmbient(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialAmbColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Ambient);
		}

		// ディフューズカラー設定
		void setMtrlDiffuse(const tnl::Vector3& col) noexcept override {
			render_param_.setMtrlDiffuse(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialDifColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Diffuse);
		}

		// スペキュラカラー設定
		void setMtrlSpecular(const tnl::Vector3& col) noexcept override {
			render_param_.setMtrlSpecular(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Specular);
		}

		// エミッシヴカラー設定
		void setMtrlEmissive(const tnl::Vector3& col) noexcept override {
			render_param_.setMtrlEmissive(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialEmiColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Emissive);
		}

		// スペキュラパワー設定
		void setMtrlSpecPower(const float pow) noexcept override {
			render_param_.setMtrlSpecPower( pow );
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcPower(mv_hdl_, 0, render_param_.dxlib_mtrl_.Power);
		}

		// 一括設定
		void setRenderParam( const RenderParam& param ) override {
			render_param_ = param;
			if (mesh_format_ != eMeshFormat::MESH_FMT_MV) return;
			MV1SetMaterialAmbColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Ambient);
			MV1SetMaterialDifColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Diffuse);
			MV1SetMaterialSpcColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Specular);
			MV1SetMaterialEmiColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Emissive);
			MV1SetMaterialSpcPower(mv_hdl_, 0, render_param_.dxlib_mtrl_.Power);
		}

		// 頂点インデックスの取得
		inline const std::vector<uint32_t>& getIndexs() { return idxs_; }

		// 頂点座標の取得
		// tips... [idx[0]] [idx[1]] [idx[2]] で三角形１枚の頂点座標
		inline const std::vector<VERTEX3D>& getVertexs() { return vtxs_; }

		// 生成情報の取得
		inline const std::shared_ptr<MeshDesc> getCreateDesc() { return desc_; }

		// 形状の取得
		inline eShapeType getShapeType() { return shape_type_; }

		// バウンディングスフィアの半径を取得
		inline float getBoundingSphereRadius() { return bd_sphere_radius_; }

		// バウンディングボックスサイズを取得
		inline tnl::Vector3 getBoundingBoxSize() { return bd_box_size_; }

		// ワールド行列でトランスフォームしたワールド空間頂点座標の取得
		// tips... [0][1][2] で三角形１枚の頂点座標
		std::vector<tnl::Vector3> createWorldVertexs();


		//==========================================================================================================================
		//
		// 
		// static function
		// 
		// tips... 幾何学形状の Create *** MV 関数で MV が付いているものに関して
		//         内部的に生成されるメッシュは MV 対応になりますが、生成時に一時的に
		// 　　　　ｘファイルフォーマットで保存をしてから再ロードを行っているので生成には相応のコストが掛かります。
		//         同じ形状のメッシュを複数生成する場合 createClone 関数を利用してください 
		//

		//-----------------------------------------------------------------------------------------------------
		// MV 形式のメッシュテーブルを MVファイルの集合として保存する
		// arg1... 保存先のパス
		// arg2... MVメッシュテーブル
		// ret.... [ true : 保存成功 ] [ false : 保存失敗 ]
		// tips... ロードの高速化措置として用意
		static bool ExportForFileFormatMVT(const std::string& file_path, std::vector<Mesh*>& mesh_tbl);

		//------------------------------------------------------------------------------------------------------------------------
		// 四角形
		// arg1... 幅・高さ ( z 不使用 )  
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreatePlaneMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false, const tnl::Vector3& ltop_uv = { 0, 0, 0 }, const tnl::Vector3& rbottom_uv = { 1, 1, 0 }) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// 立方体
		// arg1... サイズ
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... uv 誤差調整用
		// arg5... 左回りインデックス( 法線反転用 )
		// tips... UV は各面へのテクスチャが横並びになっている事を想定した作りになっています。
		//         正面・右・奥・左・上・下の横並びになっているテクスチャを用意してください
		static Shared<Mesh> CreateCubeMV(const float size, const int div_w = 5, const int div_h = 5, const float uv_adjusted = 0.005f, const bool is_left_cycle = false) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// ボックス 
		// arg1... サイズ ( 幅, 高さ, 奥行 )
		// arg2... 左面テクスチャ
		// arg3... 右面テクスチャ
		// arg4... 上面テクスチャ
		// arg5... 下面テクスチャ
		// arg6... 奥面テクスチャ
		// arg7... 手前面テクスチャ
		// arg8... 横分割数
		// arg9... 縦分割数
		// arg10... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateBoxMV(const tnl::Vector3& size, Shared<Texture> tx_left, Shared<Texture> tx_right, Shared<Texture> tx_up, Shared<Texture> tx_down, Shared<Texture> tx_forword, Shared<Texture> tx_back, const int div_w = 5, const int div_h = 5, const bool is_left_cycle = false) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// 球
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateSphereMV(const float radius, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// 二等辺三角形
		// arg1... 幅・高さ ( z 不使用 )  
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateTriangleIsoscelesMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 直角三角形
		// arg1... 幅・高さ ( z 不使用 )  
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateTriangleRightMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 正三角形
		// arg1... 1 辺の長さ
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateTriangleEquilateralMV(const float edge_lenght, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// ドーム
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で完全な半球
		// arg5... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateDomeMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 底面なしの円錐
		// arg1... 半径
		// arg2... 高さ
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で底面が完全な円
		// arg6... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateConeMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 上下に穴の空いた円錐
		// arg1... 上部半径
		// arg2... 下部半径
		// arg3... 高さ
		// arg4... 横分割数
		// arg5... 縦分割数
		// arg6... 360 度 で底面が完全な円
		// arg7... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateConeCylinderMV(const float top_radius, const float bottom_radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円盤
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で完全な円
		// arg5... 左回りインデックス( 法線反転用 )
		// tips... 360度 180度 90度 のみ
		static Shared<Mesh> CreateDiskMV(const float radius, const int div_w = 10, const int div_h = 10, const int angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円盤状のリング 
		// arg1... 半径
		// arg2... リングの太さ
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で完全な円
		// arg5... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateDiskRingMV(const float radius, const float thickness, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円状にくり抜いた四角形
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で円がくり抜かれた四角形
		// arg5... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateHollowOutDiskPlaneMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円筒
		// arg1... 半径
		// arg2... 高さ
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で円
		// arg6... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateCylinderMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 樽
		// arg1... 上下の半径
		// arg2... 高さ
		// arg3... 上下の半径に対する膨らみ率
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で円
		// arg6... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 上半分の樽
		// arg1... 上の半径
		// arg2... 高さ
		// arg3... 上の半径に対する膨らみ率
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で円
		// arg6... 左回りインデックス( 法線反転用 )
		static Shared<Mesh> CreateHalfBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = false) noexcept;


		//-----------------------------------------------------------------------------------------------------
		// トーラス
		// arg1... 半径
		// arg2... リングの太さ
		// arg3... 輪の分割数
		// arg4... 360 度 で円
		// tips... 360度 180度 90度 45度 のみ
		static Shared<Mesh> CreateTorusMV(const float radius, const float thickness, const int tessellation = 20, const int angle = 360) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 形状タイプから生成
		// arg1... 形状タイプ
		// arg2... 生成用データクラス
		static Shared<Mesh> CreateFromTypeDesc(dxe::Mesh::eShapeType shpe_type, dxe::MeshDesc* desc) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 高さマップから作成
		// arg1... 高さ参照用画像のパス
		// arg2... 幅
		// arg3... 奥行き
		// arg4... 高さの上限値
		// arg5... 横分割数
		// arg6... 縦分割数
		static Shared<Mesh> CreateFromHeightMapMV(const std::string& file_path, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);
		static Shared<Mesh> CreateFromHeightMapMV(unsigned char* pixels, const int t_width, const int t_height, const int t_bpp, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);

		//-----------------------------------------------------------------------------------------------------
		// OBJ 形式ファイルからメッシュを作成
		// arg1... フィルパス
		// arg2... スケール( デフォルト1.0f )
		static std::vector<Shared<Mesh>> CreateFromFileObjMV(const std::string& file_path, const float scl = 1.0f);

		//-----------------------------------------------------------------------------------------------------
		// DXLib が対応している形式ファイルからメッシュを作成
		// arg1... フィルパス
		// arg2... スケール( デフォルト1.0f )
		static Shared<Mesh> CreateFromFileMV(const std::string& file_path, const float scl = 1.0f);

		//-----------------------------------------------------------------------------------------------------
		// 同じメッシュを指定個数ワンセットのメッシュインスタンスにする
		// arg1... 増やしたいメッシュ
		// arg2... それぞれのワールド行列
		// tips... CreateFromFileMV 関数によって生成されたメッシュは対象外です
		static Shared<Mesh> CreateStaticMeshGroupMV(const Shared<Mesh>& increase_mesh, const std::vector<tnl::Matrix>& mesh_matrixs);

		//-----------------------------------------------------------------------------------------------------
		// 独自形式の MVT ファイルから生成
		// arg1... フィルパス
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

