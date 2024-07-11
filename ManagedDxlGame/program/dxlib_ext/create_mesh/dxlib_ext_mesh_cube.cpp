#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateCube(const float size, const int div_w, const int div_h, const float uv_adjusted, const bool is_left_cycle) noexcept
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescCube>(size, div_w, div_h, is_left_cycle);
		mesh->shape_type_ = eShapeType::CUBE;


		//--------------------------------------------------------------------------
		tnl::Vector3 uv_start[6];
		tnl::Vector3 uv_size[6];

		for (int i = 0; i < 6; ++i) {
			uv_start[i] = { 0, 0, 0 };
			uv_size[i] = { 1, 1, 0 };
		}
		//----------------------------------------------------------------------------------------------------




		tnl::Vector3 far_vtx = { 0, 0, 0 };

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num * 6);


		float sx = size * 0.5f;
		float sy = size * 0.5f;
		float sz = size * 0.5f;

		float agls[6] = { 0, 270, 180, 90, 90, -90 };
		tnl::Vector3 axis[6] = { 
			tnl::Vector3::up,
			tnl::Vector3::up,
			tnl::Vector3::up,
			tnl::Vector3::up,
			tnl::Vector3::right,
			tnl::Vector3::right
		};

		for (int n = 0; n < 6; ++n) {

			int s_idx = vtx_num * n;
			tnl::Quaternion rq = tnl::Quaternion::RotationAxis(axis[n], tnl::ToRadian(agls[n]));

			for (int i = 0; i < (div_h + 1); ++i) {
				float v = i / ((float)div_h);
				float y = -sy + v * size;
				for (int k = 0; k < (div_w + 1); ++k) {
					float u = k / ((float)div_w);

					int ary = s_idx + (i * (div_w + 1)) + k;

					tnl::Vector3 p, nm;
					p.x = -sx + u * size;
					p.y = y;
					p.z = -sz;
					p = tnl::Vector3::TransformCoord(p, rq);
					nm = tnl::Vector3::TransformCoord({0, 0, -1}, rq);

					mesh->vtxs_[ary].pos = { p.x, p.y, p.z };
					if (is_left_cycle) {
						mesh->vtxs_[ary].norm = { -nm.x, -nm.y, -nm.z };
					}
					else {
						mesh->vtxs_[ary].norm = { nm.x, nm.y, nm.z };
					}

					far_vtx.x = (fabs(p.x) > fabs(far_vtx.x)) ? p.x : far_vtx.x;
					far_vtx.y = (fabs(p.y) > fabs(far_vtx.y)) ? p.y : far_vtx.y;
					far_vtx.z = (fabs(p.z) > fabs(far_vtx.z)) ? p.z : far_vtx.z;
					mesh->vtxs_[ary].dif = GetColorU8(255, 255, 255, 255);

					mesh->vtxs_[ary].u = (float)n * (1.0f / 6.0f) + (u / 6.0f);
					mesh->vtxs_[ary].v = 1.0f - v;
					mesh->vtxs_[ary].v = std::clamp(mesh->vtxs_[ary].v, 0.001f, 0.999f);

					float su = 0, eu = 0, wu = 1.0f / 6.0f;
					switch(n) {
					case 0: su = 0.0f / 6.0f; break;
					case 1: su = 1.0f / 6.0f; break;
					case 2: su = 2.0f / 6.0f; break;
					case 3: su = 3.0f / 6.0f; break;
					case 4: su = 4.0f / 6.0f; break;
					case 5: su = 5.0f / 6.0f; break;
					}
					su += wu * ( uv_adjusted / 2 );
					eu = su + wu * ( 1.0f - uv_adjusted );
					mesh->vtxs_[ary].u = std::clamp(mesh->vtxs_[ary].u, su, eu);
				}
			}
		}


		int index_num = div_w * div_h * 6 ;
		mesh->idxs_.resize(index_num * 6);

		// 頂点インデックスの計算
		for (int n = 0; n < 6; ++n) {
			int s_idx = index_num * n;
			int v_idx = vtx_num * n;
			for (int i = 0; i < div_h; ++i) {
				for (int k = 0; k < div_w; ++k) {
					int a = s_idx + (i * div_w + k) * 6;

					if (is_left_cycle) {
						// 左上の三角形
						mesh->idxs_[a + 2] = v_idx + (i * (div_w + 1)) + k;
						mesh->idxs_[a + 1] = mesh->idxs_[a + 2] + 1;
						mesh->idxs_[a + 0] = mesh->idxs_[a + 2] + (div_w + 1);

						// 右下の三角形
						mesh->idxs_[a + 5] = mesh->idxs_[a + 0];
						mesh->idxs_[a + 4] = mesh->idxs_[a + 1];
						mesh->idxs_[a + 3] = mesh->idxs_[a + 5] + 1;

					}
					else {
						// 左上の三角形
						mesh->idxs_[a + 0] = v_idx + (i * (div_w + 1)) + k;
						mesh->idxs_[a + 1] = mesh->idxs_[a + 0] + 1;
						mesh->idxs_[a + 2] = mesh->idxs_[a + 0] + (div_w + 1);
						// 右下の三角形
						mesh->idxs_[a + 3] = mesh->idxs_[a + 2];
						mesh->idxs_[a + 4] = mesh->idxs_[a + 1];
						mesh->idxs_[a + 5] = mesh->idxs_[a + 3] + 1;
					}

				}
			}
		}

		far_vtx.abs();
		mesh->bd_sphere_radius_ = far_vtx.length();
		mesh->bd_box_size_ = far_vtx * 2.0f;

		mesh->createVBO();
		return mesh;
	}


	Shared<Mesh> Mesh::CreateCubeMV(const float size, const int div_w, const int div_h, const float uv_adjusted, const bool is_left_cycle) noexcept {
		Shared<Mesh> mesh = CreateCube(size, div_w, div_h, uv_adjusted, is_left_cycle);
		return CreateConvertMV(mesh);
	}


}

