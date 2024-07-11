#include <memory>
#include "tnl_vector3.h"
#include "tnl_intersect.h"
#include "tnl_math.h"
#include "tnl_matrix.h"

namespace tnl {

	// 点と矩形の衝突判定
	bool IsIntersectPointRect(const int point_x, const int point_y, const int rect_x, const int rect_y, const int rect_size_x, const int rect_size_y) {
		int sx = rect_x - (rect_size_x >> 1);
		int sy = rect_y - (rect_size_y >> 1);
		int ex = sx + rect_size_x;
		int ey = sy + rect_size_y;
		if (point_x < sx) return false;
		if (point_x > ex) return false;
		if (point_y < sy) return false;
		if (point_y > ey) return false;
		return true;
	}

	// 矩形と矩形
	bool IsIntersectRectPrimitive(const int a_left, const int a_right, const int a_top, const int a_bottom,
		const int b_left, const int b_right, const int b_top, const int b_bottom) {
		if (a_right < b_left) return false;
		if (a_left > b_right) return false;
		if (a_top > b_bottom) return false;
		if (a_bottom < b_top) return false;
		return true;
	}

	// 矩形と矩形
	bool IsIntersectRect(const int a_rect_x, const int a_rect_y, const int a_rect_size_w, const int a_rect_size_h,
		const int b_rect_x, const int b_rect_y, const int b_rect_size_w, const int b_rect_size_h) {

		int a_right = a_rect_x + (a_rect_size_w >> 1);
		int a_left = a_rect_x - (a_rect_size_w >> 1);
		int a_top = a_rect_y - (a_rect_size_h >> 1);
		int a_bottom = a_rect_y + (a_rect_size_h >> 1);

		int b_right = b_rect_x + (b_rect_size_w >> 1);
		int b_left = b_rect_x - (b_rect_size_w >> 1);
		int b_top = b_rect_y - (b_rect_size_h >> 1);
		int b_bottom = b_rect_y + (b_rect_size_h >> 1);

		return IsIntersectRectPrimitive(a_left, a_right, a_top, a_bottom, b_left, b_right, b_top, b_bottom);
	}

	// work... 矩形と円の衝突判定
	bool IsIntersectRectCircle(const tnl::Vector3& rect_center, const tnl::Vector2i& rect_size, const tnl::Vector3& circle_pos, float circle_radius) {
		tnl::Vector3 np = tnl::GetNearestRectPoint(rect_center, (float)rect_size.x, (float)rect_size.y, circle_pos);
		if ((np - circle_pos).length() < circle_radius) {
			return true;
		}
		return false;
	}

	// work... 線分と円の衝突判定
	bool IsIntersectLineCircle(const tnl::Vector3& line_s, const tnl::Vector3& line_e, const tnl::Vector3& circle_pos, float radius) {
		tnl::Vector3 nearest = tnl::GetNearestPointLine(circle_pos, line_s, line_e);
		float dist = (circle_pos - nearest).length();
		return dist < radius;
	}

	// work... AABB と AABBの判定
	bool IsIntersectAABB(const tnl::Vector3& a, const tnl::Vector3& a_size, const tnl::Vector3& b, const tnl::Vector3& b_size)
	{
		tnl::Vector3 a_max = tnl::ToMaxAABB(a, a_size);
		tnl::Vector3 a_min = tnl::ToMinAABB(a, a_size);
		tnl::Vector3 b_max = tnl::ToMaxAABB(b, b_size);
		tnl::Vector3 b_min = tnl::ToMinAABB(b, b_size);
		if (a_max.x < b_min.x || a_min.x > b_max.x) return false;
		if (a_max.y < b_min.y || a_min.y > b_max.y) return false;
		if (a_max.z < b_min.z || a_min.z > b_max.z) return false;
		return true;
	}


	//----------------------------------------------------------------------------------------------
	bool IsIntersectSphere(const tnl::Vector3& a, float a_radius, const tnl::Vector3& b, float b_radius) {
		float l = (a - b).length();
		return l < (a_radius + b_radius) ? true : false;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectLinePlane(const tnl::Vector3 &line_start, const tnl::Vector3 &line_end, const tnl::Vector3 &plane_pos, const tnl::Vector3 &plane_normal, tnl::Vector3 *intersect_position) {

		if (1 == GetSidesPointAndPlane(line_start, plane_normal, plane_pos) && 1 == GetSidesPointAndPlane(line_end, plane_normal, plane_pos)) return false;
		if (-1 == GetSidesPointAndPlane(line_start, plane_normal, plane_pos) && -1 == GetSidesPointAndPlane(line_end, plane_normal, plane_pos)) return false;

		tnl::Vector3 ln = line_end - line_start;
		tnl::Vector3 pa = line_start - plane_pos;
		tnl::Vector3 pb = line_end - plane_pos;
		float na = fabsf(pa.dot(plane_normal));
		float nb = fabsf(pb.dot(plane_normal));

		if(intersect_position) *intersect_position = line_start + ln * (na / (na + nb));

		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectLine2D(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4)
	{

		// x 座標によるチェック
		if (p1.x >= p2.x) {
			if ((p1.x < p3.x && p1.x < p4.x) || (p2.x > p3.x && p2.x > p4.x)) {
				return false;
			}
		}
		else {
			if ((p2.x < p3.x && p2.x < p4.x) || (p1.x > p3.x && p1.x > p4.x)) {
				return false;
			}
		}

		// y 座標によるチェック
		if (p1.y >= p2.y) {
			if ((p1.y < p3.y && p1.y < p4.y) || (p2.y > p3.y && p2.y > p4.y)) {
				return false;
			}
		}
		else {
			if ((p2.y < p3.y && p2.y < p4.y) || (p1.y > p3.y && p1.y > p4.y)) {
				return false;
			}
		}

		if (((p1.x - p2.x) * (p3.y - p1.y) + (p1.y - p2.y) * (p1.x - p3.x)) * ((p1.x - p2.x) * (p4.y - p1.y) + (p1.y - p2.y) * (p1.x - p4.x)) > 0) return false;
		if (((p3.x - p4.x) * (p1.y - p3.y) + (p3.y - p4.y) * (p3.x - p1.x)) * ((p3.x - p4.x) * (p2.y - p3.y) + (p3.y - p4.y) * (p3.x - p2.x)) > 0) return false;

		return true;
	}


	//----------------------------------------------------------------------------------------------
	bool IsIntersectLineTriangle2D(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& s, const Vector3& e)
	{
		if (IsIntersectLine2D(v1, v2, s, e)) return true;
		if (IsIntersectLine2D(v2, v3, s, e)) return true;
		if (IsIntersectLine2D(v3, v1, s, e)) return true;
		if (IsIntersectTrianglePoint(v1, v2, v3, s)) return true;
		if (IsIntersectTrianglePoint(v1, v2, v3, e)) return true;
		return false;
	}


	//----------------------------------------------------------------------------------------------
	bool IsIntersectTrianglePoint(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& p)
	{
		Vector3 a = v1 - p;
		Vector3 b = v2 - p;
		Vector3 c = v3 - p;

		float ab = Vector3::Dot(a, b);
		float ac = Vector3::Dot(a, c);
		float bc = Vector3::Dot(b, c);
		float cc = Vector3::Dot(c, c);

		if (0.0f > ((bc * ac) - (cc * ab))) return false;
		float bb = Vector3::Dot(b, b);
		if (0.0f > ((ab * bc) - (ac * bb))) return false;

		return true;
	}


	//----------------------------------------------------------------------------------------------
	bool IsIntersectLineTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& s, const Vector3& e)
	{

		Vector3 ab = v2 - v1;
		Vector3 ac = v3 - v1;
		Vector3 pq = s - e;

		Vector3 n = Vector3::Cross(ab, ac);

		float d = Vector3::Dot(pq, n);
		if (d <= 0.0f) return false;

		Vector3 ap = s - v1;
		float t = Vector3::Dot(ap, n);
		if (t < 0.0f) return false;
		if (t > d) return false;

		Vector3 e_ = Vector3::Cross(pq, ap);
		float v = Vector3::Dot(ac, e_);
		if (v < 0.0f || v > d) return false;
		float w = -Vector3::Dot(ab, e_);
		if (w < 0.0f || v + w > d) return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectRayAABB(const Vector3& pos, const Vector3& dir, const Vector3& aabb_max, const Vector3& aabb_min, Vector3* intersect_pos) {

		float tmin = 0.0f;
		float tmax = FLT_MAX;
		float p[3] = { pos.x, pos.y, pos.z };
		float d[3] = { dir.x, dir.y, dir.z };
		float max[3] = { aabb_max.x, aabb_max.y, aabb_max.z };
		float min[3] = { aabb_min.x, aabb_min.y, aabb_min.z };
		for (int i = 0; i < 3; ++i) {
			if (fabs(d[i]) < FLT_EPSILON) {
				if (p[i] < min[i] || p[i] > max[i]) return false;
			}
			else {
				float ood = 1.0f / d[i];
				float t1 = (min[i] - p[i]) * ood;
				float t2 = (max[i] - p[i]) * ood;
				if (t1 > t2) std::swap(t1, t2);
				if (t1 > tmin) tmin = t1;
				if (t2 < tmax) tmax = t2;
				if (tmin > tmax) return false;
			}
		}
		if (intersect_pos) {
			*intersect_pos = tnl::Vector3(p[0], p[1], p[2]) + (tnl::Vector3(d[0], d[1], d[2]) * tmin);
		}
		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectRayOBB(const Vector3& pos, const Vector3& dir, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3* intersect_pos ) {

		// 光線を境界ボックスの空間へ移動
		Matrix invMat = Matrix::Inverse(obb_rot);

		Vector3 p_l, dir_l;
		p_l = Vector3::TransformCoord(pos, invMat);
		invMat._41 = 0.0f;
		invMat._42 = 0.0f;
		invMat._43 = 0.0f;
		dir_l = Vector3::TransformCoord(dir, invMat);

		// 交差判定
		float p[3], d[3], min[3], max[3];
		memcpy(p, &p_l, sizeof(Vector3));
		memcpy(d, &dir_l, sizeof(Vector3));
		memcpy(min, &aabb_min, sizeof(Vector3));
		memcpy(max, &aabb_max, sizeof(Vector3));

		float t = -FLT_MAX;
		float t_max = FLT_MAX;

		for (int i = 0; i < 3; ++i) {
			if (fabs(d[i]) < FLT_EPSILON) {
				if (p[i] < min[i] || p[i] > max[i])
					return false; // 交差していない
			}
			else {
				// スラブとの距離を算出
				// t1が近スラブ、t2が遠スラブとの距離
				float odd = 1.0f / d[i];
				float t1 = (min[i] - p[i]) * odd;
				float t2 = (max[i] - p[i]) * odd;
				if (t1 > t2) {
					float tmp = t1; t1 = t2; t2 = tmp;
				}

				if (t1 > t) t = t1;
				if (t2 < t_max) t_max = t2;

				// スラブ交差チェック
				if (t >= t_max)
					return false;
			}
		}

		// 交点
		if (intersect_pos) {
			*intersect_pos = pos + (dir * t);
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectLineOBB(const Vector3& s, const Vector3& e, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3* intersect_pos) {
		tnl::Vector3 intersect;
		if (!IsIntersectRayOBB(s, Vector3::Normalize(e - s), aabb_max, aabb_min, obb_rot, &intersect)) {
			return false;
		}
		if ((intersect - s).length() > (e - s).length()) return false;
		if (intersect_pos) *intersect_pos = intersect;
		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectLineOBB2D(const Vector3& s, const Vector3& e, const Vector3& pos, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3* intersect_pos){
		tnl::Vector3 max = aabb_max;
		tnl::Vector3 min = aabb_min;
		tnl::Vector3 ss = pos - s;
		tnl::Vector3 ee = pos - e;
		max.z = 1.0f;
		min.z = -1.0f;
		return tnl::IsIntersectLineOBB(ss, ee, max, min, obb_rot, intersect_pos) ;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectRayTorus(const Vector3& s, const Vector3& dir, const Vector3& tp, const Quaternion& tq, const float tube_r, const float swept_r) {

		tnl::Vector3 o = tnl::Vector3::TransformCoord(s-tp, tq);
		tnl::Vector3 d = tnl::Vector3::TransformCoord(dir, tq);

		double ox = o.x;
		double oy = o.y;
		double oz = o.z;

		double dx = d.x;
		double dy = d.y;
		double dz = d.z;

		// define the coefficients of the quartic equation
		double sum_d_sqrd = dx * dx + dy * dy + dz * dz;
		double e = ox * ox + oy * oy + oz * oz -
			swept_r * swept_r - tube_r * tube_r;
		double f = ox * dx + oy * dy + oz * dz;
		double four_a_sqrd = 4.0 * swept_r * swept_r;

		std::vector<double> coeffs = {
			e * e - four_a_sqrd * (tube_r * tube_r - oy * oy),
				4.0 * f * e + 2.0 * four_a_sqrd * oy * dy,
				2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * dy * dy,
				4.0 * sum_d_sqrd * f,
				sum_d_sqrd * sum_d_sqrd };

		std::vector<double> solution = Solve4(coeffs);

		// ray misses the torus
		if (solution.empty()) return false;
		return true;
	}

	//----------------------------------------------------------------------------------------------
	bool IsIntersectRayTorus(const Vector3& s, const Vector3& dir, const Vector3& tp, const Quaternion& tq, const float tube_r, const float swept_r, Vector3* intersect_pos) {

		tnl::Vector3 o = tnl::Vector3::TransformCoord(s - tp, tq);
		tnl::Vector3 d = tnl::Vector3::TransformCoord(dir, tq);

		double ox = o.x;
		double oy = o.y;
		double oz = o.z;

		double dx = d.x;
		double dy = d.y;
		double dz = d.z;

		// define the coefficients of the quartic equation
		double sum_d_sqrd = dx * dx + dy * dy + dz * dz;
		double e = ox * ox + oy * oy + oz * oz -
			swept_r * swept_r - tube_r * tube_r;
		double f = ox * dx + oy * dy + oz * dz;
		double four_a_sqrd = 4.0 * swept_r * swept_r;

		std::vector<double> coeffs = {
			e * e - four_a_sqrd * (tube_r * tube_r - oy * oy),
				4.0 * f * e + 2.0 * four_a_sqrd * oy * dy,
				2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * dy * dy,
				4.0 * sum_d_sqrd * f,
				sum_d_sqrd * sum_d_sqrd };

		std::vector<double> solution = Solve4(coeffs);

		// ray misses the torus
		if (solution.empty()) return false;

		double mint = DBL_MAX;
		for (auto t : solution) {
			if ((t > DBL_EPSILON ) && (t < mint)) {
				mint = t;
			}
		}

		*intersect_pos = ( s + dir * (float)mint );
		return true;
	}


}