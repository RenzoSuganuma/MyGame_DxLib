#include <d3d11.h>
#include "../dxlib_ext_camera.h"
#include "dxlib_ext_mesh.h"
#include "../dxlib_ext_data.h"

namespace dxe {

	void Mesh::copySetting(Shared<Mesh> other) {
		other->vbo_ = vbo_;
		other->shape_type_ = shape_type_;
		other->setRenderParam(render_param_);

		for (int i = 0; i < textures_.size(); ++i) {
			other->setTexture(textures_[i], i);
		}

		if (!mesh_name_.empty()) other->setName(mesh_name_);

		if (!idxs_.empty()) {
			other->idxs_.resize(idxs_.size());
			memcpy(other->idxs_.data(), idxs_.data(), sizeof(uint32_t) * idxs_.size());
		}
		if (!vtxs_.empty()) {
			other->vtxs_.resize(vtxs_.size());
			memcpy(other->vtxs_.data(), vtxs_.data(), sizeof(VERTEX3D) * vtxs_.size());
		}
		other->bd_sphere_radius_ = bd_sphere_radius_;
		other->bd_box_size_ = bd_box_size_;
	}



	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::createClone() {
		Shared<Mesh> clone_mesh = Shared<Mesh>(new Mesh());
		if (mv_hdl_) {
			clone_mesh->mv_hdl_ = MV1CreateCloneModel(mv_hdl_);
		}
		clone_mesh->mesh_format_ = mesh_format_;
		copySetting(clone_mesh);
		clone_mesh->desc_ = desc_->createClone();
		return clone_mesh;
	}

	//----------------------------------------------------------------------------------------
	void Mesh::createPlaneIndex( const int div_w, const int div_h, const bool is_left_cycle) {
		int index_num = div_w * div_h * 6;
		idxs_.resize(index_num);

		// 頂点インデックスの計算
		for (int i = 0; i < div_h; ++i) {
			for (int k = 0; k < div_w; ++k) {
				int a = (i * div_w + k) * 6;

				// 左回りインデックス
				if(is_left_cycle) {

					// 左上の三角形
					idxs_[a + 2] = (i * (div_w + 1)) + k;
					idxs_[a + 1] = idxs_[a + 2] + 1;
					idxs_[a + 0] = idxs_[a + 2] + (div_w + 1);

					// 右下の三角形
					idxs_[a + 5] = idxs_[a + 0];
					idxs_[a + 4] = idxs_[a + 1];
					idxs_[a + 3] = idxs_[a + 5] + 1;
				}
				// 右回りインデックス
				else{
					// 左上の三角形
					idxs_[a + 0] = (i * (div_w + 1)) + k;
					idxs_[a + 1] = idxs_[a + 0] + 1;
					idxs_[a + 2] = idxs_[a + 0] + (div_w + 1);
					// 右下の三角形
					idxs_[a + 3] = idxs_[a + 2];
					idxs_[a + 4] = idxs_[a + 1];
					idxs_[a + 5] = idxs_[a + 3] + 1;
				}

			}
		}
	}

	//----------------------------------------------------------------------------------------
	void Mesh::createVBO() {
		vbo_ = std::make_shared<MeshVbo>();
		vbo_->vb_hdl_ = CreateVertexBuffer((int)vtxs_.size(), DX_VERTEX_TYPE_NORMAL_3D);
		vbo_->ib_hdl_ = CreateIndexBuffer((int)idxs_.size(), DX_INDEX_TYPE_32BIT);
		SetVertexBufferData(0, vtxs_.data(), (int)vtxs_.size(), vbo_->vb_hdl_);
		SetIndexBufferData(0, idxs_.data(), (int)idxs_.size(), vbo_->ib_hdl_);
	}

	//----------------------------------------------------------------------------------------
	std::string Mesh::getXFormatString() noexcept {

		char buf[128] = { 0 };

		std::string str;
		str.resize(10000);
		str = "\n Mesh{ \n";

		//str += std::to_string(vtxs_.size()) + "; \n";
		sprintf_s(buf, "%zd ;\0", vtxs_.size());
		str += buf;

		for (uint32_t i = 0; i < vtxs_.size(); ++i) {
			//str += std::to_string(vtxs_[i].pos.x) + "; ";
			//str += std::to_string(vtxs_[i].pos.y) + "; ";
			//str += std::to_string(vtxs_[i].pos.z) + ";";
			//str += ((vtxs_.size() - 1) != i) ? ",\n" : ";\n";

			sprintf_s(buf, "%f ; %f ; %f ;\0", vtxs_[i].pos.x, vtxs_[i].pos.y, vtxs_[i].pos.z);
			str += buf;
			str += ((vtxs_.size() - 1) != i) ? ",\n" : ";\n";

		}

		//str += std::to_string(idxs_.size() / 3) + "; \n";
		sprintf_s(buf, "%zd ;\0", idxs_.size() / 3);
		str += buf;

		for (uint32_t i = 0; i < idxs_.size(); i += 3) {
			//str += "3;";
			//str += std::to_string(idxs_[i + 0]) + ",";
			//str += std::to_string(idxs_[i + 1]) + ",";
			//str += std::to_string(idxs_[i + 2]) + ";";
			sprintf_s(buf, "3 ; %d, %d, %d ;\0", idxs_[i + 0], idxs_[i + 1], idxs_[i + 2]);
			str += buf;
			str += ((idxs_.size() - 3) != i) ? ",\n" : ";\n";

		}
		str += "MeshMaterialList{\n";
		str += "1;\n";

		//str += std::to_string(idxs_.size() / 3) + ";\n";
		sprintf_s(buf, "%zd ;\0", idxs_.size() / 3);
		str += buf;

		for (uint32_t i = 0; i < idxs_.size() / 3; ++i) {
			str += "0";
			str += (((idxs_.size() / 3) - 1) != i) ? ",\n" : ";;\n";
		}
		str += "Material { \n";

		// diff
		//str += std::to_string(render_param_.dxlib_mtrl_.Diffuse.r) + ";";
		//str += std::to_string(render_param_.dxlib_mtrl_.Diffuse.g) + ";";
		//str += std::to_string(render_param_.dxlib_mtrl_.Diffuse.b) + ";";
		//str += std::to_string(render_param_.dxlib_mtrl_.Diffuse.a) + ";;\n";
		sprintf_s(buf, "%f ; %f ; %f ; %f ;;\0"
			, render_param_.dxlib_mtrl_.Diffuse.r
			, render_param_.dxlib_mtrl_.Diffuse.g
			, render_param_.dxlib_mtrl_.Diffuse.b
			, render_param_.dxlib_mtrl_.Diffuse.a );
		str += buf;


		// power
		//str += std::to_string(render_param_.dxlib_mtrl_.Power) + ";\n";
		sprintf_s(buf, "%f ;\0", render_param_.dxlib_mtrl_.Power);
		str += buf;

		// spec
		//str += std::to_string(render_param_.dxlib_mtrl_.Specular.r) + ";";
		//str += std::to_string(render_param_.dxlib_mtrl_.Specular.g) + ";";
		//str += std::to_string(render_param_.dxlib_mtrl_.Specular.b) + ";;\n";

		sprintf_s(buf, "%f ; %f ; %f ;;\0"
			, render_param_.dxlib_mtrl_.Specular.r
			, render_param_.dxlib_mtrl_.Specular.g
			, render_param_.dxlib_mtrl_.Specular.b );
		str += buf;


		// emissive
		//str += std::to_string(render_param_.dxlib_mtrl_.Emissive.r) + ";";
		//str += std::to_string(render_param_.dxlib_mtrl_.Emissive.g) + ";";
		//str += std::to_string(render_param_.dxlib_mtrl_.Emissive.b) + ";;\n";

		sprintf_s(buf, "%f ; %f ; %f ;;\0"
			, render_param_.dxlib_mtrl_.Emissive.r
			, render_param_.dxlib_mtrl_.Emissive.g
			, render_param_.dxlib_mtrl_.Emissive.b );
		str += buf;


		str += "TextureFilename{\n";
		const char t = '"';
		str += t;
		str += "test.bmp";
		str += t;
		str += ";\n";
		str += "}\n}\n}\n";

		//----------------------------------------------------------------------------------------------------------

		str += "MeshNormals{\n";

		//str += std::to_string(vtxs_.size()) + "; \n";
		sprintf_s(buf, "%zd ;\0", vtxs_.size());
		str += buf;


		for (uint32_t i = 0; i < vtxs_.size(); ++i) {
			//str += std::to_string(vtxs_[i].norm.x) + "; ";
			//str += std::to_string(vtxs_[i].norm.y) + "; ";
			//str += std::to_string(vtxs_[i].norm.z) + ";";

			sprintf_s(buf, "%f ; %f ; %f ;\0", vtxs_[i].norm.x, vtxs_[i].norm.y, vtxs_[i].norm.z);
			str += buf;

			str += ((vtxs_.size() - 1) != i) ? ",\n" : ";\n";
		}
		str += std::to_string(idxs_.size() / 3) + "; \n";
		for (uint32_t i = 0; i < idxs_.size(); i += 3) {
			str += "3;";
			//str += std::to_string(idxs_[i + 0]) + ",";
			//str += std::to_string(idxs_[i + 1]) + ",";
			//str += std::to_string(idxs_[i + 2]) + ";";

			sprintf_s(buf, "%d, %d, %d ;\0", idxs_[i + 0], idxs_[i + 1], idxs_[i + 2]);
			str += buf;
			str += ((idxs_.size() - 3) != i) ? ",\n" : ";\n";
		}
		str += "}\n";

		//----------------------------------------------------------------------------------------------------------
		str += "MeshTextureCoords{\n";

		//str += std::to_string(vtxs_.size()) + "; \n";
		sprintf_s(buf, "%zd ;\0", vtxs_.size());
		str += buf;


		for (uint32_t i = 0; i < vtxs_.size(); ++i) {
			//str += std::to_string(vtxs_[i].u) + "; ";
			//str += std::to_string(vtxs_[i].v) + "; ";
			sprintf_s(buf, "%f ; %f ;\0", vtxs_[i].u, vtxs_[i].v);
			str += buf;

			str += ((vtxs_.size() - 1) != i) ? ",\n" : ";\n";
		}
		str += "}\n";
		str += "MeshVertexColors {\n";

		//str += std::to_string(vtxs_.size()) + "; \n";
		sprintf_s(buf, "%zd ;\0", vtxs_.size());
		str += buf;


		for (uint32_t i = 0; i < vtxs_.size(); ++i) {
			//str += std::to_string(i) + "; ";
			//str += "1.000000;1.000000;1.000000;1.000000;";
			sprintf_s(buf, "%d ;1.000000;1.000000;1.000000;1.000000;\0", i);
			str += buf;

			str += ((vtxs_.size() - 1) != i) ? ",\n" : ";\n";
		}
		str += "}\n}\n";


		return g_x_file_hedder_str + str;
	}


	//----------------------------------------------------------------------------------------
	bool Mesh::exportForFileFormatX(const std::string& file_path) {

		std::string save_str = getXFormatString() ;

		FILE* fp = nullptr;
		if (fopen_s(&fp, file_path.c_str(), "w") == 0) {
			fwrite(save_str.data(), save_str.length(), 1, fp);
			fclose(fp);
		}
		return true;
	}

	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateFromTypeDesc(dxe::Mesh::eShapeType shpe_type, dxe::MeshDesc* desc) noexcept {
		switch (shpe_type) {
		case dxe::Mesh::eShapeType::PLANE:
		{
			dxe::MeshDescPlane* d = dynamic_cast<dxe::MeshDescPlane*>(desc);
			return CreatePlaneMV(d->size_wh_, d->div_w_, d->div_h_, d->is_left_cycle_, d->ltop_uv_, d->rbottom_uv_);
		}
		case dxe::Mesh::eShapeType::CUBE:
		{
			dxe::MeshDescCube* d = dynamic_cast<dxe::MeshDescCube*>(desc);
			return CreateCubeMV(d->size_, d->div_w_, d->div_h_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::BOX:
		{
			dxe::MeshDescBox* d = dynamic_cast<dxe::MeshDescBox*>(desc);
			return CreateBoxMV(d->size_,d->tx_left_, d->tx_right_, d->tx_up_, d->tx_down_, d->tx_forword_, d->tx_back_, d->div_w_, d->div_h_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::SPHERE:
		{
			dxe::MeshDescSphere* d = dynamic_cast<dxe::MeshDescSphere*>(desc);
			return CreateSphereMV(d->radius_, d->div_w_, d->div_h_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::TRIANGLE_ISOSCELES:
		{
			dxe::MeshDescTriangleIsosceles* d = dynamic_cast<dxe::MeshDescTriangleIsosceles*>(desc);
			return CreateTriangleIsoscelesMV(d->size_wh_, d->div_w_, d->div_h_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::TRIANGLE_RIGHT:
		{
			dxe::MeshDescTriangleRight* d = dynamic_cast<dxe::MeshDescTriangleRight*>(desc);
			return CreateTriangleRightMV(d->size_wh_, d->div_w_, d->div_h_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::TRIANGLE_EQUILATERAL:
		{
			dxe::MeshDescTriangleEquilateral* d = dynamic_cast<dxe::MeshDescTriangleEquilateral*>(desc);
			return CreateTriangleEquilateralMV(d->edge_lenght_, d->div_w_, d->div_h_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::DOME:
		{
			dxe::MeshDescDome* d = dynamic_cast<dxe::MeshDescDome*>(desc);
			return CreateDomeMV(d->radius_, d->div_w_, d->div_h_, d->angle_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::CONE:
		{
			dxe::MeshDescCone* d = dynamic_cast<dxe::MeshDescCone*>(desc);
			return CreateConeMV(d->radius_, d->height_, d->div_w_, d->div_h_, d->angle_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::CYLINDER:
		{
			dxe::MeshDescCylinder* d = dynamic_cast<dxe::MeshDescCylinder*>(desc);
			return CreateCylinderMV(d->radius_, d->height_, d->div_w_, d->div_h_, d->angle_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::CONE_CYLINDER:
		{
			dxe::MeshDescConeCylinder* d = dynamic_cast<dxe::MeshDescConeCylinder*>(desc);
			return CreateConeCylinderMV(d->top_radius_, d->bottom_radius_, d->height_, d->div_w_, d->div_h_, d->angle_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::DISK:
		{
			dxe::MeshDescDisk* d = dynamic_cast<dxe::MeshDescDisk*>(desc);
			return CreateDiskMV(d->radius_, d->div_w_, d->div_h_, d->angle_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::DISK_RING:
		{
			dxe::MeshDescDiskRing* d = dynamic_cast<dxe::MeshDescDiskRing*>(desc);
			return CreateDiskRingMV(d->radius_, d->thickness_, d->div_w_, d->div_h_, d->angle_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::HOLLOW_OUT_DISK_PLANE:
		{
			dxe::MeshDescHollowOutDiskPlane* d = dynamic_cast<dxe::MeshDescHollowOutDiskPlane*>(desc);
			return CreateHollowOutDiskPlaneMV(d->radius_, d->div_w_, d->div_h_, d->angle_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::BARREL:
		{
			dxe::MeshDescBarrel* d = dynamic_cast<dxe::MeshDescBarrel*>(desc);
			return CreateBarrelMV(d->radius_, d->height_, d->bulge_, d->div_w_, d->div_h_, d->angle_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::HALF_BARREL:
		{
			dxe::MeshDescHalfBarrel* d = dynamic_cast<dxe::MeshDescHalfBarrel*>(desc);
			return CreateHalfBarrelMV(d->radius_, d->height_, d->bulge_, d->div_w_, d->div_h_, d->angle_, d->is_left_cycle_);
		}
		case dxe::Mesh::eShapeType::TORUS:
		{
			dxe::MeshDescTorus* d = dynamic_cast<dxe::MeshDescTorus*>(desc);
			return CreateTorusMV(d->radius_, d->thickness_, d->tessellation_, d->angle_);
		}
		case dxe::Mesh::eShapeType::HEIGHT_MAP:
		{
			dxe::MeshDescHeightMap* d = dynamic_cast<dxe::MeshDescHeightMap*>(desc);
			return CreateFromHeightMapMV(d->file_path_, d->size_whd_.x, d->size_whd_.z, d->size_whd_.y, d->div_w_, d->div_h_);
		}

		}
		return nullptr;
	}


	//----------------------------------------------------------------------------------------
	bool Mesh::exportForFileFormatMV(const std::string& file_path) {
		if (mesh_format_ != eMeshFormat::MESH_FMT_PG) return false;
		Shared<Mesh> mesh;
		exportForFileFormatX("temp.x");
		mesh = CreateFromFileMV("temp.x");
		DeleteFileA("temp.x");		
		MV1SaveModelToMV1File(mesh->mv_hdl_, file_path.c_str());
		return true;
	}


	//----------------------------------------------------------------------------------------
	bool Mesh::ExportForFileFormatMVT(const std::string& file_path, std::vector<Mesh*>& mesh_tbl) {

		for (uint32_t i = 0; i < mesh_tbl.size(); ++i) {
			MV1SaveModelToMV1File(mesh_tbl[i]->mv_hdl_, ( file_path + std::to_string(i)).c_str());
		}

		std::unordered_map<std::string, Shared<dxe::Texture>> tex_map;
		for (uint32_t i = 0; i < mesh_tbl.size(); ++i) {
			for (uint32_t k = 0; k < mesh_tbl[i]->getTextureSlotNum(); ++k) {
				Shared<dxe::Texture> t = mesh_tbl[i]->getTexture(k);
				auto it = tex_map.find(t->getFilePath());
				if (it != tex_map.end()) continue;
				tex_map.insert( std::make_pair( t->getFileName(), t)  );
			}
		}

		uint64_t total_size = 3 ;			// format "mvt"
		uint32_t* sector_size = new uint32_t[mesh_tbl.size()];

		total_size += sizeof(uint32_t);		// sector num			4byte
		total_size += sizeof(uint32_t);		// all texture num		4byte

		for (auto tex : tex_map) {
			total_size += sizeof(uint32_t);						// texture name size	4byte
			total_size += sizeof(uint32_t);						// texture data size	4byte
			total_size += tex.second->getFileName().length();	// texture name
			total_size += tex.second->getDataSize();			// texture data
		}

		for (uint32_t i = 0; i < mesh_tbl.size(); ++i) {
			uint32_t mesh_name_lenght = (uint32_t)mesh_tbl[i]->getName().length();
			uint32_t mesh_data_lenght = (uint32_t)tnl::GetFileSize((file_path + std::to_string(i)).c_str());

			sector_size[i] = 0;
			sector_size[i] += sizeof(uint32_t);			// sector size			4byte

			sector_size[i] += sizeof(uint32_t);			// mesh name size		4byte
			sector_size[i] += sizeof(uint32_t);			// mesh data size		4byte
			sector_size[i] += mesh_name_lenght;			// mesh name data
			sector_size[i] += mesh_data_lenght;			// mesh data

			sector_size[i] += sizeof(uint32_t);			// texture num
			for (uint32_t k = 0; k < mesh_tbl[i]->getTextureSlotNum(); ++k) {
				Shared<dxe::Texture> tex = mesh_tbl[i]->getTexture(k);
				uint32_t tex_name_lenght = (uint32_t)tex->getFileName().length();
				sector_size[i] += sizeof(uint32_t);		// texture slot			4byte
				sector_size[i] += sizeof(uint32_t);		// texture name size	4byte
				sector_size[i] += tex_name_lenght;		// texture name
			}

			sector_size[i] += sizeof(tnl::Vector3);		// position
			sector_size[i] += sizeof(tnl::Vector3);		// scale
			sector_size[i] += sizeof(tnl::Quaternion);	// rotation
			total_size += sector_size[i];
		}

		char* buff = new char[total_size];
		char* p = buff;


		// data format
		*p = 'm'; p++;
		*p = 'v'; p++;
		*p = 't'; p++;

		// write sector num
		*((uint32_t*)p) = (uint32_t)mesh_tbl.size(); p += sizeof(uint32_t);

		// write all texture num
		*((uint32_t*)p) = (uint32_t)tex_map.size(); p += sizeof(uint32_t);

		for (auto tex : tex_map) {
			// texture name size	4byte
			*((uint32_t*)p) = (uint32_t)tex.second->getFileName().length();
			p += sizeof(uint32_t);

			// texture data size	4byte
			*((uint32_t*)p) = tex.second->getDataSize();			
			p += sizeof(uint32_t);

			// texture name
			memcpy(p, tex.second->getFileName().data(), tex.second->getFileName().length());
			p += tex.second->getFileName().length();

			// texture data
			memcpy(p, tex.second->getFileData(), tex.second->getDataSize());
			p += tex.second->getDataSize();
		}


		for (uint32_t i = 0; i < mesh_tbl.size(); ++i) {
			uint32_t tex_num = mesh_tbl[i]->getTextureSlotNum();
			uint32_t mesh_name_length = (uint32_t)mesh_tbl[i]->getName().length();
			uint32_t mesh_data_length = (uint32_t)tnl::GetFileSize((file_path + std::to_string(i)).c_str());

			*((uint32_t*)p) = sector_size[i];			p += sizeof(uint32_t);
			*((uint32_t*)p) = mesh_name_length;			p += sizeof(uint32_t);
			*((uint32_t*)p) = mesh_data_length;			p += sizeof(uint32_t);

			// write mesh name
			memcpy(p, mesh_tbl[i]->getName().data(), mesh_tbl[i]->getName().length() );
			p += mesh_tbl[i]->getName().length() ;

			// write mesh data
			FILE* fp = nullptr;
			fopen_s(&fp, (file_path + std::to_string(i)).c_str(), "rb");
			if ( fp ) {
				fread_s(p, mesh_data_length, mesh_data_length, 1, fp);
				p += mesh_data_length ;
				fclose(fp) ;
			}

			// write texture num
			*((uint32_t*)p) = tex_num;					p += sizeof(uint32_t);

			for (uint32_t k = 0; k < tex_num; ++k) {
				// write texture slot
				*((uint32_t*)p) = k;
				p += sizeof(uint32_t);

				// write texture file name length
				*((uint32_t*)p) = (uint32_t)mesh_tbl[i]->getTexture(k)->getFileName().length();
				p += sizeof(uint32_t);

				// write texture file name
				memcpy(p, mesh_tbl[i]->getTexture(k)->getFileName().data(), mesh_tbl[i]->getTexture(k)->getFileName().length());
				p += mesh_tbl[i]->getTexture(k)->getFileName().length();
			}

			(*(float*)p) = mesh_tbl[i]->pos_.x;					p += sizeof(float);
			(*(float*)p) = mesh_tbl[i]->pos_.y;					p += sizeof(float);
			(*(float*)p) = mesh_tbl[i]->pos_.z;					p += sizeof(float);
			(*(float*)p) = mesh_tbl[i]->scl_.x;					p += sizeof(float);
			(*(float*)p) = mesh_tbl[i]->scl_.y;					p += sizeof(float);
			(*(float*)p) = mesh_tbl[i]->scl_.z;					p += sizeof(float);
			(*(float*)p) = mesh_tbl[i]->rot_.x;					p += sizeof(float);
			(*(float*)p) = mesh_tbl[i]->rot_.y;					p += sizeof(float);
			(*(float*)p) = mesh_tbl[i]->rot_.z;					p += sizeof(float);
			(*(float*)p) = mesh_tbl[i]->rot_.w;					p += sizeof(float);
		}

		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "wb");
		if (fp) {
			fwrite(buff, total_size, 1, fp);
			fclose(fp);
		}

		for (int i = 0; i < mesh_tbl.size(); ++i) {
			DeleteFileA((file_path + std::to_string(i)).c_str());
		}

		delete[] buff;
		delete[] sector_size;

		return true;
	}



	//----------------------------------------------------------------------------------------
	int FileReadFunc(const TCHAR* FilePath, void** FileImageAddr, int* FileSize, void* FileReadFuncData){ return -1; }
	int FileReleaseFunc(void* MemoryAddr, void* FileReadFuncData) { return 0; }
	std::vector<Shared<Mesh>> Mesh::CreateFromFileMVT(const std::string& file_path) {

		uint32_t fsize = (uint32_t)tnl::GetFileSize(file_path.c_str());

		std::vector<Shared<Mesh>> meshs;

		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "rb");
		if (fp) {
			char* buff = new char[fsize];
			fread_s(buff, fsize, fsize, 1, fp);

			char* p = buff;
			p += 3; // format string "mvt" 3byte

			// read sector num
			uint32_t sector_num = *(uint32_t*)p; p += sizeof(uint32_t); 
			meshs.resize(sector_num);

			// read all texture num
			uint32_t all_texture_num = *(uint32_t*)p; p += sizeof(uint32_t);
			meshs.resize(sector_num);

			// read all texture
			for (uint32_t i = 0; i < all_texture_num; ++i) {
				uint32_t tex_name_size = *(uint32_t*)p; p += sizeof(uint32_t);
				uint32_t tex_data_size = *(uint32_t*)p; p += sizeof(uint32_t);
				char* name = new char[ tex_name_size + 1 ];
				memset(name, 0, tex_name_size + 1);
				memcpy(name, p, tex_name_size);
				p += tex_name_size;
				dxe::Texture::CreateFromMemory(p, tex_data_size, std::string(name));
				p += tex_data_size;
				delete[] name;
			}

			for (uint32_t i = 0; i < meshs.size(); ++i) {
				Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
				mesh->mesh_format_ = Mesh::eMeshFormat::MESH_FMT_MV;

				uint32_t sector_size		= *(uint32_t*)p; p += sizeof(uint32_t);
				uint32_t mesh_name_size		= *(uint32_t*)p; p += sizeof(uint32_t);
				uint32_t mesh_data_size		= *(uint32_t*)p; p += sizeof(uint32_t);
				char* name = nullptr;
				if (mesh_name_size) {
					name = new char[mesh_name_size + 1];
					memset(name, 0, mesh_name_size + 1);
					memcpy(name, p, mesh_name_size);
					mesh->setName(std::string(name));
					p += mesh_name_size;
					delete[] name;
				}

				mesh->mv_hdl_ = MV1LoadModelFromMem(p, mesh_data_size, FileReadFunc, FileReleaseFunc, NULL);
				p += mesh_data_size;

				uint32_t tex_num = *(uint32_t*)p; p += sizeof(uint32_t);
				for (uint32_t k = 0; k < tex_num; ++k) {
					uint32_t slot			= *(uint32_t*)p; p += sizeof(uint32_t);
					uint32_t tex_name_size	= *(uint32_t*)p; p += sizeof(uint32_t);
					char* name = new char[tex_name_size + 1];
					memset(name, 0, tex_name_size + 1);
					memcpy(name, p, tex_name_size);
					Shared<dxe::Texture> tex = dxe::Texture::CreateFromFile(std::string(name));
					mesh->setTexture(tex, slot);
					p += tex_name_size;
					delete[] name;
				}

				float x, y, z, w;
				x = *(float*)p; p += sizeof(float);
				y = *(float*)p; p += sizeof(float);
				z = *(float*)p; p += sizeof(float);
				mesh->pos_ = { x, y, z };

				x = *(float*)p; p += sizeof(float);
				y = *(float*)p; p += sizeof(float);
				z = *(float*)p; p += sizeof(float);
				mesh->scl_ = { x, y, z };

				x = *(float*)p; p += sizeof(float);
				y = *(float*)p; p += sizeof(float);
				z = *(float*)p; p += sizeof(float);
				w = *(float*)p; p += sizeof(float);
				mesh->rot_ = { x, y, z, w };

				DxLib::VECTOR dxv = MV1GetMeshMaxPosition(mesh->mv_hdl_, 0);
				mesh->bd_box_size_ = tnl::Vector3(dxv.x, dxv.y, dxv.z) * 2.0f;
				mesh->bd_sphere_radius_ = mesh->bd_box_size_.length() * 0.5f;

				meshs[i] = mesh;
			}

			fclose(fp);
			delete[] buff;
		}

		return meshs;
	}


	//----------------------------------------------------------------------------------------
	std::vector<tnl::Vector3> Mesh::createWorldVertexs() {

		std::vector<tnl::Vector3> verts;
		tnl::Matrix tm = tnl::Matrix::Translation(pos_);
		tnl::Matrix rm = rot_.getMatrix();
		tnl::Matrix sm = tnl::Matrix::Scaling(scl_);
		tnl::Matrix wm = sm * rm * tm;

		verts.resize(idxs_.size() * 3);
		for (int i = 0; i < idxs_.size(); i += 3) {
			VERTEX3D v1 = vtxs_[idxs_[i + 0]];
			VERTEX3D v2 = vtxs_[idxs_[i + 1]];
			VERTEX3D v3 = vtxs_[idxs_[i + 2]];
			tnl::Vector3 p1 = { v1.pos.x, v1.pos.y, v1.pos.z };
			tnl::Vector3 p2 = { v2.pos.x, v2.pos.y, v2.pos.z };
			tnl::Vector3 p3 = { v3.pos.x, v3.pos.y, v3.pos.z };
			tnl::Matrix wm1 = tnl::Matrix::Translation(p1) * wm;
			tnl::Matrix wm2 = tnl::Matrix::Translation(p2) * wm;
			tnl::Matrix wm3 = tnl::Matrix::Translation(p3) * wm;
			p1 = { wm1._41, wm1._42, wm1._43 };
			p2 = { wm2._41, wm2._42, wm2._43 };
			p3 = { wm3._41, wm3._42, wm3._43 };
			verts[i + 0] = p1;
			verts[i + 1] = p2;
			verts[i + 2] = p3;
		}
		return verts;
	}


	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateFromFileMV(const std::string& file_path, const float scl)
	{
		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->shape_type_ = eShapeType::RESOUCE;
		mesh->mesh_format_ = Mesh::eMeshFormat::MESH_FMT_MV;
		mesh->mv_hdl_ = MV1LoadModel(file_path.c_str());
		mesh->scl_ = { scl, scl, scl };
		mesh->desc_ = std::make_shared<dxe::MeshDescResouce>( file_path, scl );
		DxLib::VECTOR dxv = MV1GetMeshMaxPosition(mesh->mv_hdl_, 0);
		mesh->bd_box_size_ = tnl::Vector3(dxv.x, dxv.y, dxv.z) * 2.0f ;
		mesh->bd_sphere_radius_ = mesh->bd_box_size_.length() * 0.5f ;

		return mesh;
	}


	//----------------------------------------------------------------------------------------
	void Mesh::render(const Shared<dxe::Camera> camera) {

		MATRIX view, proj;
		memcpy(view.m, camera->view_.m, sizeof(float) * 16);
		memcpy(proj.m, camera->proj_.m, sizeof(float) * 16);
		SetCameraViewMatrix(view);
		SetupCamera_ProjectionMatrix(proj);

		SetLightEnable(render_param_.is_default_light_enable_);

		// 頂点の射影変換用行列
		tnl::Matrix mt_vertex;

		// 頂点のワールド行列
		tnl::Matrix mt_vtx_world;

		// オブジェクトとしての座標・回転・スケール・ワールド行列
		tnl::Matrix mt_trans;
		tnl::Matrix mt_rotation;
		tnl::Matrix mt_scale;
		tnl::Matrix mt_obj_world;

		// オブジェクトのワールド行列の作成
		mt_trans = tnl::Matrix::Translation(pos_.x, pos_.y, pos_.z);

		mt_rotation = rot_.getMatrix();

		mt_scale = tnl::Matrix::Scaling(scl_.x, scl_.y, scl_.z);

		// ワールド行列を計算 ( スケール行列 x 回転行列 x 座標行列 )
		// ※ 右手座標系は行列の掛け順が逆になるので注意
		mt_obj_world = mt_scale * mt_rotation * mt_trans;

		MATRIX dxm;
		memcpy(dxm.m, mt_obj_world.m, sizeof(float)*16);


		int i_alpha = (int)(alpha_ * 255.0f );
		if (Mesh::eMeshFormat::MESH_FMT_PG == mesh_format_) {
			SetMaterialParam(render_param_.dxlib_mtrl_);
			SetTransformToWorld(&dxm);
			SetUseZBuffer3D(render_param_.is_depth_test_enable_);
			SetWriteZBuffer3D(render_param_.is_write_depth_buffer_enable_);
			SetUseBackCulling(render_param_.dxlib_culling_mode_);
			SetDrawBlendMode(render_param_.dxlib_blend_mode_, i_alpha);
			DrawPrimitiveIndexed3D_UseVertexBuffer(vbo_->vb_hdl_, vbo_->ib_hdl_, DX_PRIMTYPE_TRIANGLELIST, textures_[0]->getDxLibGraphHandle(), true);
		}
		else {
			MATRIX dxim;
			CreateIdentityMatrix(&dxim);
			SetTransformToWorld(&dxim);
			MV1SetUseZBuffer(mv_hdl_, render_param_.is_depth_test_enable_);
			MV1SetWriteZBuffer(mv_hdl_, render_param_.is_write_depth_buffer_enable_);
			MV1SetWireFrameDrawFlag(mv_hdl_, render_param_.is_wire_frame_enable_);
			MV1SetMeshBackCulling(mv_hdl_, 0, render_param_.dxlib_culling_mode_);
			MV1SetSampleFilterMode(mv_hdl_, render_param_.dxlib_sample_filter_mode_);
			MV1SetMeshDrawBlendMode(mv_hdl_, 0, render_param_.dxlib_blend_mode_);
			MV1SetMeshDrawBlendParam(mv_hdl_, 0, i_alpha);
			MV1SetMatrix(mv_hdl_ , dxm);
			MV1DrawModel(mv_hdl_);
		}

		//---------------------------------------------------------------------
		//
		// デバッグライン
		//
		SetWriteZBuffer3D(FALSE);
		SetUseZBuffer3D(FALSE);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_AXIS)			DrawAxis(camera, pos_, rot_, bd_sphere_radius_);
		SetUseZBuffer3D(TRUE);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_OBB)			DrawOBB(camera, pos_, rot_, bd_box_size_);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_BD_SPHERE)	DrawBdSphere(camera, pos_, bd_sphere_radius_);
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_VTX_NML)		drawVtxNormal();
		if (flg_dbg_line_ & Mesh::fDebugLine::FLG_AABB)			DrawAABB(camera, pos_, bd_box_size_);
	}


	//----------------------------------------------------------------------------------------
	void Mesh::reserveShadow() {
		// オブジェクトとしての座標・回転・スケール・ワールド行列
		tnl::Matrix mt_trans;
		tnl::Matrix mt_rotation;
		tnl::Matrix mt_scale;
		tnl::Matrix mt_obj_world;

		// オブジェクトのワールド行列の作成
		mt_trans = tnl::Matrix::Translation(pos_.x, pos_.y, pos_.z);

		mt_rotation = rot_.getMatrix();

		mt_scale = tnl::Matrix::Scaling(scl_.x, scl_.y, scl_.z);

		// ワールド行列を計算 ( スケール行列 x 回転行列 x 座標行列 )
		// ※ 右手座標系は行列の掛け順が逆になるので注意
		mt_obj_world = mt_scale * mt_rotation * mt_trans;

		MATRIX dxm;
		memcpy(dxm.m, mt_obj_world.m, sizeof(float) * 16);

		MATRIX dxim;
		CreateIdentityMatrix(&dxim);
		SetTransformToWorld(&dxim);
		MV1SetMatrix(mv_hdl_, dxm);
		MV1DrawModel(mv_hdl_);
	}

	//----------------------------------------------------------------------------------------
	void Mesh::drawVtxNormal() {
		// オブジェクトのワールド行列の作成
		tnl::Matrix mt_trans;
		tnl::Matrix mt_rot;
		tnl::Matrix mt_obj_world;
		mt_trans = tnl::Matrix::Translation(pos_.x, pos_.y, pos_.z);
		mt_rot = rot_.getMatrix();
		MATRIX im;
		mt_obj_world = mt_rot * mt_trans;
		memcpy(im.m, mt_obj_world.m, sizeof(float) * 16);
		SetTransformToWorld(&im);

		float length = 5.0f;
		for (auto &v : vtxs_) {
			DxLib::VECTOR nv;
			nv.x = v.pos.x + v.norm.x * length;
			nv.y = v.pos.y + v.norm.y * length;
			nv.z = v.pos.z + v.norm.z * length;
			DrawLine3D(v.pos, nv, 0xffffff00);
		}
	}


	//----------------------------------------------------------------------------------------
	Shared<Mesh> Mesh::CreateConvertMV(Shared<Mesh> mesh) {
#if 0
		Shared<Mesh> new_mesh;
		mesh->exportForFileFormatX("temp.x");
		new_mesh = CreateFromFileMV("temp.x");
		mesh->copySetting(new_mesh);
		if (mesh->getTexture()) {
			new_mesh->setTexture(mesh->getTexture());
		}
		new_mesh->desc_ = mesh->getCreateDesc()->createClone();
		DeleteFileA("temp.x");
#else

		Shared<Mesh> new_mesh = Shared<Mesh>(new Mesh());
		new_mesh->mesh_format_ = Mesh::eMeshFormat::MESH_FMT_MV;

		std::string x_format = mesh->getXFormatString();
		new_mesh->mv_hdl_ = MV1LoadModelFromMem(x_format.data(), (int)x_format.size(), FileReadFunc, FileReleaseFunc, NULL);

		DxLib::VECTOR dxv = MV1GetMeshMaxPosition(new_mesh->mv_hdl_, 0);
		new_mesh->bd_box_size_ = tnl::Vector3(dxv.x, dxv.y, dxv.z) * 2.0f;
		new_mesh->bd_sphere_radius_ = new_mesh->bd_box_size_.length() * 0.5f;

		mesh->copySetting(new_mesh);
		if (mesh->getTexture()) {
			new_mesh->setTexture(mesh->getTexture());
		}

		new_mesh->desc_ = mesh->getCreateDesc()->createClone();


#endif

		return new_mesh;
	}

	//----------------------------------------------------------------------------------------
	void Mesh::calcVertexNoraml() {

		for (int i = 0; i < idxs_.size(); i += 3) {
			tnl::Vector3 p1 = { TNL_DEP_V3(vtxs_[idxs_[i + 0]].pos) };
			tnl::Vector3 p2 = { TNL_DEP_V3(vtxs_[idxs_[i + 1]].pos) };
			tnl::Vector3 p3 = { TNL_DEP_V3(vtxs_[idxs_[i + 2]].pos) };

			tnl::Vector3 v1 = p2 - p1;
			tnl::Vector3 v2 = p3 - p1;
			v1.normalize();
			v2.normalize();

			tnl::Vector3 sumv;
			DxLib::VECTOR nml;
			sumv = { TNL_DEP_V3(vtxs_[idxs_[i + 0]].norm) };
			sumv += tnl::Vector3::Cross(v2, v1);
			nml = { TNL_DEP_V3(sumv) };
			vtxs_[idxs_[i + 0]].norm = nml;

			sumv = { TNL_DEP_V3(vtxs_[idxs_[i + 1]].norm) };
			sumv += tnl::Vector3::Cross(v2, v1);
			nml = { TNL_DEP_V3(sumv) };
			vtxs_[idxs_[i + 1]].norm = nml;

			sumv = { TNL_DEP_V3(vtxs_[idxs_[i + 2]].norm) };
			sumv += tnl::Vector3::Cross(v2, v1);
			nml = { TNL_DEP_V3(sumv) };
			vtxs_[idxs_[i + 2]].norm = nml;
		}

		for (int i = 0; i < vtxs_.size(); ++i) {
			tnl::Vector3 n = { TNL_DEP_V3(vtxs_[i].norm) };
			n.normalize();
			vtxs_[i].norm = { TNL_DEP_V3(n) };
		}

	}

}
