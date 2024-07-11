#include "../mesh/dxlib_ext_mesh.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateBox(const tnl::Vector3& size, Shared<Texture> tx_left, Shared<Texture> tx_right, Shared<Texture> tx_up, Shared<Texture> tx_down, Shared<Texture> tx_forword, Shared<Texture> tx_back, const int div_w, const int div_h, const bool is_left_cycle) noexcept
	{
		int scr_hd = 0;
		Shared<Texture> textures[6] = {
			tx_back,
			tx_right,
			tx_forword,
			tx_left,
			tx_up,
			tx_down
		};

		tnl::Vector3 uv_start[6];
		tnl::Vector3 uv_size[6];

		int total_w = 0;
		int max_h = 0;
		for (int i = 0; i < 6; ++i) {
			uint32_t h = textures[i]->getHeight();
			max_h = (int(h) > max_h) ? h : max_h;
			total_w += textures[i]->getWidth();
		}

		int start_w = 0;
		for (int i = 0; i < 6; ++i) {
			uv_start[i].x = static_cast<float>(start_w) / static_cast<float>(total_w);
			uv_start[i].y = 0;
			uv_size[i].x = static_cast<float>(textures[i]->getWidth()) / static_cast<float>(total_w);
			uv_size[i].y = static_cast<float>(textures[i]->getHeight()) / static_cast<float>(max_h);
			start_w += textures[i]->getWidth();
		}

		SetDrawValidGraphCreateFlag(TRUE);
		scr_hd = MakeGraph(total_w, max_h);
		SetDrawValidGraphCreateFlag(FALSE);

		SetDrawScreen(scr_hd);
		ClearDrawScreen();

		int x = 0;
		for (int i = 0; i < 6; ++i) {
			DrawGraph(x, 0, textures[i]->getDxLibGraphHandle(), true);
			x += textures[i]->getWidth();
		}
		SetDrawScreen(DX_SCREEN_BACK);

		Shared<Texture> fix_texture = Texture::CreateFromDxLibGraphHdl( scr_hd );


		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescBox>(size, tx_left, tx_right, tx_up, tx_down, tx_forword, tx_back, div_w, div_h, is_left_cycle);
		mesh->shape_type_ = eShapeType::BOX;

		tnl::Vector3 far_vtx = { 0, 0, 0 };

		// 横並びの頂点数 = ( 横分割数 * 2 ) - ( 横分割数 - 1 )
		// 縦並びの頂点数 = ( 縦分割数 * 2 ) - ( 縦分割数 - 1 )
		// 総頂点数 = 横並びの頂点数 * 縦並びの頂点数
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num * 6);


		float sx = size.x * 0.5f;
		float sy = size.y * 0.5f;
		float sz = size.z * 0.5f;

		float agls[6] = { 0, 270, 180, 90, 90, -90 };
		tnl::Vector3 axis[6] = { 
			tnl::Vector3::up,
			tnl::Vector3::up,
			tnl::Vector3::up,
			tnl::Vector3::up,
			tnl::Vector3::right,
			tnl::Vector3::right
		};

		tnl::Vector3 start_xyz[6] = {
			{ sx, sy, sz }, // back
			{ sz, sy, sx }, // right
			{ sx, sy, sz }, // forword
			{ sz, sy, sz }, // left
			{ sx, sz, sy }, // up
			{ sx, sz, sy }, // down
		};

		tnl::Vector3 size_xy[6] = {
			{ size.x, size.y, sz }, // back
			{ size.z, size.y, sx }, // right
			{ size.x, size.y, sz }, // forword
			{ size.z, size.y, sx }, // left
			{ size.x, size.z, sy }, // up
			{ size.x, size.z, sy }, // down
		};

		for (int n = 0; n < 6; ++n) {
			int s_idx = vtx_num * n;
			tnl::Quaternion rq = tnl::Quaternion::RotationAxis(axis[n], tnl::ToRadian(agls[n]));

			for (int i = 0; i < (div_h + 1); ++i) {
				float v = i / ((float)div_h);
				float y = -start_xyz[n].y + v * size_xy[n].y;

				for (int k = 0; k < (div_w + 1); ++k) {
					float u = k / ((float)div_w);

					int ary = s_idx + (i * (div_w + 1)) + k;

					tnl::Vector3 p, nm;
					p.x = -start_xyz[n].x + u * size_xy[n].x;
					p.y = y;
					p.z = -size_xy[n].z;
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

					mesh->vtxs_[ary].u = uv_start[n].x + ( u * uv_size[n].x );
					mesh->vtxs_[ary].v = (1.0f - (1.0f - uv_size[n].y)) - ( v * uv_size[n].y );

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
		mesh->setTexture( fix_texture );
		return mesh;
	}


	Shared<Mesh> Mesh::CreateBoxMV(const tnl::Vector3& size, Shared<Texture> tx_left, Shared<Texture> tx_right, Shared<Texture> tx_up, Shared<Texture> tx_down, Shared<Texture> tx_forword, Shared<Texture> tx_back, const int div_w, const int div_h, const bool is_left_cycle) noexcept {
		Shared<Mesh> mesh = CreateBox(size, tx_left, tx_right, tx_up, tx_down, tx_forword, tx_back, div_w, div_h, is_left_cycle);
		return CreateConvertMV(mesh);
	}


}

