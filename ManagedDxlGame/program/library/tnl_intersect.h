#pragma once
#include "tnl_matrix.h"

namespace tnl {

	//-----------------------------------------------------------------------------------------------------
	// 点と矩形(正方形)のの衝突検知
	// arg1...点のｘ座標
	// arg2...点のｙ座標
	// arg3...矩形の中心ｘ座標
	// arg4...矩形の中心ｙ座標
	// arg5...矩形のサイズ X
	// arg5...矩形のサイズ Y
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	bool IsIntersectPointRect(const int point_x, const int point_y, const int rect_x, const int rect_y, const int rect_size_x, const int rect_size_y);

	//-----------------------------------------------------------------------------------------------------
	// 矩形と矩形の衝突検知 ※矩形の左上・右下の頂点座標指定
	// arg1...矩形Aの左のｘ座標
	// arg2...矩形Aの右のｘ座標
	// arg3...矩形Aの上のｙ座標
	// arg4...矩形Aの下のｙ座標
	// arg5...矩形Bの左のｘ座標
	// arg6...矩形Bの右のｘ座標
	// arg7...矩形Bの上のｙ座標
	// arg8...矩形Bの下のｙ座標
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	bool IsIntersectRectPrimitive(const int a_left, const int a_right, const int a_top, const int a_bottom,
		const int b_left, const int b_right, const int b_top, const int b_bottom);

	//-----------------------------------------------------------------------------------------------------
	// 矩形と矩形の衝突検知　※矩形の中心と幅高さ指定
	// arg1...矩形Aのｘ座標
	// arg2...矩形Aのｙ座標
	// arg3...矩形Aの幅
	// arg4...矩形Aの高さ
	// arg5...矩形Bのｘ座標
	// arg6...矩形Bのｙ座標
	// arg7...矩形Bの幅
	// arg8...矩形Bの高さ
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	bool IsIntersectRect(const int a_rect_x, const int a_rect_y, const int a_rect_size_w, const int a_rect_size_h,
		const int b_rect_x, const int b_rect_y, const int b_rect_size_w, const int b_rect_size_h);

	//-----------------------------------------------------------------------------------------------------
	// 矩形と矩形の衝突検知　※矩形の中心と幅高さ指定
	// arg1...矩形Aの座標
	// arg2...矩形Aの幅
	// arg3...矩形Aの高さ
	// arg4...矩形Bの座標
	// arg5...矩形Bの幅
	// arg6...矩形Bの高さ
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	inline bool IsIntersectRect(const tnl::Vector3& a, const int a_rect_size_w, const int a_rect_size_h,
		const tnl::Vector3& b, const int b_rect_size_w, const int b_rect_size_h) {
		return IsIntersectRect((int)a.x, (int)a.y, a_rect_size_w, a_rect_size_h, (int)b.x, (int)b.y, b_rect_size_w, b_rect_size_h);
	}
 
	//----------------------------------------------------------------------------------------------
	// work... 矩形と円の衝突判定
	// arg1... 矩形の中心座標
	// arg2... 矩形サイズ
	// arg3... 円の座標
	// arg4... 円の半径
	// ret.... [衝突している : true] [衝突していない : false]
	bool IsIntersectRectCircle(const tnl::Vector3& rect_center, const tnl::Vector2i& rect_size, const tnl::Vector3& circle_pos, float circle_radius);


	//----------------------------------------------------------------------------------------------
	// work... 線分と円の衝突判定
	// arg1... 線分の始点座標
	// arg2... 線分の終点座標
	// arg3... 円の座標
	// arg4... 円の半径
	// ret.... [衝突している : true] [衝突していない : false]
	bool IsIntersectLineCircle(const tnl::Vector3& line_s, const tnl::Vector3& line_e, const tnl::Vector3& circle_pos, float radius);


	//----------------------------------------------------------------------------------------------
	// work... AABB 同士の衝突判定
	// arg1... A 座標
	// arg2... A のサイズ
	// arg3... B の座標
	// arg4... B のサイズ
	// ret.... [衝突している : true] [衝突していない : false]
	bool IsIntersectAABB( const tnl::Vector3& a, const tnl::Vector3& a_size, const tnl::Vector3& b, const tnl::Vector3& b_size );


	//----------------------------------------------------------------------------------------------
	// work... 2D上の線分の判定
	// arg1... 線分Aの始点
	// arg2... 線分Aの終点
	// arg3... 線分Bの始点
	// arg4... 線分Bの終点
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... none
	bool IsIntersectLine2D(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);

	//----------------------------------------------------------------------------------------------
	// work... 三角形と線分の衝突判定
	// arg1-3. 三角形を構成する頂点
	// arg5-5. 始点と終点 
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... 2D 用
	bool IsIntersectLineTriangle2D(const Vector3& _v1, const Vector3& _v2, const Vector3& _v3, const Vector3& _s, const Vector3& _e);


	//-----------------------------------------------------------------------------------------------------
	// 球と球の衝突検知
	// arg1...円Aの中心座標
	// arg2...円Aの半径
	// arg3...円Bの中心座標
	// arg4...円Bの半径
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	bool IsIntersectSphere(const tnl::Vector3& a, float a_radius, const tnl::Vector3& b, float b_radius);

	//----------------------------------------------------------------------------------------------
	// work... 線分と平面の衝突判定
	// arg1... 線分の始点
	// arg2... 線分の終点
	// arg3... 平面上の座標 ( 0, 0, 0 以外 )
	// arg4... 平面の法線
	// arg5... 衝突点の受け取り用( 必要なければ省略可 )
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... 衝突していない場合 arg5 で渡した引数は変化なし
	bool IsIntersectLinePlane(const tnl::Vector3 &line_start, const tnl::Vector3 &line_end,
		const tnl::Vector3 &plane_pos, const tnl::Vector3 &plane_normal,
		tnl::Vector3* intersect_position = nullptr);

	//----------------------------------------------------------------------------------------------
	// work... 三角形と点の判定
	// arg1-3. 三角形の頂点座標
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... none
	bool IsIntersectTrianglePoint(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& p);

	//----------------------------------------------------------------------------------------------
	// work... 三角形と線分の衝突判定
	// arg1-3. 三角形を構成する頂点
	// arg2-4. 始点と終点 
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... 3D 用
	bool IsIntersectLineTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& s, const Vector3& e);


	//----------------------------------------------------------------------------------------------
	// work... レイとAABB の衝突判定
	// arg1... レイ座標
	// arg2... レイベクトル
	// arg3... 左奥上の座標
	// arg4... 右前下の座標
	// arg7... 演算結果の交点
	// ret.... [衝突している : true] [衝突していない : false]
	bool IsIntersectRayAABB(const Vector3& pos, const Vector3& dir, const Vector3& aabb_max, const Vector3& aabb_min, Vector3* intersect_pos = nullptr);

	//----------------------------------------------------------------------------------------------
	// work... レイとOBB の衝突判定
	// arg1... レイ座標 ( 線分なら始点 )
	// arg2... レイベクトル
	// arg3... ボックスをAABB とした時の 左奥上の座標
	// arg4... ボックスをAABB とした時の 右前下の座標
	// arg5... OBB の回転行列
	// arg7... 演算結果の交点
	// ret.... [衝突している : true] [衝突していない : false]
	bool IsIntersectRayOBB( const Vector3& pos, const Vector3& dir, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3* intersect_pos = nullptr );

	//----------------------------------------------------------------------------------------------
	// work... 線分とOBB の衝突判定
	// arg1... 始点
	// arg2... 終点
	// arg3... OBB が回転していない AABB とした時の max
	// arg4... OBB が回転していない AABB とした時の min
	// arg5... OBB の姿勢
	// arg6... 演算結果の交点
	// ret.... [衝突している : true] [衝突していない : false]
	bool IsIntersectLineOBB(const Vector3& s, const Vector3& e, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3* intersect_pos = nullptr );

	//----------------------------------------------------------------------------------------------
	// work... 線分とOBB の衝突判定 2D用
	// arg1... 始点
	// arg2... 終点
	// arg3... ボックス座標
	// arg4... OBB が回転していない AABB とした時の max
	// arg5... OBB が回転していない AABB とした時の min
	// arg6... OBB の姿勢
	// arg7... 演算結果の交点
	// ret.... [衝突している : true] [衝突していない : false]
	bool IsIntersectLineOBB2D(const Vector3& s, const Vector3& e, const Vector3& pos, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3* intersect_pos = nullptr);

	//----------------------------------------------------------------------------------------------
	// work... レイとトーラスの衝突判定
	// arg1... レイ座標
	// arg2... レイベクトル
	// arg3... トーラス中心座標
	// arg4... トーラス姿勢
	// arg5... トーラスの輪の半径
	// arg6... トーラス全体の半径
	// ret.... [衝突している : true] [衝突していない : false]
	bool IsIntersectRayTorus(const Vector3& s, const Vector3& dir, const Vector3& tp, const Quaternion& tq, const float tube_r, const float swept_r);
	bool IsIntersectRayTorus(const Vector3& s, const Vector3& dir, const Vector3& tp, const Quaternion& tq, const float tube_r, const float swept_r, Vector3* intersect_pos = nullptr );

}
