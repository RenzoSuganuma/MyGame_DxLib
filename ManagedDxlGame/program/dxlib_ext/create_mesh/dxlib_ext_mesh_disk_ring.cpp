#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateDiskRing(const float radius, const float thickness, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());

		mesh->desc_ = std::make_shared<MeshDescDiskRing>(radius, thickness, div_w, div_h, angle, is_left_cycle);
		mesh->shape_type_ = eShapeType::DISK_RING;

		tnl::Vector3 far_vtx = { 0, 0, 0 };

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		int stack = div_h;
		int srice = div_w;

		float thick = (thickness > radius) ? radius : thickness;
		thick /= tnl::PI;
		float inner_radius = radius - thick;

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (stack + 1); ++i) {
			float v = i / ((float)stack) * 0.5f;
			float y = cosf(tnl::PI * v) * radius;
			float r = (1.0f / (float)stack) * (radius - y) * radius;
			r *= (thick / radius);

			float angle_ofs = tnl::ToRadian(angle * 0.5f);

			for (int k = 0; k < (srice + 1); ++k) {
				float u = k / ((float)srice);
				u /= (360.0f / angle);

				tnl::Vector3 vv;
				float rd = angle_ofs + 2 * tnl::PI * u;
				vv.x = (cosf(rd) * r) + (cosf(rd) * inner_radius);
				vv.y = (sinf(rd) * r) + (sinf(rd) * inner_radius);
				vv.z = 0;

				far_vtx.x = (fabs(vv.x) > fabs(far_vtx.x)) ? vv.x : far_vtx.x;
				far_vtx.y = (fabs(vv.y) > fabs(far_vtx.y)) ? vv.y : far_vtx.y;
				far_vtx.z = (fabs(vv.z) > fabs(far_vtx.z)) ? vv.z : far_vtx.z;

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { vv.x, vv.y, vv.z };

				mesh->vtxs_[(i * (srice + 1)) + k].u = 1.0f / (float)srice * k;
				mesh->vtxs_[(i * (srice + 1)) + k].v = (1.0f / (float)stack * i);

				mesh->vtxs_[(i * (srice + 1)) + k].norm.x = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.y = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.z = (is_left_cycle) ? 1.0f : -1.0f;

				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, !is_left_cycle) ;
		mesh->createVBO();

		mesh->bd_sphere_radius_ = far_vtx.length();
		mesh->bd_box_size_ = far_vtx * 2.0f;

		return mesh;

	}

	Shared<Mesh> Mesh::CreateDiskRingMV(const float radius, const float thickness, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept {
		Shared<Mesh> mesh = CreateDiskRing(radius, thickness, div_w, div_h, angle, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
