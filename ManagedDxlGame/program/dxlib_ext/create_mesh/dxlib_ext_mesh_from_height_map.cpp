#include "../mesh/dxlib_ext_mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../library/stb_image.h"

namespace dxe {

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateFromHeightMap(unsigned char* pixels, const int t_width, const int t_height, const int t_bpp, const float width, const float depth, const float height_max, const int div_w, const int div_h) {
		uint32_t* argb_tbl = new uint32_t[t_width * t_height];
		for (int y = 0; y < t_height; ++y) {
			for (int x = 0; x < t_width; ++x) {
				uint32_t a = (4 == t_bpp) ? pixels[(y * t_width + x) * t_bpp + 4] : 0xff;
				uint32_t r = pixels[(y * t_width + x) * t_bpp + 0];
				uint32_t g = pixels[(y * t_width + x) * t_bpp + 1];
				uint32_t b = pixels[(y * t_width + x) * t_bpp + 2];
				argb_tbl[(y * t_width) + x] = (a << 24) | (r << 16) | (g << 8) | (b << 0);
			}
		}

		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescHeightMap>("", tnl::Vector3(width, height_max, depth), div_w, div_h);
		mesh->shape_type_ = eShapeType::HEIGHT_MAP;

		mesh->bd_sphere_radius_ = tnl::Vector3(width, 0, depth).length();
		mesh->bd_box_size_ = { width, height_max, depth };

		// ‰¡•À‚Ñ‚Ì’¸“_” = ( ‰¡•ªŠ„” * 2 ) - ( ‰¡•ªŠ„” - 1 )
		// c•À‚Ñ‚Ì’¸“_” = ( c•ªŠ„” * 2 ) - ( c•ªŠ„” - 1 )
		// ‘’¸“_” = ‰¡•À‚Ñ‚Ì’¸“_” * c•À‚Ñ‚Ì’¸“_”
		int vtx_num = ((div_w * 2) - (div_w - 1)) * ((div_h * 2) - (div_h - 1));
		mesh->vtxs_.resize(vtx_num);

		float sx = width * 0.5f;
		float sz = depth * 0.5f;

		for (int i = 0; i < (div_h + 1); ++i) {
			float v = i / ((float)div_h);
			float vv = i / ((float)div_h + 1);
			float z = sz - v * depth;

			for (int k = 0; k < (div_w + 1); ++k) {
				float u = k / ((float)div_w);
				float uu = k / ((float)div_w + 1);
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.x = -sx + u * width;
				mesh->vtxs_[(i * (div_w + 1)) + k].pos.z = z;

				uint32_t tx = (uint32_t)(uu * (float)t_width);
				uint32_t ty = (uint32_t)(vv * (float)t_height);

				uint32_t col = argb_tbl[(t_width * ty) + tx];
				uint32_t r = ((col & 0x00ff0000) >> 16);
				uint32_t g = ((col & 0x0000ff00) >> 8);
				uint32_t b = ((col & 0x000000ff) >> 0);

				float hr = (float)(r + g + b) / (255.0f * 3.0f);

				mesh->vtxs_[(i * (div_w + 1)) + k].pos.y = height_max * hr;

				mesh->vtxs_[(i * (div_w + 1)) + k].u = u;
				mesh->vtxs_[(i * (div_w + 1)) + k].v = v;

				mesh->vtxs_[(i * (div_w + 1)) + k].dif = GetColorU8(255, 255, 255, 255);
			}
		}
		mesh->createPlaneIndex(div_w, div_h, true);

		for (uint32_t i = 0; i < mesh->idxs_.size(); i += 3) {
			DxLib::VECTOR dv1 = mesh->vtxs_[mesh->idxs_[i + 0]].pos;
			DxLib::VECTOR dv2 = mesh->vtxs_[mesh->idxs_[i + 1]].pos;
			DxLib::VECTOR dv3 = mesh->vtxs_[mesh->idxs_[i + 2]].pos;
			tnl::Vector3 v1 = { dv1.x, dv1.y, dv1.z };
			tnl::Vector3 v2 = { dv2.x, dv2.y, dv2.z };
			tnl::Vector3 v3 = { dv3.x, dv3.y, dv3.z };
			tnl::Vector3 va = tnl::Vector3::Normalize(v1 - v2);
			tnl::Vector3 vb = tnl::Vector3::Normalize(v3 - v2);
			tnl::Vector3 n = tnl::Vector3::Cross(va, vb);

			mesh->vtxs_[mesh->idxs_[i + 0]].norm.x += n.x;
			mesh->vtxs_[mesh->idxs_[i + 0]].norm.y += n.y;
			mesh->vtxs_[mesh->idxs_[i + 0]].norm.z += n.z;

			mesh->vtxs_[mesh->idxs_[i + 1]].norm.x += n.x;
			mesh->vtxs_[mesh->idxs_[i + 1]].norm.y += n.y;
			mesh->vtxs_[mesh->idxs_[i + 1]].norm.z += n.z;

			mesh->vtxs_[mesh->idxs_[i + 2]].norm.x += n.x;
			mesh->vtxs_[mesh->idxs_[i + 2]].norm.y += n.y;
			mesh->vtxs_[mesh->idxs_[i + 2]].norm.z += n.z;
		}

		for (uint32_t i = 0; i < mesh->vtxs_.size(); ++i) {
			DxLib::VECTOR dv = mesh->vtxs_[i].norm;
			tnl::Vector3 v = tnl::Vector3::Normalize({ dv.x, dv.y, dv.z });
			mesh->vtxs_[i].norm = { v.x, v.y, v.z };
		}

		mesh->createVBO();

		delete[] argb_tbl;

		return mesh;
	}

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateFromHeightMap(const std::string& file_path, const float width, const float depth, const float height_max, const int div_w, const int div_h)
	{
		unsigned char* pixels = nullptr;
		int t_width, t_height, t_bpp;
		pixels = stbi_load(file_path.c_str(), &t_width, &t_height, &t_bpp, 0);
		Shared<Mesh> mesh = CreateFromHeightMap(pixels, t_width, t_height, t_bpp, width, depth, height_max, div_w, div_h);
		mesh->desc_ = std::make_shared<MeshDescHeightMap>(file_path, tnl::Vector3( width, height_max, depth), div_w, div_h);
		stbi_image_free(pixels);
		return mesh;
	}

	Shared<Mesh> Mesh::CreateFromHeightMapMV(const std::string& file_path, const float width, const float depth, const float height_max, const int div_w, const int div_h) {
		Shared<Mesh> mesh = CreateFromHeightMap(file_path, width, depth, height_max, div_w, div_h);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}

	Shared<Mesh> Mesh::CreateFromHeightMapMV(unsigned char* pixels, const int t_width, const int t_height, const int t_bpp, const float width, const float depth, const float height_max, const int div_w, const int div_h) {
		Shared<Mesh> mesh = CreateFromHeightMap(pixels, t_width, t_height, t_bpp, width, depth, height_max, div_w, div_h);
		mesh = CreateConvertMV(mesh);
		return mesh;
	}

}

