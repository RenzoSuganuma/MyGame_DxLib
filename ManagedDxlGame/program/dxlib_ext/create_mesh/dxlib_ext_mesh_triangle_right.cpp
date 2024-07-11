#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateTriangleRight(const tnl::Vector3& size_wh, const int div_w, const int div_h, const bool is_left_cycle) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescTriangleRight>(size_wh, div_w, div_h, is_left_cycle);
		mesh->shape_type_ = eShapeType::TRIANGLE_RIGHT;

		mesh->bd_sphere_radius_ = size_wh.length();
		mesh->bd_box_size_ = { size_wh.x, size_wh.y, 1.0f };

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		float width = size_wh.x;
		float height = size_wh.y;

		float ww = width * 0.5f;
		float hh = height * 0.5f;

		int stack = div_h;
		int srice = div_w;
		//	tnl::Vector3 c = (tnl::Vector3(0, hh, 0) + tnl::Vector3(-ww, -hh, 0) + tnl::Vector3(ww, -hh, 0)) / 3.0f;

		// 頂点座標・UV・法線の計算
		tnl::Vector3 vv;
		for (int i = 0; i < (stack + 1); ++i) {
			for (int k = 0; k < (srice + 1); ++k) {
				vv.x = ((1.0f / stack) * i * width) * (1.0f / srice * k);
				vv.y = height - (height / stack * i);
				vv.z = 0;

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { vv.x, vv.y, vv.z };

//				mesh->vtxs_[(i * (srice + 1)) + k].u = 0.5f + (mesh->vtxs_[(i * (srice + 1)) + k].pos.x / width);

				mesh->vtxs_[(i * (srice + 1)) + k].u = (mesh->vtxs_[(i * (srice + 1)) + k].pos.x / width);
				mesh->vtxs_[(i * (srice + 1)) + k].v = 1.0f - (1.0f / (float)stack * i);

				mesh->vtxs_[(i * (srice + 1)) + k].norm.x = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.y = 0;
				mesh->vtxs_[(i * (srice + 1)) + k].norm.z = (is_left_cycle) ? 1.0f : -1.0f;

				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, !is_left_cycle);
		mesh->createVBO();
		return mesh;
	}


	Shared<Mesh> Mesh::CreateTriangleRightMV(const tnl::Vector3& size_wh, const int div_w, const int div_h, const bool is_left_cycle) noexcept {
		Shared<Mesh> mesh = CreateTriangleRight(size_wh, div_w, div_h, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}
	
}

