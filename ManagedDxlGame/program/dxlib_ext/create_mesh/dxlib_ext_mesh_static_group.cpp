#pragma once
#include "../mesh/dxlib_ext_mesh.h"


namespace dxe {


	Shared<Mesh> Mesh::CreateStaticMeshGroupMV(const Shared<Mesh>& increase_mesh, const std::vector<tnl::Matrix>& mesh_matrixs) {

		Shared<Mesh> mesh = Shared<Mesh>(new Mesh());
		mesh->desc_ = std::make_shared<MeshDescGroup>();
		mesh->shape_type_ = eShapeType::GROUP;
		mesh->setTexture(increase_mesh->getTexture());

		if (increase_mesh->getVertexs().empty()) {

			int err = MV1SetupReferenceMesh(increase_mesh->getDxMvHdl(), -1, 0, 0, -1);
			auto info = MV1GetReferenceMesh(increase_mesh->getDxMvHdl(), -1, 0, 0, -1);
			MV1TerminateReferenceMesh(increase_mesh->getDxMvHdl(), -1, 0, 0, -1);

			uint32_t inst_num = (uint32_t)mesh_matrixs.size();

			mesh->idxs_.resize(inst_num * info.PolygonNum * 3);
			mesh->vtxs_.resize(inst_num * info.PolygonNum * 3);

			tnl::Matrix rm;
			tnl::Vector3 vpos, norm;

			for (uint32_t k = 0; k < inst_num; ++k) {

				for (uint32_t i = 0; i < (uint32_t)info.PolygonNum; ++i) {
					uint32_t a = (k * info.PolygonNum * 3) + i * 3 ;
					mesh->idxs_[a + 0] = a + 0;
					mesh->idxs_[a + 1] = a + 1;
					mesh->idxs_[a + 2] = a + 2;

					rm = mesh_matrixs[k];
					rm._41 = rm._42 = rm._43 = 0;

					vpos = { TNL_DEP_V3(info.Vertexs[info.Polygons[i].VIndex[0]].Position) };
					vpos = tnl::Vector3::TransformCoord(vpos, mesh_matrixs[k]);
					mesh->vtxs_[a + 0].pos = { TNL_DEP_V3(vpos) };

					vpos = { TNL_DEP_V3(info.Vertexs[info.Polygons[i].VIndex[1]].Position) };
					vpos = tnl::Vector3::TransformCoord(vpos, mesh_matrixs[k]);
					mesh->vtxs_[a + 1].pos = { TNL_DEP_V3(vpos) };

					vpos = { TNL_DEP_V3(info.Vertexs[info.Polygons[i].VIndex[2]].Position) };
					vpos = tnl::Vector3::TransformCoord(vpos, mesh_matrixs[k]);
					mesh->vtxs_[a + 2].pos = { TNL_DEP_V3(vpos) };

					norm = { TNL_DEP_V3(info.Vertexs[info.Polygons[i].VIndex[0]].Normal) };
					norm = tnl::Vector3::TransformCoord(norm, rm);
					mesh->vtxs_[a + 0].norm = { TNL_DEP_V3(norm) };

					norm = { TNL_DEP_V3(info.Vertexs[info.Polygons[i].VIndex[1]].Normal) };
					norm = tnl::Vector3::TransformCoord(norm, rm);
					mesh->vtxs_[a + 1].norm = { TNL_DEP_V3(norm) };

					norm = { TNL_DEP_V3(info.Vertexs[info.Polygons[i].VIndex[2]].Normal) };
					norm = tnl::Vector3::TransformCoord(norm, rm);
					mesh->vtxs_[a + 2].norm = { TNL_DEP_V3(norm) };


					mesh->vtxs_[a + 0].dif = info.Vertexs[info.Polygons[i].VIndex[0]].DiffuseColor;
					mesh->vtxs_[a + 1].dif = info.Vertexs[info.Polygons[i].VIndex[1]].DiffuseColor;
					mesh->vtxs_[a + 2].dif = info.Vertexs[info.Polygons[i].VIndex[2]].DiffuseColor;

					mesh->vtxs_[a + 0].spc = info.Vertexs[info.Polygons[i].VIndex[0]].SpecularColor;
					mesh->vtxs_[a + 1].spc = info.Vertexs[info.Polygons[i].VIndex[1]].SpecularColor;
					mesh->vtxs_[a + 2].spc = info.Vertexs[info.Polygons[i].VIndex[2]].SpecularColor;

					mesh->vtxs_[a + 0].su = 0;
					mesh->vtxs_[a + 1].su = 0;
					mesh->vtxs_[a + 2].su = 0;

					mesh->vtxs_[a + 0].sv = 0;
					mesh->vtxs_[a + 1].sv = 0;
					mesh->vtxs_[a + 2].sv = 0;

					mesh->vtxs_[a + 0].u = info.Vertexs[info.Polygons[i].VIndex[0]].TexCoord[0].u;
					mesh->vtxs_[a + 1].u = info.Vertexs[info.Polygons[i].VIndex[1]].TexCoord[0].u;
					mesh->vtxs_[a + 2].u = info.Vertexs[info.Polygons[i].VIndex[2]].TexCoord[0].u;

					mesh->vtxs_[a + 0].v = info.Vertexs[info.Polygons[i].VIndex[0]].TexCoord[0].v;
					mesh->vtxs_[a + 1].v = info.Vertexs[info.Polygons[i].VIndex[1]].TexCoord[0].v;
					mesh->vtxs_[a + 2].v = info.Vertexs[info.Polygons[i].VIndex[2]].TexCoord[0].v;
				}

			}

		}
		else {

			uint32_t vtx_num = (uint32_t)increase_mesh->getVertexs().size();
			uint32_t idx_num = (uint32_t)increase_mesh->getIndexs().size();
			uint32_t inst_num = (uint32_t)mesh_matrixs.size();

			mesh->vtxs_.resize(vtx_num * inst_num);
			mesh->idxs_.resize(idx_num * inst_num);

			tnl::Matrix rm;
			tnl::Vector3 vpos, norm;
			auto verts = increase_mesh->getVertexs();
			for (uint32_t i = 0; i < inst_num; ++i) {
				for (uint32_t k = 0; k < vtx_num; ++k) {
					rm = mesh_matrixs[i];
					rm._41 = rm._42 = rm._43 = 0;
					vpos = tnl::Vector3::TransformCoord({ TNL_DEP_V3(verts[k].pos) }, mesh_matrixs[i]);
					norm = tnl::Vector3::TransformCoord({ TNL_DEP_V3(verts[k].norm) }, rm);
					mesh->vtxs_[(i * vtx_num) + k].pos = { TNL_DEP_V3(vpos) };
					mesh->vtxs_[(i * vtx_num) + k].norm = { TNL_DEP_V3(norm) };
					mesh->vtxs_[(i * vtx_num) + k].su = verts[k].su;
					mesh->vtxs_[(i * vtx_num) + k].sv = verts[k].sv;
					mesh->vtxs_[(i * vtx_num) + k].u = verts[k].u;
					mesh->vtxs_[(i * vtx_num) + k].v = verts[k].v;
					mesh->vtxs_[(i * vtx_num) + k].dif = verts[k].dif;
					mesh->vtxs_[(i * vtx_num) + k].spc = verts[k].spc;
				}
			}

			auto indexs = increase_mesh->getIndexs();
			for (uint32_t i = 0; i < inst_num; ++i) {
				for (uint32_t k = 0; k < idx_num; ++k) {
					mesh->idxs_[(i * idx_num) + k] = indexs[k] + (i * vtx_num);
				}
			}
		}

		mesh->createVBO();
		mesh = CreateConvertMV(mesh);
		return mesh;
	}


}
