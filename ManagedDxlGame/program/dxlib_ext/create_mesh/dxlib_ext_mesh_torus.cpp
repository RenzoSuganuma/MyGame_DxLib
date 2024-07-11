#include <directxmath.h>
#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateTorus(const float radius, const float thickness, const int tessellation, const int angle) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescTorus>(radius, thickness, tessellation, angle);
		mesh->shape_type_ = eShapeType::TORUS;

		tnl::Vector3 far_vtx = { 0, 0, 0 };

		uint32_t deg = 360;
		deg = (angle < 360) ? 180 : deg;
		deg = (angle < 180) ? 90 : deg;
		deg = (angle < 90) ? 45 : deg;

		int stride = tessellation + 1;

		std::vector<VERTEX3D> vtxs;
		std::vector<uint32_t> idxs;

		// 全体の円形
		for (int i = 0; i <= tessellation; i++)
		{
			float u = float(i) / float(tessellation);
			float outerAngle = float(i) * tnl::PI * 2.0f / float(tessellation);

			if (180 == deg) {
				u *= 2;
			}
			else if (90 == deg) {
				u *= 4;
			}
			else if (45 == deg) {
				u *= 8;
			}

			tnl::Matrix transform = tnl::Matrix::Translation(radius, 0, 0) * tnl::Matrix::RotationY(outerAngle);

			// チューブ状の輪
			for (int k = 0; k <= tessellation; k++)
			{
				float v = 1 - float(k) / float(tessellation);

				float innerAngle = float(k) * tnl::PI * 2.0f / float(tessellation) + tnl::PI;
				float dx, dy;
				
				DirectX::XMScalarSinCos(&dy, &dx, innerAngle);

				// Create a vertex.
				tnl::Vector3 normal(dx, dy, 0);
				tnl::Vector3 position = normal * (thickness / 2);
				tnl::Vector3 textureCoordinate = textureCoordinate = { u, v, 0 };

				position = tnl::Vector3::Transform(position, transform);
				normal = tnl::Vector3::TransformNormal(normal, transform);

				far_vtx.x = (fabs(position.x) > fabs(far_vtx.x)) ? position.x : far_vtx.x;
				far_vtx.y = (fabs(position.y) > fabs(far_vtx.y)) ? position.y : far_vtx.y;
				far_vtx.z = (fabs(position.z) > fabs(far_vtx.z)) ? position.z : far_vtx.z;

				VERTEX3D vtx;
				vtx.pos = { position.x, position.y, position.z };
				vtx.norm = { normal.x,  normal.y, normal.z };
				vtx.u = textureCoordinate.x;
				vtx.v = textureCoordinate.y;
				vtxs.emplace_back(vtx);

				size_t nextI = (i + 1) % stride;
				size_t nextJ = (k + 1) % stride;

				idxs.emplace_back(uint32_t(i * stride + k));
				idxs.emplace_back(uint32_t(i * stride + nextJ));
				idxs.emplace_back(uint32_t(nextI * stride + k));

				idxs.emplace_back(uint32_t(i * stride + nextJ));
				idxs.emplace_back(uint32_t(nextI * stride + nextJ));
				idxs.emplace_back(uint32_t(nextI * stride + k));

			}

		}

		uint32_t div = 1;
		div = (deg < 360) ? 2 : div;
		div = (deg < 180) ? 4 : div;
		div = (deg < 90) ? 8 : div;

		uint32_t vtx_size = ((uint32_t)vtxs.size() / div) + (tessellation + 1);
		uint32_t idx_size = (uint32_t)idxs.size() - (((tessellation + 1) * 6) * ((tessellation - (tessellation / div) + 1)));

		mesh->vtxs_.resize(vtx_size);
		mesh->idxs_.resize(idx_size);
		memcpy(mesh->vtxs_.data(), vtxs.data(), sizeof(VERTEX3D) * vtx_size);
		memcpy(mesh->idxs_.data(), idxs.data(), sizeof(uint32_t) * idx_size);

		mesh->bd_sphere_radius_ = far_vtx.length();
		mesh->bd_box_size_ = far_vtx * 2.0f;

		mesh->createVBO();
		return mesh;

	}


	Shared<Mesh> Mesh::CreateTorusMV(const float radius, const float thickness, const int tessellation, const int angle) noexcept {
		Shared<Mesh> mesh = CreateTorus(radius, thickness, tessellation, angle);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
