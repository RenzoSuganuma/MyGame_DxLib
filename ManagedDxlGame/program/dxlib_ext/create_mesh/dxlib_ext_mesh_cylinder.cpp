#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateCylinder(const float radius, const float height, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescCylinder>(radius, height, div_w, div_h, angle, is_left_cycle);
		mesh->shape_type_ = eShapeType::CYLINDER;

		tnl::Vector3 far_vtx = { 0, 0, 0 };

		// �����т̒��_�� = ( �������� * 2 ) - ( �������� - 1 )
		// �c���т̒��_�� = ( �c������ * 2 ) - ( �c������ - 1 )
		// �����_�� = �����т̒��_�� * �c���т̒��_��
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		int stack = div_h;
		int srice = div_w;

		// ���_���W�EUV�E�@���̌v�Z
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

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { v.x, v.y, v.z };

				far_vtx.x = (fabs(v.x) > fabs(far_vtx.x)) ? v.x : far_vtx.x;
				far_vtx.y = (fabs(v.y) > fabs(far_vtx.y)) ? v.y : far_vtx.y;
				far_vtx.z = (fabs(v.z) > fabs(far_vtx.z)) ? v.z : far_vtx.z;

				mesh->vtxs_[(i * (srice + 1)) + k].u = 1.0f / (float)srice * k;
				mesh->vtxs_[(i * (srice + 1)) + k].v = (1.0f / (float)stack * i);

				mesh->vtxs_[(i * (srice + 1)) + k].norm = { 0, 0, 0 };
				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, !is_left_cycle);
		mesh->calcVertexNoraml();
		mesh->createVBO();

		mesh->bd_sphere_radius_ = far_vtx.length();
		mesh->bd_box_size_ = far_vtx * 2.0f;

		return mesh;

	}


	Shared<Mesh> Mesh::CreateCylinderMV(const float radius, const float height, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept {
		Shared<Mesh> mesh = CreateCylinder(radius, height, div_w, div_h, angle, is_left_cycle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
