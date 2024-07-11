#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreatePlane(
		const tnl::Vector3& size_wh, const int div_w, const int div_h, const bool is_left_cycle,
		const tnl::Vector3& ltop_uv, const tnl::Vector3& rbottom_uv ) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescPlane>(size_wh, div_w, div_h, is_left_cycle, ltop_uv, rbottom_uv);
		mesh->shape_type_ = eShapeType::PLANE;

		tnl::Vector3 far_vtx = { 0, 0, 0 };

		// ‰¡•À‚Ñ‚Ì’¸“_” = ( ‰¡•ªŠ„” * 2 ) - ( ‰¡•ªŠ„” - 1 )
		// c•À‚Ñ‚Ì’¸“_” = ( c•ªŠ„” * 2 ) - ( c•ªŠ„” - 1 )
		// ‘’¸“_” = ‰¡•À‚Ñ‚Ì’¸“_” * c•À‚Ñ‚Ì’¸“_”
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		float sx = size_wh.x * 0.5f;
		float sy = size_wh.y * 0.5f;

		float u_size = rbottom_uv.x - ltop_uv.x;
		float v_size = rbottom_uv.y - ltop_uv.y;

		for (int i = 0; i < (div_h + 1); ++i) {
			float v = i / ((float)div_h);
			float y = -sy + v * size_wh.y;

			for (int k = 0; k < (div_w + 1); ++k) {
				float u = k / ((float)div_w);
				tnl::Vector3 vv = { -sx + u * size_wh.x, y, 0 };
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.x = vv.x;
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.y = vv.y;
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.z = 0;

				far_vtx.x = (fabs(vv.x) > fabs(far_vtx.x)) ? vv.x : far_vtx.x;
				far_vtx.y = (fabs(vv.y) > fabs(far_vtx.y)) ? vv.y : far_vtx.y;
				far_vtx.z = (fabs(vv.z) > fabs(far_vtx.z)) ? vv.z : far_vtx.z;

				mesh->vtxs_[(i * (div_w + 1)) + k].norm.x = 0;
				mesh->vtxs_[(i * (div_w + 1)) + k].norm.y = 0;
				//mesh->vtxs_[(i * (div_w + 1)) + k].norm.z = (is_left_cycle)? -1.0f : 1.0f;
				mesh->vtxs_[(i * (div_w + 1)) + k].norm.z = (is_left_cycle) ? 1.0f : -1.0f;

				//mesh->vtxs_[(i * (div_w + 1)) + k].u = u;
				//mesh->vtxs_[(i * (div_w + 1)) + k].v = 1.0f - v;
				mesh->vtxs_[(i * (div_w + 1)) + k].u = ltop_uv.x + ( u * u_size );
//				mesh->vtxs_[(i * (div_w + 1)) + k].v = ( 1.0f - ltop_uv.y ) + ( ( 1.0f - v ) * v_size );
				mesh->vtxs_[(i * (div_w + 1)) + k].v = 1.0f - ( ltop_uv.y + ( v * v_size ) );


				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, is_left_cycle);
		mesh->createVBO();

		mesh->bd_sphere_radius_ = far_vtx.length();
		mesh->bd_box_size_ = far_vtx * 2.0f;

		return mesh;
	}


	Shared<Mesh> Mesh::CreatePlaneMV(const tnl::Vector3& size_wh, const int div_w, const int div_h, const bool is_left_cycle,
		const tnl::Vector3& ltop_uv, const tnl::Vector3& rbottom_uv ) noexcept {
		Shared<Mesh> mesh = CreatePlane(size_wh, div_w, div_h, is_left_cycle, ltop_uv, rbottom_uv);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}

