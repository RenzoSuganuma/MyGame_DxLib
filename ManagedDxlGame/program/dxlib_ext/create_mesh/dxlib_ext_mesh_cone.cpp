#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateCone(const float radius, const float height, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescCone>(radius, height, div_w, div_h, angle, is_left_cycle);
		mesh->shape_type_ = eShapeType::CONE;

		tnl::Vector3 far_vtx = { 0, 0, 0 };

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		int stack = div_h;
		int srice = div_w;

		mesh->createPlaneIndex(div_w, div_h, !is_left_cycle);

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (stack + 1); ++i) {
			float v = i / ((float)stack) * 0.5f;
			float y = cosf(tnl::PI * v) * radius;
//			float r = (1.0f / (float)stack) * (radius - y) * radius * 0.5f;
			float r = (1.0f / (float)stack) * (radius - y) * (float)stack;
			float angle_ofs = tnl::ToRadian(angle * 0.5f);

			// 頭頂部の頂点が重複するのを防ぐため ( 法線が計算できなくなるので )
			r = (r < FLT_EPSILON) ? 0.00001f : r;

			for (int k = 0; k < (srice + 1); ++k) {
				float u = k / ((float)srice);
				u /= (360.0f / angle);

				tnl::Vector3 vv;
				vv.x = (cosf(angle_ofs + 2 * tnl::PI * u)) * r;
				vv.y = y * (height / radius);
				vv.z = (sinf(angle_ofs + 2 * tnl::PI * u)) * r;

				far_vtx.x = (fabs(vv.x) > fabs(far_vtx.x)) ? vv.x : far_vtx.x;
				far_vtx.y = (fabs(vv.y) > fabs(far_vtx.y)) ? vv.y : far_vtx.y;
				far_vtx.z = (fabs(vv.z) > fabs(far_vtx.z)) ? vv.z : far_vtx.z;

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { vv.x, vv.y, vv.z };

				mesh->vtxs_[(i * (srice + 1)) + k].u = 1.0f / (float)srice * k;
				mesh->vtxs_[(i * (srice + 1)) + k].v = (1.0f / (float)stack * i);

				mesh->vtxs_[(i * (srice + 1)) + k].norm = { 0, 0, 0 };
				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->calcVertexNoraml();

		far_vtx.abs();
		mesh->bd_sphere_radius_ = far_vtx.length();
		mesh->bd_box_size_ = far_vtx * 2.0f;

		mesh->createVBO();
		return mesh;

	}


	Shared<Mesh> Mesh::CreateConeMV(const float radius, const float height, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept {
		Shared<Mesh> mesh = CreateCone(radius, height, div_w, div_h, angle, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
