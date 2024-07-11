#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateDome(const float radius, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescDome>(radius, div_w, div_h, angle, is_left_cycle);
		mesh->shape_type_ = eShapeType::DOME;

		tnl::Vector3 far_vtx = { 0, 0, 0 };

		// �����т̒��_�� = ( �������� * 2 ) - ( �������� - 1 )
		// �c���т̒��_�� = ( �c������ * 2 ) - ( �c������ - 1 )
		// �����_�� = �����т̒��_�� * �c���т̒��_��
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		int stack = div_h;
		int srice = div_w;

		// ���_���W�EUV�E�@���̌v�Z
		tnl::Vector3 vv;
		for (int i = 0; i < (stack + 1); ++i) {
			float v = i / ((float)stack) * 0.5f;
			float y = cosf(tnl::PI * v) * radius;
			float r = sinf(tnl::PI * v) * radius;

			// �������̒��_���d������̂�h������ ( �@�����v�Z�ł��Ȃ��Ȃ�̂� )
			r = (r < FLT_EPSILON) ? 0.00001f : r;

			float angle_ofs = tnl::ToRadian(angle * 0.5f);

			for (int k = 0; k < (srice + 1); ++k) {
				float u = k / ((float)srice);
				u /= (360.0f / angle);

				vv.x = cosf(angle_ofs + 2 * tnl::PI * u) * r;
				vv.y = y;
				vv.z = sinf(angle_ofs + 2 * tnl::PI * u) * r;

				far_vtx.x = (fabs(vv.x) > fabs(far_vtx.x)) ? vv.x : far_vtx.x;
				far_vtx.y = (fabs(vv.y) > fabs(far_vtx.y)) ? vv.y : far_vtx.y;
				far_vtx.z = (fabs(vv.z) > fabs(far_vtx.z)) ? vv.z : far_vtx.z;

				mesh->vtxs_[(i * (srice + 1)) + k].pos = { vv.x, vv.y, vv.z };

				mesh->vtxs_[(i * (srice + 1)) + k].u = 1.0f / (float)srice * k;
				mesh->vtxs_[(i * (srice + 1)) + k].v = (1.0f / (float)stack * i);

				vv.normalize();
				if (is_left_cycle) {
					mesh->vtxs_[(i * (srice + 1)) + k].norm = { -vv.x, -vv.y, -vv.z };
				}
				else {
					mesh->vtxs_[(i * (srice + 1)) + k].norm = { vv.x, vv.y, vv.z };
				}

				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}

		mesh->createPlaneIndex(div_w, div_h, is_left_cycle);
		mesh->createVBO();

		mesh->bd_sphere_radius_ = far_vtx.length();
		mesh->bd_box_size_ = far_vtx * 2.0f;

		return mesh;

	}


	Shared<Mesh> Mesh::CreateDomeMV(const float radius, const int div_w, const int div_h, const float angle, const bool is_left_cycle) noexcept {
		Shared<Mesh> mesh = CreateDome(radius, div_w, div_h, angle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
