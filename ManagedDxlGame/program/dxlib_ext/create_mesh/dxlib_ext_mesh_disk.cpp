#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateDisk(const float radius, const int div_w, const int div_h, const int angle, const bool is_left_cycle) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescDisk>(radius, div_w, div_h, angle, is_left_cycle);
		mesh->shape_type_ = eShapeType::DISK;

		tnl::Vector3 far_vtx = { 0, 0, 0 };

		uint32_t deg = 360;
		deg = (angle < 360) ? 180 : deg;
		deg = (angle < 180) ? 90 : deg;
		deg = (angle < 90) ? 45 : deg;

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		int stack = div_h;
		int srice = div_w;

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (stack + 1); ++i) {
			float v = i / ((float)stack);
			if (360 > deg) v *= 0.5f;

			float y = cosf(tnl::PI * v) * radius;
			float r = sinf(tnl::PI * v) * radius;
			for (int k = 0; k < (srice + 1); ++k) {
				float u = k / ((float)srice);
				if (90 >= deg) u *= 0.5f;
				tnl::Vector3 vv = tnl::Vector3(cosf(tnl::PI * u) * r, y, 0);

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { vv.x, vv.y, vv.z };

				far_vtx.x = (fabs(vv.x) > fabs(far_vtx.x)) ? vv.x : far_vtx.x;
				far_vtx.y = (fabs(vv.y) > fabs(far_vtx.y)) ? vv.y : far_vtx.y;
				far_vtx.z = (fabs(vv.z) > fabs(far_vtx.z)) ? vv.z : far_vtx.z;

				mesh->vtxs_[(i * (srice + 1)) + k].u = 0.5f + (vv.x / radius * 0.5f);
				mesh->vtxs_[(i * (srice + 1)) + k].v = (0.5f + (vv.y / radius * 0.5f));

				mesh->vtxs_[(i * (srice + 1)) + k].norm.x = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.y = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.z = ( is_left_cycle ) ? 1.0f : -1.0f ;

				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, is_left_cycle);
		mesh->createVBO();

		mesh->bd_sphere_radius_ = far_vtx.length();
		mesh->bd_box_size_ = far_vtx * 2.0f;

		return mesh;

	}


	Shared<Mesh> Mesh::CreateDiskMV(const float radius, const int div_w, const int div_h, const int angle, const bool is_left_cycle) noexcept {
		Shared<Mesh> mesh = CreateDisk(radius, div_w, div_h, angle, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
