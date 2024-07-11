#include <random>
#include <numbers>
#include "tnl_math.h"
#include "tnl_quaternion.h"
#include "tnl_intersect.h"

namespace tnl {

    //--------------------------------------------------------------------------------------------------------------------------------
    std::vector<double> Solve2(std::vector<double>& c) {
        double p = c[1] / (2 * c[2]);
        double q = c[0] / c[2];
        double D = p * p - q;
        if (IsZeroD(D)) {
            return {-p};
        }else if (D < 0) {
            return {};
        }else /* if (D > 0) */ {
            double sqrt_D = sqrt(D);
            return { sqrt_D - p, -sqrt_D - p };
        }
    }
    //--------------------------------------------------------------------------------------------------------------------------------
    std::vector<double> Solve3(std::vector<double>& c) {
        /* normal form: x^3 + Ax^2 + Bx + C = 0 */
        double A = c[2] / c[3];
        double B = c[1] / c[3];
        double C = c[0] / c[3];

        /*  substitute x = y - A/3 to eliminate quadric term:
        x^3 +px + q = 0 */
        double sq_A = A * A;
        double p = 1.0 / 3 * (-1.0 / 3 * sq_A + B);
        double q = 1.0 / 2 * (2.0 / 27 * A * sq_A - 1.0 / 3 * A * B + C);

        /* use Cardano's formula */

        double cb_p = p * p * p;
        double D = q * q + cb_p;

        std::vector<double> s ;

        if (IsZeroD(D)) {
            if (IsZeroD(q)) /* one triple solution */ {
                s = {0};
            }else /* one single and one double solution */ {
                double u = cbrt(-q);
                s = { 2 * u, -u };
            }
        }else if (D < 0) /* Casus irreducibilis: three real solutions */ {
            double phi = 1.0 / 3 * acos(-q / sqrt(-cb_p));
            double t = 2 * sqrt(-p);
            s = { t * cos(phi),
                -t * cos(phi + std::numbers::pi / 3),
                -t * cos(phi - std::numbers::pi / 3) };

        }
        else /* one real solution */ {
            double sqrt_D = sqrt(D);
            double u = cbrt(sqrt_D - q);
            double v = -cbrt(sqrt_D + q);
            s = { u + v };
        }

        /* resubstitute */
        double sub = 1.0 / 3 * A;
        for (uint32_t i = 0; i < s.size(); ++i)
            s[i] -= sub;
        return s;
    }
    //--------------------------------------------------------------------------------------------------------------------------------
    std::vector<double> Solve4(std::vector<double>& c) {
        /* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */
        double A = c[3] / c[4];
        double B = c[2] / c[4];
        double C = c[1] / c[4];
        double D = c[0] / c[4];

        /*  substitute x = y - A/4 to eliminate cubic term:
        x^4 + px^2 + qx + r = 0 */
        double sq_A = A * A;
        double p = -3.0 / 8 * sq_A + B;
        double q = 1.0 / 8 * sq_A * A - 1.0 / 2 * A * B + C;
        double r = -3.0 / 256 * sq_A * sq_A + 1.0 / 16 * sq_A * B - 1.0 / 4 * A * C + D;
        std::vector<double> s ;

        if (IsZeroD(r)) {
            /* no absolute term: y(y^3 + py + q) = 0 */
            std::vector<double> coeffs = { q, p, 0, 1 };
            s = Solve3(coeffs);
            s.emplace_back(0);
        }
        else {
            /* solve the resolvent cubic ... */
            std::vector<double> coeffs = {
                1.0 / 2 * r * p - 1.0 / 8 * q * q,
                    -r,
                    -1.0 / 2 * p,
                    1 };
            s = Solve3(coeffs);

            /* ... and take the one real solution ... */
            double z = s[0];

            /* ... to build two quadric equations */
            double u = z * z - r;
            double v = 2 * z - p;

            if (IsZeroD(u))
                u = 0;
            else if (u > 0)
                u = sqrt(u);
            else
                return {};

            if (IsZeroD(v))
                v = 0;
            else if (v > 0)
                v = sqrt(v);
            else
                return {};

            coeffs = {
                z - u,
                    q < 0 ? -v : v,
                    1 };

            s = Solve2(coeffs);

            coeffs = { z + u,
                q < 0 ? v : -v,
                1 };
            auto t = Solve2(coeffs);
            s.insert(s.end(), t.begin(), t.end());
        }

        /* resubstitute */
        double sub = 1.0 / 4 * A;
        for (uint32_t i = 0; i < s.size(); ++i)
            s[i] -= sub;

        return s;
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    std::mt19937 g_mersenne ;
    void SetSeedMersenneTwister32(int seed) {
        g_mersenne.seed(seed);
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    float GetRandomDistributionFloat(float min, float max) {
        std::uniform_real_distribution<> param(static_cast<double>(min), static_cast<double>(max));
        return static_cast<float>(param(g_mersenne));
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    int GetSidesPointAndPlane(const Vector3& v, const Vector3& pn, const Vector3& pv)
	{
		Vector3 vec = v - pv;
		float a = vec.dot( pn );
		if (a > 0.0f) 		return 1;		// ï\
		else if (a < 0.0f)	return -1;		// ó†
		else 				return 0;		// ïΩñ è„
	}

    //--------------------------------------------------------------------------------------------------------------------------------
    eRegionPtRect GetRegionPointAndRect(const Vector3& p, const Vector3& rp, const int rect_w, const int rect_h) {
		tnl::Vector3 v1 = tnl::Vector3::Normalize({ float(rect_h), float(rect_w), 0 });
		tnl::Vector3 v2 = tnl::Vector3::Normalize({ float(rect_h), float(-rect_w), 0 });
		tnl::Vector3 vc1 = v1;
		tnl::Vector3 vc2 = v2;

		int s1 = tnl::GetSidesPointAndPlane(p, vc1, rp);
		int s2 = tnl::GetSidesPointAndPlane(p, vc2, rp);
		if (s1 >= 0 && s2 >= 0) {
			return eRegionPtRect::RIGHT;
		}
		else if (s1 >= 0 && s2 <= 0) {
			return eRegionPtRect::DOWN;
		}
		else if (s1 <= 0 && s2 >= 0) {
			return eRegionPtRect::UP;
		}
		else {
			return eRegionPtRect::LEFT;
		}
	}

    //--------------------------------------------------------------------------------------------------------------------------------
    eRegionPtAABB GetRegionPointAndAABB(const Vector3& p, const Vector3& max, const Vector3& min) {
        tnl::Vector3 c = (max + min) * 0.5f;
        tnl::Vector3 pp = p - c ;
        tnl::Vector3 mmax = max - c;
        tnl::Vector3 mmin = min - c;

        int side[4] = { 0, 0, 0, 0 };
        tnl::Vector3 t_max[4];
        tnl::Vector3 t_min[4];
        tnl::Vector3 vn[4];
        t_max[0] = { mmax.x, 0, mmax.z };
        t_min[0] = { mmin.x, 0, mmin.z };
        t_max[1] = { mmax.x, 0, -mmax.z };
        t_min[1] = { mmin.x, 0, -mmin.z };
        t_max[2] = { 0, mmax.y, mmax.z };
        t_min[2] = { 0, mmin.y, mmin.z };
        t_max[3] = { 0, -mmax.y, mmax.z };
        t_min[3] = { 0, -mmin.y, mmin.z };
        vn[0] = tnl::Vector3::Normalize(tnl::Vector3::Cross((t_min[0] - t_max[0]), { 0, -1, 0 }));
        vn[1] = tnl::Vector3::Normalize(tnl::Vector3::Cross((t_min[1] - t_max[1]), { 0, -1, 0 }));
        vn[2] = tnl::Vector3::Normalize(tnl::Vector3::Cross((t_min[2] - t_max[2]), { -1, 0, 0 }));
        vn[3] = tnl::Vector3::Normalize(tnl::Vector3::Cross((t_min[3] - t_max[3]), { -1, 0, 0 }));

        for (int i = 0; i < 4; ++i) side[i] = GetSidesPointAndPlane(pp, vn[i], t_max[i]);

        // ëOÇÃóÃàÊÇ…ë∂ç›
        if (-1 == side[0] && -1 == side[1] && 1 == side[2] && -1 == side[3]) return eRegionPtAABB::FORWORD;

        // å„ÇÃóÃàÊÇ…ë∂ç›
        if (1 == side[0] && 1 == side[1] && -1 == side[2] && 1 == side[3]) return eRegionPtAABB::BACK;

        eRegionPtAABB region[4] = {
            eRegionPtAABB::DOWN, eRegionPtAABB::RIGHT, eRegionPtAABB::UP, eRegionPtAABB::LEFT
        };
        eRegionPtRect rect_region = GetRegionPointAndRect(pp, (mmax + mmin) * 0.5f, (int)(mmax.x - mmin.x), (int)(mmax.y - mmin.y));
        return region[static_cast<int>(rect_region)];
    }



    //--------------------------------------------------------------------------------------------------------------------------------
    eRegionPtXzOBB GetXzRegionPointAndOBB(const Vector3& p, const Vector3& op, const Vector3& size, const Quaternion& q) {
        tnl::Vector3 pv1 = tnl::Vector3::Normalize(size) * 0.5f ;
        tnl::Vector3 pv2 = {-pv1.x, pv1.y, pv1.z};
        tnl::Vector3 pn1 = tnl::Vector3::Normalize( tnl::Vector3::Cross(pv1, pv1.xz()) );
        tnl::Vector3 pn2 = tnl::Vector3::Normalize( tnl::Vector3::Cross(pv2, pv2.xz()) );
        pv1 = op + tnl::Vector3::TransformCoord(pv1, q);
        pv2 = op + tnl::Vector3::TransformCoord(pv2, q);
        pn1 = tnl::Vector3::TransformCoord(pn1, q);
        pn2 = tnl::Vector3::TransformCoord(pn2, q);

        int s1 = tnl::GetSidesPointAndPlane(p, pn1, pv1);
        int s2 = tnl::GetSidesPointAndPlane(p, pn2, pv2);
        if (s1 >= 0 && s2 >= 0) {
            return eRegionPtXzOBB::RIGHT;
        }
        else if (s1 >= 0 && s2 <= 0) {
            return eRegionPtXzOBB::BACK;
        }
        else if (s1 <= 0 && s2 >= 0) {
            return eRegionPtXzOBB::FORWORD;
        }
        else {
            return eRegionPtXzOBB::LEFT;
        }
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    tnl::Vector3 GetNearestPointAABB(const tnl::Vector3& point, const tnl::Vector3& aabb_max, const tnl::Vector3& aabb_min) {
        tnl::Vector3 q;
        float v = point.x;
        if (v < aabb_min.x) v = aabb_min.x;
        if (v > aabb_max.x) v = aabb_max.x;
        q.x = v;

        v = point.y;
        if (v < aabb_min.y) v = aabb_min.y;
        if (v > aabb_max.y) v = aabb_max.y;
        q.y = v;

        v = point.z;
        if (v < aabb_min.z) v = aabb_min.z;
        if (v > aabb_max.z) v = aabb_max.z;
        q.z = v;

        return q;
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, float w, float h, const tnl::Vector3 &point) {
		tnl::Vector3 nearest(0, 0, 0);
		float hw = w * 0.5f;
		float hh = h * 0.5f;
		float l = rect_pos.x - hw;
		float r = rect_pos.x + hw;
		float t = rect_pos.y - hh;
		float b = rect_pos.y + hh;
		nearest.x = (point.x > l && point.x < r) ? point.x : (point.x > l) ? r : l;
		nearest.y = (point.y > t && point.y < b) ? point.y : (point.y > t) ? b : t;
		return nearest;
	}


    //--------------------------------------------------------------------------------------------------------------------------------
    tnl::Vector3 GetNearestRectPoint(const tnl::Vector3& rect_pos, const tnl::Vector3& axis_x, const tnl::Vector3& axis_y, const tnl::Vector3& rect_side_length, const tnl::Vector3& point) {
        tnl::Vector3 d = point - rect_pos;
        tnl::Vector3 q = rect_pos;
        float dist = tnl::Vector3::Dot(d, axis_x);
        if (dist > rect_side_length.x) dist = rect_side_length.x;
        if (dist < -rect_side_length.x) dist = -rect_side_length.x;
        q += axis_x * dist;

        dist = tnl::Vector3::Dot(d, axis_y);
        if (dist > rect_side_length.y) dist = rect_side_length.y;
        if (dist < -rect_side_length.y) dist = -rect_side_length.y;
        q += axis_y * dist;
        return q;
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    tnl::Vector3 GetNearestPointTriangle(const tnl::Vector3& p, const tnl::Vector3& a, const tnl::Vector3& b, const tnl::Vector3& c) {
        tnl::Vector3 ab = b - a;
        tnl::Vector3 ac = c - a;
        tnl::Vector3 ap = p - a;
        float d1 = tnl::Vector3::Dot(ab, ap);
        float d2 = tnl::Vector3::Dot(ac, ap);
        if (d1 <= 0.0f && d2 <= 0.0f) return a;

        tnl::Vector3 bp = p - b;
        float d3 = tnl::Vector3::Dot(ab, bp);
        float d4 = tnl::Vector3::Dot(ac, bp);
        if (d3 >= 0.0f && d4 <= d3) return b;

        float vc = d1 * d4 - d3 * d2;
        if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
            float v = d1 / (d1 - d3);
            return a + ( ab * v );
        }

        tnl::Vector3 cp = p - c;
        float d5 = tnl::Vector3::Dot(ab, cp);
        float d6 = tnl::Vector3::Dot(ac, cp);
        if (d6 >= 0.0f && d5 <= d6) return c;
        
        float vb = d5 * d2 - d1 * d6;
        if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
            float w = d2 / (d2 - d6);
            return a + (ac * w);
        }

        float va = d3 * d6 - d5 * d4;
        if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
            float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
            return b + ((c - b) * w);
        }

        float denom = 1.0f / (va + vb + vc);
        float v = vb * denom;
        float w = vc * denom;
        return a + (ab * v) + (ac * w);
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    tnl::Vector3 GetNearestPointPlane(const tnl::Vector3& v, const tnl::Vector3& pn, const tnl::Vector3& pv) {
        float d = tnl::Vector3::Dot(pn, pv);
        float t = tnl::Vector3::Dot(pn, v) - d;
        return v - (pn * t);
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    tnl::Vector3 GetNearestPointLine(const tnl::Vector3& v, const tnl::Vector3& a, const tnl::Vector3& b) {
        tnl::Vector3 ab = b - a;
        float t = tnl::Vector3::Dot( v-a, ab );
        if (t <= 0.0f) {
            return a;
        }
        else {
            float denom = tnl::Vector3::Dot(ab, ab);
            if (t >= denom) {
                return b;
            }
            else {
                t /= denom;
                return a + (ab * t);
            }
        }
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    std::tuple<tnl::Vector3, tnl::Vector3> GetNearestLines(const tnl::Vector3& s1, const tnl::Vector3& e1, const tnl::Vector3& s2, const tnl::Vector3& e2) {

        float s, t ;
        tnl::Vector3 c1, c2;

        tnl::Vector3 d1 = e1 - s1;
        tnl::Vector3 d2 = e2 - s2;
        tnl::Vector3 r = s1 - s2;
        float a = tnl::Vector3::Dot(d1, d1);
        float e = tnl::Vector3::Dot(d2, d2);
        float f = tnl::Vector3::Dot(d2, r);

        if (a <= FLT_EPSILON && e <= FLT_EPSILON) {
            s = t = 0.0f;
            c1 = s1;
            c2 = s2;
            return std::make_tuple(c1, c2);
        }
        if (a <= FLT_EPSILON) {
            s = 0.0f;
            t = f / e;
            t = std::clamp(t, 0.0f, 1.0f);
        }
        else {
            float c = tnl::Vector3::Dot(d1, r);
            if (e <= FLT_EPSILON) {
                t = 0.0f;
                s = std::clamp(-c/a, 0.0f, 1.0f);
            }
            else {
                float b = tnl::Vector3::Dot(d1, d2);
                float denom = a * e - b * b;
                if (denom != 0.0f) {
                    s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
                }
                else s = 0.0f;

                t = (b * s + f) / e;

                if (t < 0.0f) {
                    t = 0.0f;
                    s = std::clamp(-c/a, 0.0f, 1.0f);
                }
                else if(t > 1.0f) {
                    t = 1.0f;
                    s = std::clamp((b-c)/a, 0.0f, 1.0f);
                }
            }
        }
        c1 = s1 + d1 * s;
        c2 = s2 + d2 * t;
        return std::make_tuple( c1, c2 );

    }


    //--------------------------------------------------------------------------------------------------------------------------------
    tnl::Vector3 GetRandomPointInsidePlane(const tnl::Vector3& plane_position, const tnl::Vector3& plane_normal, float radius) {

        if (plane_normal.length() <= FLT_EPSILON) return plane_position;

        tnl::Vector3 cv = tnl::Vector3::Cross(plane_normal, tnl::Vector3::left);
        if (cv.length() <= FLT_EPSILON) cv = tnl::Vector3::Cross(plane_normal, tnl::Vector3::up);
        if (cv.length() <= FLT_EPSILON) cv = tnl::Vector3::Cross(plane_normal, tnl::Vector3::forward);

        tnl::Quaternion rot = tnl::Quaternion::RotationAxis(cv, tnl::ToRadian(90));
        tnl::Vector3 random_v = tnl::Vector3::TransformCoord(plane_normal, rot);

        float angle = tnl::GetRandomDistributionFloat(-180, 180);
        rot = tnl::Quaternion::RotationAxis(plane_normal, tnl::ToRadian(angle));
        random_v = tnl::Vector3::TransformCoord(random_v, rot);

        return plane_position + random_v * tnl::GetRandomDistributionFloat(0, radius);

    }

    //--------------------------------------------------------------------------------------------------------------------------------
    std::tuple<tnl::Vector3, float> CircumscribedCircle(const tnl::Vector3& a, const tnl::Vector3& b, const tnl::Vector3& c){
        tnl::Vector3 a2 = a * a;
        tnl::Vector3 b2 = b * b;
        tnl::Vector3 c2 = c * c;

        float CT = 2 * ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
        float x = ((c.y - a.y) * (b2.x - a2.x + b2.y - a2.y) + (a.y - b.y) * (c2.x - a2.x + c2.y - a2.y)) / CT;
        float y = ((a.x - c.x) * (b2.x - a2.x + b2.y - a2.y) + (b.x - a.x) * (c2.x - a2.x + c2.y - a2.y)) / CT;

        float r = (tnl::Vector3(x, y, 0) - a).length();

        return std::make_tuple(tnl::Vector3(x, y, 0), r);
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    Vector3 BezierSpline(const Vector3& _a1, const Vector3& _a2, const Vector3& _a3, const Vector3& _a4, float t) {

        t = (1.0f < t) ? 1.0f : t;
        t = (0.0f > t) ? 0.0f : t;

        float wkT = (1.0f - t);
        float wkX, wkY, wkZ;
        wkX = wkY = wkZ = 0.0f;

        float T2 = (t * t);
        float T3 = (t * t * t);
        float wkT2 = (wkT * wkT);
        float wkT3 = (wkT * wkT * wkT);

        wkX = ((wkT3)*_a1.x);
        wkX = wkX + ((3 * (wkT2)) * (t * _a2.x));
        wkX = wkX + (((3 * wkT) * (T2)) * (_a3.x));
        wkX = wkX + ((T3)*_a4.x);

        wkY = ((wkT3)*_a1.y);
        wkY = wkY + ((3 * (wkT2)) * (t * _a2.y));
        wkY = wkY + (((3 * wkT) * (T2)) * (_a3.y));
        wkY = wkY + ((T3)*_a4.y);

        wkZ = ((wkT3)*_a1.z);

        wkZ = wkZ + ((3 * (wkT2)) * (t * _a2.z));
        wkZ = wkZ + (((3 * wkT) * (T2)) * (_a3.z));
        wkZ = wkZ + ((T3)*_a4.z);

        return Vector3( wkX, wkY, wkZ );
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    eCorrResRect CorrectPositionRect(
        const tnl::Vector3& a_before
        , const tnl::Vector3& b_before
        , const tnl::Vector2i& a_size
        , const tnl::Vector2i& b_size
        , tnl::Vector3& a_out
        , tnl::Vector3& b_out
        , eCorrTypeRect correct_type_x
        , eCorrTypeRect correct_type_y
        , float correct_space ) {

        // à⁄ìÆó ÇÃëÂÇ´Ç¢ï˚Çï‚ê≥ÇÃäÓèÄÇ∆Ç∑ÇÈ
        float l1 = (a_before - a_out).length();
        float l2 = (b_before - b_out).length();

        if ((l1 < l2) && eCorrTypeRect::PWRFL_A == correct_type_x) correct_type_x = eCorrTypeRect::PWRFL_B;
        else if ((l1 < l2) && eCorrTypeRect::PWRFL_B == correct_type_x) correct_type_x = eCorrTypeRect::PWRFL_A;

        if ((l1 < l2) && eCorrTypeRect::PWRFL_A == correct_type_y) correct_type_y = eCorrTypeRect::PWRFL_B;
        else if ((l1 < l2) && eCorrTypeRect::PWRFL_B == correct_type_y) correct_type_y = eCorrTypeRect::PWRFL_A;

        tnl::Vector2i m_size = (l1 >= l2) ? a_size : b_size;
        tnl::Vector2i u_size = (l1 >= l2) ? b_size : a_size;
        tnl::Vector3 m_out = (l1 >= l2) ? a_out : b_out;
        tnl::Vector3 u_out = (l1 >= l2) ? b_out : a_out;
        tnl::Vector3 m_before = (l1 >= l2) ? a_before : b_before;
        tnl::Vector3 u_beore = (l1 >= l2) ? b_before : a_before;

        // äÓèÄÇ∆Ç»Ç¡ÇΩãÈå`ÇÃÇ‡Ç§àÍï˚Ç÷ÇÃç≈ãﬂì_
        tnl::Vector3 m_near = tnl::GetNearestRectPoint(m_before, (float)m_size.x, (float)m_size.y, u_out);

        // ç≈ãﬂì_Ç™Ç‡Ç§ï–ï˚ÇÃà⁄ìÆëOÇÃãÈå`Ç©ÇÁëŒäpê¸Çà¯Ç¢ÇΩéûè„â∫ç∂âEÇ«ÇÃóÃàÊÇ…ë∂ç›Ç∑ÇÈÇ©
        tnl::eRegionPtRect region = tnl::GetRegionPointAndRect(m_near, u_beore, u_size.x, u_size.y);

        float aw2 = static_cast<float>(m_size.x >> 1);
        float ah2 = static_cast<float>(m_size.y >> 1);
        float bw2 = static_cast<float>(u_size.x >> 1);
        float bh2 = static_cast<float>(u_size.y >> 1);
        float aw4 = static_cast<float>(m_size.x >> 2);
        float ah4 = static_cast<float>(m_size.y >> 2);
        float bw4 = static_cast<float>(u_size.x >> 2);
        float bh4 = static_cast<float>(u_size.y >> 2);
        float half_correct_space = correct_space / 2;

        eCorrResRect return_correct_dir;

        std::function<void()> CorrectUp = [&]() {
            if (eCorrTypeRect::PWRFL_A == correct_type_y) u_out.y = m_out.y + bh2 + ah2 + correct_space;
            if (eCorrTypeRect::PWRFL_B == correct_type_y) m_out.y = u_out.y - bh2 - ah2 - correct_space;
            if (eCorrTypeRect::BOTH == correct_type_y) {
                float c = (m_out.y + u_out.y) / 2;
                m_out.y = c - bh4 - ah4 - half_correct_space;
                u_out.y = c + bh4 + ah4 + half_correct_space;
            }
            return_correct_dir = eCorrResRect::UP;
            };

        std::function<void()> CorrectDown = [&]() {
            if (eCorrTypeRect::PWRFL_A == correct_type_y) u_out.y = m_out.y - bh2 - ah2 - correct_space;
            if (eCorrTypeRect::PWRFL_B == correct_type_y) m_out.y = u_out.y + bh2 + ah2 + correct_space;
            if (eCorrTypeRect::BOTH == correct_type_y) {
                float c = (m_out.y + u_out.y) / 2;
                m_out.y = c + bh4 + ah4 + half_correct_space;
                u_out.y = c - bh4 - ah4 - half_correct_space;
            }
            return_correct_dir = eCorrResRect::DOWN;
            };

        std::function<void()> CorrectRight = [&]() {
            if (eCorrTypeRect::PWRFL_A == correct_type_x) u_out.x = m_out.x - bw2 - aw2 - correct_space;
            if (eCorrTypeRect::PWRFL_B == correct_type_x) m_out.x = u_out.x + bw2 + aw2 + correct_space;
            if (eCorrTypeRect::BOTH == correct_type_x) {
                float c = (m_out.x + u_out.x) / 2;
                m_out.x = c + bw4 + aw4 + half_correct_space;
                u_out.x = c - bw4 - aw4 - half_correct_space;
            }
            return_correct_dir = eCorrResRect::RIGHT;
            };

        std::function<void()> CorrectLeft = [&]() {
            if (eCorrTypeRect::PWRFL_A == correct_type_x) u_out.x = m_out.x + bw2 + aw2 + correct_space;
            if (eCorrTypeRect::PWRFL_B == correct_type_x) m_out.x = u_out.x - bw2 - aw2 - correct_space;
            if (eCorrTypeRect::BOTH == correct_type_x) {
                float c = (m_out.x + u_out.x) / 2;
                m_out.x = c - bw4 - aw4 - half_correct_space;
                u_out.x = c + bw4 + aw4 + half_correct_space;
            }
            return_correct_dir = eCorrResRect::LEFT;
            };

        switch (region) {
        case tnl::eRegionPtRect::UP:	CorrectUp(); break;
        case tnl::eRegionPtRect::DOWN:	CorrectDown(); break;
        case tnl::eRegionPtRect::RIGHT:	CorrectRight(); break;
        case tnl::eRegionPtRect::LEFT:	CorrectLeft(); break;
        }

        a_out = (l1 >= l2) ? m_out : u_out;
        b_out = (l1 >= l2) ? u_out : m_out;
        if (l1 < l2) {
            switch (return_correct_dir) {
            case eCorrResRect::LEFT:	return_correct_dir = eCorrResRect::RIGHT; break;
            case eCorrResRect::RIGHT:	return_correct_dir = eCorrResRect::LEFT; break;
            case eCorrResRect::UP:		return_correct_dir = eCorrResRect::DOWN; break;
            case eCorrResRect::DOWN:	return_correct_dir = eCorrResRect::UP; break;
            }
        }

        return return_correct_dir;
    }

    //--------------------------------------------------------------------------------------------------------------------------------
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
        , float correct_space)
    {

        // à⁄ìÆó ÇÃëÂÇ´Ç¢ï˚Çï‚ê≥ÇÃäÓèÄÇ∆Ç∑ÇÈ
        float l1 = (a_before - a_out).length();
        float l2 = (b_before - b_out).length();

        if ((l1 < l2) && eCorrTypeAABB::PWRFL_A == correct_type_x) correct_type_x = eCorrTypeAABB::PWRFL_B;
        else if ((l1 < l2) && eCorrTypeAABB::PWRFL_B == correct_type_x) correct_type_x = eCorrTypeAABB::PWRFL_A;

        if ((l1 < l2) && eCorrTypeAABB::PWRFL_A == correct_type_y) correct_type_y = eCorrTypeAABB::PWRFL_B;
        else if ((l1 < l2) && eCorrTypeAABB::PWRFL_B == correct_type_y) correct_type_y = eCorrTypeAABB::PWRFL_A;

        if ((l1 < l2) && eCorrTypeAABB::PWRFL_A == correct_type_z) correct_type_z = eCorrTypeAABB::PWRFL_B;
        else if ((l1 < l2) && eCorrTypeAABB::PWRFL_B == correct_type_z) correct_type_z = eCorrTypeAABB::PWRFL_A;

        tnl::Vector3 m_size = (l1 >= l2) ? a_size : b_size;
        tnl::Vector3 u_size = (l1 >= l2) ? b_size : a_size;
        tnl::Vector3 m_out = (l1 >= l2) ? a_out : b_out;
        tnl::Vector3 u_out = (l1 >= l2) ? b_out : a_out;
        tnl::Vector3 m_before = (l1 >= l2) ? a_before : b_before;
        tnl::Vector3 u_before = (l1 >= l2) ? b_before : a_before;

        tnl::Vector3 m_max = tnl::ToMaxAABB(m_before, m_size);
        tnl::Vector3 m_min = tnl::ToMinAABB(m_before, m_size);
        tnl::Vector3 u_max = tnl::ToMaxAABB(u_before, u_size);
        tnl::Vector3 u_min = tnl::ToMinAABB(u_before, u_size);

        tnl::Vector3 nearest = tnl::GetNearestPointAABB(u_out, m_max, m_min);
        tnl::eRegionPtAABB region = tnl::GetRegionPointAndAABB(nearest, u_max, u_min);


        float distance = 0;
        tnl::Vector3 correct_base_point;

        if (eCorrTypeAABB::PWRFL_A == correct_type_x) correct_base_point.x = m_out.x;
        if (eCorrTypeAABB::PWRFL_B == correct_type_x) correct_base_point.x = u_out.x;
        if (eCorrTypeAABB::BOTH == correct_type_x) correct_base_point.x = (m_out.x + u_out.x) / 2.0f;

        if (eCorrTypeAABB::PWRFL_A == correct_type_y) correct_base_point.y = m_out.y;
        if (eCorrTypeAABB::PWRFL_B == correct_type_y) correct_base_point.y = u_out.y;
        if (eCorrTypeAABB::BOTH == correct_type_y) correct_base_point.y = (m_out.y + u_out.y) / 2.0f;

        if (eCorrTypeAABB::PWRFL_A == correct_type_z) correct_base_point.z = m_out.z;
        if (eCorrTypeAABB::PWRFL_B == correct_type_z) correct_base_point.z = u_out.z;
        if (eCorrTypeAABB::BOTH == correct_type_z) correct_base_point.z = (m_out.z + u_out.z) / 2.0f;

        eCorrResAABB return_correct_dir;

        switch (region) {
        case tnl::eRegionPtAABB::UP:
            distance = (m_size.y + u_size.y) * 0.5f;
            if (eCorrTypeAABB::PWRFL_A == correct_type_y) u_out.y = correct_base_point.y - distance - correct_space;
            if (eCorrTypeAABB::PWRFL_B == correct_type_y) m_out.y = correct_base_point.y + distance + correct_space;
            if (eCorrTypeAABB::BOTH == correct_type_y) {
                m_out.y = correct_base_point.y + (distance * 0.5f) + (correct_space * 0.5f);
                u_out.y = correct_base_point.y - (distance * 0.5f) - (correct_space * 0.5f);
            }
            return_correct_dir = eCorrResAABB::UP;
            break;
        case tnl::eRegionPtAABB::DOWN:
            distance = (m_size.y + u_size.y) * 0.5f;
            if (eCorrTypeAABB::PWRFL_A == correct_type_y) u_out.y = correct_base_point.y + distance + correct_space;
            if (eCorrTypeAABB::PWRFL_B == correct_type_y) m_out.y = correct_base_point.y - distance - correct_space;
            if (eCorrTypeAABB::BOTH == correct_type_y) {
                m_out.y = correct_base_point.y - (distance * 0.5f) - (correct_space * 0.5f);
                u_out.y = correct_base_point.y + (distance * 0.5f) + (correct_space * 0.5f);
            }
            return_correct_dir = eCorrResAABB::DOWN;
            break;
        case tnl::eRegionPtAABB::RIGHT:
            distance = (m_size.x + u_size.x) * 0.5f;
            if (eCorrTypeAABB::PWRFL_A == correct_type_x) u_out.x = correct_base_point.x - distance - correct_space;
            if (eCorrTypeAABB::PWRFL_B == correct_type_x) m_out.x = correct_base_point.x + distance + correct_space;
            if (eCorrTypeAABB::BOTH == correct_type_x) {
                m_out.x = correct_base_point.x + (distance * 0.5f) + (correct_space * 0.5f);
                u_out.x = correct_base_point.x - (distance * 0.5f) - (correct_space * 0.5f);
            }
            return_correct_dir = eCorrResAABB::RIGHT;
            break;
        case tnl::eRegionPtAABB::LEFT:
            distance = (m_size.x + u_size.x) * 0.5f;
            if (eCorrTypeAABB::PWRFL_A == correct_type_x) u_out.x = correct_base_point.x + distance + correct_space;
            if (eCorrTypeAABB::PWRFL_B == correct_type_x) m_out.x = correct_base_point.x - distance - correct_space;
            if (eCorrTypeAABB::BOTH == correct_type_x) {
                m_out.x = correct_base_point.x - (distance * 0.5f) - (correct_space * 0.5f);
                u_out.x = correct_base_point.x + (distance * 0.5f) + (correct_space * 0.5f);
            }
            return_correct_dir = eCorrResAABB::LEFT;
            break;
        case tnl::eRegionPtAABB::BACK:
            distance = (m_size.z + u_size.z) * 0.5f;
            if (eCorrTypeAABB::PWRFL_A == correct_type_z) u_out.z = correct_base_point.z - distance - correct_space;
            if (eCorrTypeAABB::PWRFL_B == correct_type_z) m_out.z = correct_base_point.z + distance + correct_space;
            if (eCorrTypeAABB::BOTH == correct_type_z) {
                m_out.z = correct_base_point.z + (distance * 0.5f) + (correct_space * 0.5f);
                u_out.z = correct_base_point.z - (distance * 0.5f) - (correct_space * 0.5f);
            }
            return_correct_dir = eCorrResAABB::BACK;
            break;
        case tnl::eRegionPtAABB::FORWORD:
            distance = (m_size.z + u_size.z) * 0.5f;
            if (eCorrTypeAABB::PWRFL_A == correct_type_z) u_out.z = correct_base_point.z + distance + correct_space;
            if (eCorrTypeAABB::PWRFL_B == correct_type_z) m_out.z = correct_base_point.z - distance - correct_space;
            if (eCorrTypeAABB::BOTH == correct_type_z) {
                m_out.z = correct_base_point.z - (distance * 0.5f) - (correct_space * 0.5f);
                u_out.z = correct_base_point.z + (distance * 0.5f) + (correct_space * 0.5f);
            }
            return_correct_dir = eCorrResAABB::FORWORD;
            break;
        }

        a_out = (l1 >= l2) ? m_out : u_out;
        b_out = (l1 >= l2) ? u_out : m_out;
        if (l1 < l2) {
            switch (return_correct_dir) {
            case eCorrResAABB::LEFT:	return_correct_dir = eCorrResAABB::RIGHT; break;
            case eCorrResAABB::RIGHT:	return_correct_dir = eCorrResAABB::LEFT; break;
            case eCorrResAABB::UP:		return_correct_dir = eCorrResAABB::DOWN; break;
            case eCorrResAABB::DOWN:	return_correct_dir = eCorrResAABB::UP; break;
            case eCorrResAABB::FORWORD:	return_correct_dir = eCorrResAABB::BACK; break;
            case eCorrResAABB::BACK:	return_correct_dir = eCorrResAABB::FORWORD; break;
            }
        }

        return return_correct_dir;
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    tnl::Vector3 CorrectPositionRectCircle(
        const tnl::Vector3& rect_center
        , const tnl::Vector2i& rect_size
        , const tnl::Vector3& circle_before
        , const float circle_radius
        , tnl::Vector3& circle_out
        , float correct_space ) {

        tnl::Vector3 rect_np = tnl::GetNearestRectPoint(rect_center, (float)rect_size.x, (float)rect_size.y, circle_before);
        tnl::Vector3 rect_to_cir = tnl::Vector3::Normalize(circle_out - rect_np);
        tnl::Vector3 circle_np = circle_before + tnl::Vector3::Normalize(rect_np - circle_before) * circle_radius;
        circle_out = rect_np + (rect_to_cir * (circle_radius + correct_space));

        return rect_to_cir;
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    tnl::Vector3 CorrectPositionLineCircle(
        const tnl::Vector3& line_s
        , const tnl::Vector3& line_e
        , const tnl::Vector3& circle_before
        , const float circle_radius
        , tnl::Vector3& circle_out
        , float correct_space) {

        tnl::Vector3 line_np = tnl::GetNearestPointLine(circle_before, line_s, line_e);
        tnl::Vector3 line_to_cir = tnl::Vector3::Normalize(circle_out - line_np);
        tnl::Vector3 circle_np = circle_before + tnl::Vector3::Normalize(line_np - circle_before) * circle_radius;
        circle_out = line_np + (line_to_cir * (circle_radius + correct_space));
        return line_to_cir;
    }



    //--------------------------------------------------------------------------------------------------------------------------------
    std::vector<tnl::Vector2i> GetBresenhamsLine(const tnl::Vector2i& st, const tnl::Vector2i& en) {

        std::vector<tnl::Vector2i> lines;
        tnl::Vector2i s = st;
        tnl::Vector2i e = en;
        tnl::Vector2i abs = e - s;
        abs.abs();

        tnl::Vector2i adv;
        adv.x = (s.x < e.x) ? 1 : -1;
        adv.y = (s.y < e.y) ? 1 : -1;

        if (abs.x > abs.y) {
            lines.resize(abs.x);
            int e = -abs.x;
            for (int i = 0; i < abs.x; ++i) {
                lines[i] = { s.x, s.y };
                s.x += adv.x;
                e += 2 * abs.y;
                if (e >= 0) {
                    s.y += adv.y;
                    e -= 2 * abs.x;
                }
            }
        }
        else {
            lines.resize(abs.y);
            int e = -abs.y;
            for (int i = 0; i < abs.y; ++i) {
                lines[i] = { s.x, s.y };
                s.y += adv.y;
                e += 2 * abs.x;
                if (e >= 0) {
                    s.x += adv.x;
                    e -= 2 * abs.y;
                }
            }

        }

        return lines;
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    void EasyAdjustObjectVelocity(
        float centroid_radius,
        float mass,
        float friction,
        tnl::Vector3& prev_move_vel,
        tnl::Vector3& move_vel,
        tnl::Vector3& center_of_gravity)
    {
        const float mass_max = 100.0f;
        mass = std::clamp(mass, 0.0f, mass_max);

        tnl::Vector3 displacement = move_vel - prev_move_vel;
        prev_move_vel = move_vel;

        center_of_gravity -= displacement * (mass / centroid_radius);
        if (center_of_gravity.length() > centroid_radius) {
            center_of_gravity.normalize();
            center_of_gravity *= centroid_radius;
        }

        // äµê´ÇÃå∏êäó¶( èdÇ¢ÇŸÇ«å∏êäÇµÇ√ÇÁÇ¢ÇÊÇ§åvéZ ) 
        float inertia_decay_rate = mass / (mass_max + 1.0f);
        center_of_gravity *= inertia_decay_rate;

        float friction_decay_rate = (inertia_decay_rate * 0.5f) + (1.0f - friction);
        friction_decay_rate = std::clamp(friction_decay_rate, 0.0f, 1.0f);
        move_vel *= friction_decay_rate;

    }


    //--------------------------------------------------------------------------------------------------------------------------------
    uint32_t CountPolygonIntersections(const std::vector<tnl::Vector2i>& polygon_vertexs, const tnl::Vector2i& point) {
        if (polygon_vertexs.size() < 3) return 0;
        uint32_t crossing_count = 0;
        for (int i = 0; i < polygon_vertexs.size() - 1; ++i) {
            bool y_condition1 = (polygon_vertexs[i].y <= point.y) && (polygon_vertexs[i + 1].y > point.y);
            bool y_condition2 = (polygon_vertexs[i].y > point.y) && (polygon_vertexs[i + 1].y <= point.y);
            if (!(y_condition1 || y_condition2)) continue;
            int32_t vertical_ratio = (point.y - polygon_vertexs[i].y) / (polygon_vertexs[i + 1].y - polygon_vertexs[i].y);
            if (!(point.x < (polygon_vertexs[i].x + (vertical_ratio * (polygon_vertexs[i + 1].x - polygon_vertexs[i].x))))) continue;
            ++crossing_count;
        }
        return crossing_count;
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    void CutOverLineLoop(std::vector<tnl::Vector2i>& line_points, bool is_add_loop_point) {
        for (int i = 2; i < line_points.size(); ++i) {
            tnl::Vector3 v1[2];
            v1[0] = line_points[i - 2];
            v1[1] = line_points[i - 1];
            tnl::Vector2i segment1[2] = { line_points[i - 2], line_points[i - 1] };

            for (int k = i + 4; k < line_points.size(); ++k) {
                tnl::Vector3 v2[2];
                v2[0] = line_points[k - 2];
                v2[1] = line_points[k - 1];
                if (!tnl::IsIntersectLine2D(v1[0], v1[1], v2[0], v2[1])) continue;

                auto np = GetNearestLines(v1[0], v1[1], v2[0], v2[1]);
                line_points[i - 2] = { (int32_t)std::get<0>(np).x, (int32_t)std::get<0>(np).y };
                line_points[k - 1] = { (int32_t)std::get<1>(np).x, (int32_t)std::get<1>(np).y };


                auto begin = line_points.begin() + k;
                if (begin != line_points.end()) {
                    line_points.erase(begin, line_points.end());
                }

                begin = line_points.begin();
                if (0 < (i - 2)) line_points.erase(begin, begin + (i - 2));

                return;
            }
        }

        if (is_add_loop_point) {
            line_points.emplace_back(line_points[0]);
        }
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    CubicSpline::CubicSpline(const std::vector<tnl::Vector3>& v) {

        int n = static_cast<int>(v.size()) - 1;

        for (int i = 0; i <= n; ++i) {
            a_.emplace_back(v[i]);
        }

        for (int i = 0; i <= n; ++i) {
            if (i == 0) {
                c_.emplace_back(tnl::Vector3{ 0, 0, 0 });
            }
            else if (i == n) {
                c_.emplace_back(tnl::Vector3{ 0, 0, 0 });
            }
            else {
                c_.emplace_back(tnl::Vector3{
                    3.0f * (a_[i - 1].x - 2.0f * a_[i].x + a_[i + 1].x),
                    3.0f * (a_[i - 1].y - 2.0f * a_[i].y + a_[i + 1].y),
                    3.0f * (a_[i - 1].z - 2.0f * a_[i].z + a_[i + 1].z)
                    });
            }
        }

        for (int i = 0; i < n; ++i) {
            if (i == 0) {
                w_.emplace_back(tnl::Vector3{ 0, 0, 0 });
            }
            else {
                float x = 4.0f - w_[i - 1].x;
                float y = 4.0f - w_[i - 1].y;
                float z = 4.0f - w_[i - 1].z;
                c_[i].x = (c_[i].x - c_[i - 1].x) / x;
                c_[i].y = (c_[i].y - c_[i - 1].y) / y;
                c_[i].z = (c_[i].z - c_[i - 1].z) / z;
                w_.emplace_back(tnl::Vector3{ 1.0f / x, 1.0f / y, 1.0f / z });
            }
        }

        for (int i = (n - 1); i > 0; --i) {
            c_[i] = c_[i] - c_[i + 1] * w_[i];
        }

        for (int i = 0; i <= n; ++i) {
            if (i == n) {
                d_.emplace_back(tnl::Vector3{ 0,0,0 });
                b_.emplace_back(tnl::Vector3{ 0,0,0 });
            }
            else {
                d_.emplace_back((c_[i + 1] - c_[i]) / 3.0f);
                b_.emplace_back(a_[i + 1] - a_[i] - c_[i] - d_[i]);
            }
        }

    }


    //--------------------------------------------------------------------------------------------------------------------------------
    float SingleOscillationy( eOscStart osc_start, float decay, float period, float progress_time) {

        float ret;
        if (osc_start == eOscStart::CENTER) {
            // å∏êäêUìÆÇÃåˆéÆ y = e^-ax * sin(bx)
            ret = powf(static_cast<float>(std::numbers::e), -decay * progress_time) * sinf(period * progress_time);
        }
        else if (osc_start == eOscStart::STOK) {
            float t = -(static_cast<float>(std::numbers::pi) / 2);
            ret = 1.0f + powf(static_cast<float>(std::numbers::e), -decay * progress_time) * sinf( t + ( period * progress_time) ) ;
        }
        return ret;
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    float UniformLerp(float s, float e, float time_limit, float ct) {
        if (time_limit <= 0 || fabs(e - s) <= FLT_EPSILON) return s;
        if (ct >= time_limit) return e;
        return s + (e - s) * (ct / time_limit);
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    float SmoothLerp(float s, float e, float time_limit, float ct, int strength ) {
        if (time_limit <= 0 || fabs(e - s) <= FLT_EPSILON) return s;
        if (ct >= time_limit) return e;
        strength = (strength > 5) ? 5 : strength;

        float t = 0;
        float si = 0;
        float cct = ct;

        for (int i = 0; i < strength; ++i) {
            float ss = 0;
            float ee = time_limit;

            t = cct / time_limit * 180;
            si = 0.5f + sinf(tnl::ToRadian(-90) + tnl::ToRadian(t)) * 0.5f;
            cct = ss + si * (ee - ss);
        }

        t = cct / time_limit * 180;
        si = 0.5f + sinf(tnl::ToRadian(-90) + tnl::ToRadian(t)) * 0.5f;
        return s + si * (e - s);
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    float AccelLerp(float s, float e, float time_limit, float ct, int strength ) {

        // É[ÉçÇ‹ÇΩÇÕïâÇÃéûä‘êßå¿ÅAÇ‹ÇΩÇÕèâä˙à íuÇ∆èIóπà íuÇ™ìØÇ∂èÍçá
        if (time_limit <= 0 || fabs(e - s) <= FLT_EPSILON) return s;
        if (ct >= time_limit) return e;
        strength = (strength > 5) ? 5 : strength;

        float distance = 0;
        float fluctuation = 0;
        float a = 0;
        float vn = 0;
        float cct = ct;

        for (int i = 0; i < strength; ++i) {
            float ss = 0;
            float ee = time_limit;

            distance = ee - ss;
            fluctuation = (2.0f * distance) / powf(time_limit, 2.0f);

            a = fluctuation;
            vn = (ee - ss) / fabs(ee - ss);
            cct = vn * ((fluctuation * powf(cct, 2)) * 0.5f);
        }

        // é©óRóéâ∫ÇÃåˆéÆÇóòópÇµÇƒâ¡ë¨ìxÇåvéZ 
        // åˆéÆ t = sqrt( 2h / g )
        // ïœå`ÇµÇƒ g = 2h / t^2
        // [ t:éûä‘  h:çÇÇ≥  g:èdóÕâ¡ë¨ìx ]
        distance = e - s;
        fluctuation = (2.0f * distance) / powf(time_limit, 2.0f);

        // ìôâ¡ë¨íºê¸â^ìÆÇÃåˆéÆ x = x0 + ( v0 * t ) + ( 1/2 * a * t^2 )
        // ( v0 * t ) ÇÕèâë¨ÇæÇ™ç°âÒÇÕégÇÌÇ»Ç¢ÇÃÇ≈è»ó™
        // [ x:åªç›à íu  x0:èâä˙à íu  t:éûä‘  a:â¡ë¨ìx ]
        a = fluctuation;
        vn = (e - s) / fabs(e - s);
        return s + vn * ((fluctuation * powf(cct, 2)) * 0.5f);
    }

    //--------------------------------------------------------------------------------------------------------------------------------
    float DecelLerp(float s, float e, float time_limit, float ct) {

        // É[ÉçÇ‹ÇΩÇÕïâÇÃéûä‘êßå¿ÅAÇ‹ÇΩÇÕèâä˙à íuÇ∆èIóπà íuÇ™ìØÇ∂èÍçá
        if (time_limit <= 0 || fabs(e - s) <= FLT_EPSILON) return s;
        if (ct >= time_limit) return e;

        float distance = e - s;
        float fluctuation = (2.0f * distance) / powf(time_limit, 2.0f);

        float a = fluctuation;
        float t = ct;
        float vn = (e - s) / fabs(e - s);
        return e - vn * ((a * powf(time_limit - t, 2)) * 0.5f);
    }


    //--------------------------------------------------------------------------------------------------------------------------------
    PointsLerp::PointsLerp(const std::vector< tnl::Vector3 >& points) {
        points_.resize(points.size());
        section_lengths_.resize(points.size());
        std::copy(points.begin(), points.end(), points_.begin());

        for (size_t i = 1; i < points_.size(); ++i) {
            section_lengths_[i] = (points_[i - 1] - points_[i]).length();
            all_length_ += section_lengths_[i];
            section_lengths_[i] += section_lengths_[i - 1];
        }
    }
    float PointsLerp::getLengthRate(float t) const noexcept {
        float n = 0;
        t = modff(t, &n);
        t = (t < 0.0f) ? 1.0f + t : t;
        return all_length_ * t;
    }

    float PointsLerp::getLengthPoint(int pn) const noexcept {
        if (pn < 0 || pn >= (int)section_lengths_.size()) return 0;
        return section_lengths_[pn];
    }

    int PointsLerp::getPrevPoint(float t) const noexcept {
        float d = getLengthRate(t);
        float c = 0.0f;
        for (size_t i = 0; i < section_lengths_.size(); ++i) {
            if (d >= section_lengths_[i]) continue;
            return (int)(i - 1);
        }
        return 0;
    }

    //------------------------------------------------------------------------------------------------------------
    // ê≥òZäpå`ÇÃï”ÇÃí∑Ç≥ÇïùÇå≥Ç…éÊìæ
    // arg1... ê≥òZäpå`ÇÃïù
    // ret.... ï”ÇÃí∑Ç≥
    float GetHexagonEdgeLenght(float width) {
        return (width / 2.0f) / sqrt(3.0f) * 2.0f;
    }

    //------------------------------------------------------------------------------------------------------------
    // ê≥òZäpå`ÇÃçÇÇ≥ÇïùÇ©ÇÁéÊìæ
    // arg1... ê≥òZäpå`ÇÃïù
    // ret.... ï”ÇÃí∑Ç≥
    float GetHexagonHeight(float width) {
        // ê≥òZäpå`Ç…é˚Ç‹ÇÈâ~ÇÃîºåa
        float radius = width / 2;
        // â~ÇàÕÇ§ê≥òZäpå`ÇÃï”ÇÃí∑Ç≥
        float edge = radius / sqrt(3.0f) * 2.0f;
        // ê≥òZäpå`ÇÃíÜêSÇ©ÇÁÇÃçÇÇ≥
        float height = tnl::Vector3(-radius, edge * 0.5f, 0).length();
        return height * 2;
    }


    //------------------------------------------------------------------------------------------------------------
    // ê≥òZäpå`ÇÃí∏ì_ç¿ïWÇéÊìæ
    // arg1... ê≥òZäpå`ÇÃïù
    // ret.... í∏ì_ç¿ïWîzóÒ
    // tips... ñﬂÇËílÇÃîzóÒÇÕí∑Ç≥ 6 Ç≈Ç†ÇÈÇ±Ç∆Ç…íçà”
    std::vector<tnl::Vector3> GetHexagonVertices(float width) {

        std::vector<tnl::Vector3> vertex;
        // ê≥òZäpå`Ç…é˚Ç‹ÇÈâ~ÇÃîºåa
        float radius = width / 2;
        // â~ÇàÕÇ§ê≥òZäpå`ÇÃï”ÇÃí∑Ç≥
        float edge = radius / sqrt(3.0f) * 2.0f;
        // ê≥òZäpå`ÇÃíÜêSÇ©ÇÁÇÃçÇÇ≥
        float height = tnl::Vector3(-radius, edge * 0.5f, 0).length();

        vertex.resize(6);

        // ÇUäpå`ÇÃç∂è„
        vertex[0] = tnl::Vector3(-radius, edge * 0.5f, 0);
        // ÇUäpå`ÇÃè„
        vertex[1] = tnl::Vector3(0, height, 0);
        // ÇUäpå`ÇÃâEè„
        vertex[2] = tnl::Vector3(radius, edge * 0.5f, 0);
        // ÇUäpå`ÇÃâEâ∫
        vertex[3] = tnl::Vector3(radius, -edge * 0.5f, 0);
        // ÇUäpå`ÇÃâ∫
        vertex[4] = tnl::Vector3(0, -height, 0);
        // ÇUäpå`ÇÃç∂â∫
        vertex[5] = tnl::Vector3(-radius, -edge * 0.5f, 0);

        return std::move(vertex);
    }


}