#include "dxlib_ext_mesh_base.h"

namespace dxe {

	//------------------------------------------------------------------------------------------------------------------------
	void MeshBase::loadMaterial(const std::string& file_path) {
		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "rb");
		if (fp) {
			fread(&render_param_.dxlib_mtrl_.Diffuse.a, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Diffuse.r, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Diffuse.g, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Diffuse.b, sizeof(float), 1, fp);

			fread(&render_param_.dxlib_mtrl_.Ambient.a, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Ambient.r, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Ambient.g, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Ambient.b, sizeof(float), 1, fp);

			fread(&render_param_.dxlib_mtrl_.Emissive.a, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Emissive.r, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Emissive.g, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Emissive.b, sizeof(float), 1, fp);

			fread(&render_param_.dxlib_mtrl_.Specular.a, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Specular.r, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Specular.g, sizeof(float), 1, fp);
			fread(&render_param_.dxlib_mtrl_.Specular.b, sizeof(float), 1, fp);

			fread(&render_param_.dxlib_mtrl_.Power, sizeof(float), 1, fp);
			fclose(fp);
			setRenderParam(render_param_);
		}
	}

	//------------------------------------------------------------------------------------------------------------------------
	void MeshBase::saveMaterial(const std::string& file_path) {
		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "wb");
		if (fp) {
			fwrite(&render_param_.dxlib_mtrl_.Diffuse.a, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Diffuse.r, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Diffuse.g, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Diffuse.b, sizeof(float), 1, fp);

			fwrite(&render_param_.dxlib_mtrl_.Ambient.a, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Ambient.r, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Ambient.g, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Ambient.b, sizeof(float), 1, fp);

			fwrite(&render_param_.dxlib_mtrl_.Emissive.a, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Emissive.r, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Emissive.g, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Emissive.b, sizeof(float), 1, fp);

			fwrite(&render_param_.dxlib_mtrl_.Specular.a, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Specular.r, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Specular.g, sizeof(float), 1, fp);
			fwrite(&render_param_.dxlib_mtrl_.Specular.b, sizeof(float), 1, fp);

			fwrite(&render_param_.dxlib_mtrl_.Power, sizeof(float), 1, fp);
			fclose(fp);
		}
	}

}