#include <algorithm>
#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateBarrel(const float radius, const float height, const float bulge, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescBarrel>(radius, height, bulge, div_w, div_h, angle, is_left_cycle);
		mesh->shape_type_ = eShapeType::BARREL;

		tnl::Vector3 far_vtx = { 0, 0, 0 };

		mesh->createPlaneIndex(div_w, div_h, !is_left_cycle);

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		int stack = div_h;
		int srice = div_w;

		// 頂点座標・UV・法線の計算
		for (int i = 0; i < (stack + 1); ++i) {
			float r = radius;
			float angle_ofs = tnl::ToRadian(angle * 0.5f);

			for (int k = 0; k < (srice + 1); ++k) {
				float u = k / ((float)srice);
				u /= (360.0f / angle);

				tnl::Vector3 v;

				v.x = (cosf(angle_ofs + 2 * tnl::PI * u)) * r;
				v.y = (height * 0.5f) - (height / stack * i);
				v.z = (sinf(angle_ofs + 2 * tnl::PI * u)) * r;

				v.x *= 1.0f + sinf(tnl::PI * (height / stack * i) / height ) * (bulge - 1.0f);
				v.z *= 1.0f + sinf(tnl::PI * (height / stack * i) / height ) * (bulge - 1.0f);

				far_vtx.x = (fabs(v.x) > fabs(far_vtx.x)) ? v.x : far_vtx.x;
				far_vtx.y = (fabs(v.y) > fabs(far_vtx.y)) ? v.y : far_vtx.y;
				far_vtx.z = (fabs(v.z) > fabs(far_vtx.z)) ? v.z : far_vtx.z;

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { v.x, v.y, v.z };

				mesh->vtxs_[(i * (srice + 1)) + k].u = 1.0f / (float)srice * k;
				mesh->vtxs_[(i * (srice + 1)) + k].v = (1.0f / (float)stack * i);

				//v = v.xz();
				//v.normalize();
				//if (is_left_cycle) {
				//	mesh->vtxs_[(i * (srice + 1)) + k].norm = { -v.x, -v.y, -v.z };
				//}
				//else {
				//	mesh->vtxs_[(i * (srice + 1)) + k].norm = { v.x, v.y, v.z };
				//}
				mesh->vtxs_[(i * (srice + 1)) + k].norm = { 0, 0, 0 };

				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->calcVertexNoraml();

		far_vtx.abs();
		mesh->bd_sphere_radius_ = far_vtx.length() ;
		mesh->bd_box_size_ = far_vtx * 2.0f ;

		mesh->createVBO();
		return mesh;

	}


	Shared<Mesh> Mesh::CreateBarrelMV(const float radius, const float height, const float bulge, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept {
		Shared<Mesh> mesh = CreateBarrel(radius, height, bulge, div_w, div_h, angle, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
