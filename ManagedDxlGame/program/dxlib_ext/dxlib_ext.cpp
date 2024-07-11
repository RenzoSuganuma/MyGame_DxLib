#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "dxlib_ext.h"
#include "dxlib_ext_data.h"

std::string g_drag_file_path;
float g_delta_time = 0;
float g_unlimit_delta_time = 0;

//------------------------------------------------------------------------------------------------------------------------------
void DrawStringEx(float x, float y, int color, const char* _str, ...) {
	char buff[1024] = { 0 };
	va_list argptr;
	va_start(argptr, _str);
	vsprintf_s(buff, _str, argptr);
	va_end(argptr);

	float py = y;
	std::string str = buff;
	int font_size = GetFontSize();
	while (1) {
		auto pos = str.find_first_of("\n");
		if (pos == std::string::basic_string::npos) {
			DrawStringF(x, py, str.c_str(), color);
			break;
		}
		DrawStringF(x, py, str.substr(0, pos).c_str(), color);
		py += font_size;
		str = str.substr(pos+1, str.length()-1);
	}
}


//------------------------------------------------------------------------------------------------------------------------------
void DrawStringEx(int x, int y, int color, const char* _str, ...) {
	char buff[1024] = { 0 };
	va_list argptr;
	va_start(argptr, _str);
	vsprintf_s(buff, _str, argptr);
	va_end(argptr);

	int py = y;
	std::string str = buff;
	int font_size = GetFontSize();
	while (1) {
		auto pos = str.find_first_of("\n");
		if (pos == std::string::basic_string::npos) {
			DrawString(x, py, str.c_str(), color);
			break;
		}
		DrawString(x, py, str.substr(0, pos).c_str(), color);
		py += font_size;
		str = str.substr(pos + 1, str.length() - 1);
	}
}


//------------------------------------------------------------------------------------------------------------------------------
void DrawFpsIndicator(const tnl::Vector3& pos, float delta_time) {
	static float sum_time = 0;
	static float sum_fps = 0;
	static float fps_rate = 0;
	static float unlim_sum_fps = 0;
	static float unlim_fps = 0;

	static float width = 0;
	static int call_num = 0;
	static int font_hdl = 0;

	if (0 == font_hdl) {
		font_hdl = CreateFontToHandle(_T("DrawFpsIndicator"), 10, 9, DX_FONTTYPE_NORMAL, GetDefaultFontHandle());
	}

	sum_time += delta_time;
	sum_fps += GetFPS();
	g_unlimit_delta_time = ( g_unlimit_delta_time > FLT_EPSILON) ? g_unlimit_delta_time : 1.0f ;
	unlim_sum_fps += 1000.0f / g_unlimit_delta_time;
	call_num++;
	if (sum_time > 0.1f) {
		fps_rate = sum_fps / call_num;
		fps_rate /= DXE_FIX_FPS;
		sum_time = 0;
		sum_fps = 0;
		unlim_fps = unlim_sum_fps / (float)call_num;
		unlim_sum_fps = 0;
		call_num = 0;
	}
	width += ((198 * fps_rate) - width) * 0.05f;
	int x = (int)pos.x;
	int y = (int)pos.y;
	DrawBox(x, y, x + 320, y + 10, 0, true);
	DrawBox(x + 1, y + 1, x + 199, y + 9, -1, false);
	DrawBox(x + 2, y + 2, x + (int)width, y + 8, 0xff77ff77, true);
	DrawStringToHandleEx(pos.x + 210, pos.y, -1, font_hdl, "%.1f / %.1f fps", DXE_FIX_FPS * fps_rate, unlim_fps);
}

//------------------------------------------------------------------------------------------------------------------------------
void DrawStringToHandleEx(const float x, const float y, const int color, const int hdl, const char* _str, ...) {
	char buff[255] = { 0 };
	va_list argptr;
	va_start(argptr, _str);
	vsprintf_s(buff, _str, argptr);
	va_end(argptr);
	DrawStringToHandle((int)x, (int)y, buff, color, hdl);
}

//------------------------------------------------------------------------------------------------------------------------------
void DrawLineEx(const tnl::Vector3& p, const tnl::Vector3& v, const float length, const int color) {
	tnl::Vector3 n = v;
	n.normalize();
	n *= length;
	DrawLine((int)p.x, (int)p.y, (int)p.x + (int)n.x, (int)p.y + (int)n.y, color, false);
}

//------------------------------------------------------------------------------------------------------------------------------
void DrawBoxEx(const tnl::Vector3& p, const float width, const float height, const bool fill, const int color) {
	float lx = p.x - width * 0.5f;
	float rx = p.x + width * 0.5f;
	float up = p.y - height * 0.5f;
	float down = p.y + height * 0.5f;
	DrawBox((int)lx, (int)up, (int)rx, (int)down, color, fill);
}

//------------------------------------------------------------------------------------------------------------------------------
void DrawRotaBox(const tnl::Vector3& pos, float w, float h, float rad, int color) {

	float hw = w / 2.0f;
	float hh = h / 2.0f;
	tnl::Vector3 v[4];
	tnl::Quaternion q = tnl::Quaternion::RotationAxis({ 0, 0, 1 }, rad);
	v[0] = tnl::Vector3(-hw, -hh, 0);
	v[1] = tnl::Vector3(hw, -hh, 0);
	v[2] = tnl::Vector3(hw, hh, 0);
	v[3] = tnl::Vector3(-hw, hh, 0);
	v[0] = pos + tnl::Vector3::TransformCoord(v[0], q);
	v[1] = pos + tnl::Vector3::TransformCoord(v[1], q);
	v[2] = pos + tnl::Vector3::TransformCoord(v[2], q);
	v[3] = pos + tnl::Vector3::TransformCoord(v[3], q);

	DrawLine((int)v[0].x, (int)v[0].y, (int)v[1].x, (int)v[1].y, color);
	DrawLine((int)v[1].x, (int)v[1].y, (int)v[2].x, (int)v[2].y, color);
	DrawLine((int)v[2].x, (int)v[2].y, (int)v[3].x, (int)v[3].y, color);
	DrawLine((int)v[3].x, (int)v[3].y, (int)v[0].x, (int)v[0].y, color);

}

//------------------------------------------------------------------------------------------------------------------------------
void DrawHexagon(const tnl::Vector3 pos, float width, int color ) {

	// 正六角形に収まる円の半径
	float radius = width / 2;
	// 円を囲う正六角形の辺の長さ
	float edge = radius / sqrt(3.0f) * 2.0f;
	// 正六角形の中心からの高さ
	float height = tnl::Vector3(-radius, edge * 0.5f, 0).length();

	tnl::Vector3 vertex[6];
	// ６角形の左上
	vertex[0] = pos + tnl::Vector3(-radius, edge * 0.5f, 0);
	// ６角形の上
	vertex[1] = pos + tnl::Vector3(0, height, 0);
	// ６角形の右上
	vertex[2] = pos + tnl::Vector3(radius, edge * 0.5f, 0);
	// ６角形の右下
	vertex[3] = pos + tnl::Vector3(radius, -edge * 0.5f, 0);
	// ６角形の下
	vertex[4] = pos + tnl::Vector3(0, -height, 0);
	// ６角形の左下
	vertex[5] = pos + tnl::Vector3(-radius, -edge * 0.5f, 0);

	//DrawCircle(pos.x, pos.y, radius, 0xffff00ff, false);
	for (int i = 0; i < 5; ++i) {
		DrawLine((int)vertex[i].x, (int)vertex[i].y, (int)vertex[i + 1].x, (int)vertex[i + 1].y, color);
	}
	DrawLine((int)vertex[5].x, (int)vertex[5].y, (int)vertex[0].x, (int)vertex[0].y, color);
}

//------------------------------------------------------------------------------------------------------------------------------
void DrawLine3DEx(const Shared<dxe::Camera> camera, const tnl::Vector3& st, const tnl::Vector3& en, const int color ) {

	MATRIX view, proj;
	memcpy(view.m, camera->view_.m, sizeof(float) * 16);
	memcpy(proj.m, camera->proj_.m, sizeof(float) * 16);
	SetCameraViewMatrix(view);
	SetupCamera_ProjectionMatrix(proj);

	MATRIX im;
	CreateIdentityMatrix(&im);
	SetTransformToWorld(&im);

	DrawLine3D({ st.x, st.y, st.z }, { en.x, en.y, en.z }, color );
}


//------------------------------------------------------------------------------------------------------------------------------
void DrawGridGround(const Shared<dxe::Camera> camera, const float square_size, int row_num, int color) {

	MATRIX view, proj;
	memcpy(view.m, camera->view_.m, sizeof(float) * 16);
	memcpy(proj.m, camera->proj_.m, sizeof(float) * 16);
	SetCameraViewMatrix(view);
	SetupCamera_ProjectionMatrix(proj);

	MATRIX im;
	CreateIdentityMatrix(&im);
	SetTransformToWorld(&im);

	row_num = (1 == row_num % 2) ? row_num + 1 : row_num;

	float l = square_size * row_num * 0.5f;
	float n = -l;
	for (int i = 0; i < row_num + 1; ++i) {
		if ((row_num >> 1) == i) {
			DrawLine3D({ n, 0, l }, { n, 0, -l }, 0xff0000ff);
			DrawLine3D({ l, 0, n }, { -l, 0, n }, 0xffff0000);
		}
		else {
			DrawLine3D({ n, 0, l }, { n, 0, -l }, color);
			DrawLine3D({ l, 0, n }, { -l, 0, n }, color);
		}
		n += square_size;
	}
	DrawLine3D({ 0, l, 0 }, { 0, -l, 0 }, 0xff00ff00);

}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawHexagonGround(const Shared<dxe::Camera> camera, const float hex_width, int row_num, bool is_draw_center_line, int color) {

	MATRIX view, proj;
	memcpy(view.m, camera->view_.m, sizeof(float) * 16);
	memcpy(proj.m, camera->proj_.m, sizeof(float) * 16);
	SetCameraViewMatrix(view);
	SetupCamera_ProjectionMatrix(proj);

	MATRIX im;
	CreateIdentityMatrix(&im);
	SetTransformToWorld(&im);


	float radius = hex_width / 2;
	float edge = radius / sqrt(3.0f) * 2.0f;
	float height = tnl::Vector3(-radius, edge * 0.5f, 0).length();

	tnl::Vector3 vertex[6];
	vertex[0] = tnl::Vector3(-radius, 0, edge * 0.5f);
	vertex[1] = tnl::Vector3(0, 0, height);
	vertex[2] = tnl::Vector3(radius, 0, edge * 0.5f);
	vertex[3] = tnl::Vector3(radius, 0, -edge * 0.5f);
	vertex[4] = tnl::Vector3(0, 0, -height);
	vertex[5] = tnl::Vector3(-radius, 0, -edge * 0.5f);

	tnl::Vector3 s = { -hex_width * row_num / 2, 0, (hex_width / 4) + height * row_num / 2 };

	for (int i = 0; i < row_num; ++i) {

		for (int k = 0; k < row_num; ++k) {

			float px = s.x + (k * radius * 2.0f);
			float pz = s.z - (i * height * 1.5f);
			// 偶数段は右にずらして描画
			px += (i % 2) * radius;

			tnl::Vector3 n = { px, 0, pz };

			for (int i = 0; i < 5; ++i) {
				DrawLine3D(DxLib::VECTOR( TNL_DEP_V3( ( n + vertex[i] )) ), DxLib::VECTOR(TNL_DEP_V3((n + vertex[i + 1]))), color);
			}
			DrawLine3D(DxLib::VECTOR(TNL_DEP_V3((n + vertex[5]))), DxLib::VECTOR(TNL_DEP_V3((n + vertex[0]))), color);
		}
	}

	if (!is_draw_center_line) return;
	DrawLine3D({ 0, 1000, 0 }, { 0, -1000, 0 }, 0xff00ff00);
	DrawLine3D({ -1000, 0, 0 }, { 1000, 0, 0 }, 0xffff0000);
	DrawLine3D({ 0, 0, 1000 }, { 0, 0, -1000 }, 0xff0000ff);

}


//------------------------------------------------------------------------------------------------------------------------------
void DrawAxis(const Shared<dxe::Camera> camera, const tnl::Vector3& pos, const tnl::Quaternion& rot, const float length)
{
	MATRIX view, proj;
	memcpy(view.m, camera->view_.m, sizeof(float) * 16);
	memcpy(proj.m, camera->proj_.m, sizeof(float) * 16);
	SetCameraViewMatrix(view);
	SetupCamera_ProjectionMatrix(proj);

	// オブジェクトのワールド行列の作成
	tnl::Matrix mt_trans;
	tnl::Matrix mt_rot;
	tnl::Matrix mt_obj_world;
	mt_trans = tnl::Matrix::Translation(pos.x, pos.y, pos.z);
	mt_rot = rot.getMatrix();
	MATRIX im;
	mt_obj_world = mt_rot * mt_trans;
	memcpy(im.m, mt_obj_world.m, sizeof(float) * 16);
	SetTransformToWorld(&im);

	tnl::Vector3 up(0, length, 0);
	tnl::Vector3 right(length, 0, 0);
	tnl::Vector3 depth(0, 0, length);
	DrawLine3D({ 0, 0, 0 }, { up.x, up.y, up.z }, 0xff00ff00);
	DrawLine3D({ 0, 0, 0 }, { right.x, right.y, right.z }, 0xffff0000);
	DrawLine3D({ 0, 0, 0 }, { depth.x, depth.y, depth.z }, 0xff0000ff);
}


//------------------------------------------------------------------------------------------------------------------------------
void DrawOBB(const Shared<dxe::Camera> camera, const tnl::Vector3& pos, const tnl::Quaternion& rot, const tnl::Vector3& size, const int color)
{

	MATRIX view, proj;
	memcpy(view.m, camera->view_.m, sizeof(float) * 16);
	memcpy(proj.m, camera->proj_.m, sizeof(float) * 16);
	SetCameraViewMatrix(view);
	SetupCamera_ProjectionMatrix(proj);

	// オブジェクトのワールド行列の作成
	tnl::Matrix mt_trans;
	tnl::Matrix mt_rot;
	tnl::Matrix mt_obj_world;
	mt_trans = tnl::Matrix::Translation(pos.x, pos.y, pos.z);
	mt_rot = rot.getMatrix();
	MATRIX im;
	mt_obj_world = mt_rot * mt_trans;
	memcpy(im.m, mt_obj_world.m, sizeof(float) * 16);
	SetTransformToWorld(&im);


	float w = size.x * 0.5f;
	float h = size.y * 0.5f;
	float d = size.z * 0.5f;
	tnl::Vector3 v[8] = {
		{ -w,  h,  d }, {  w,  h,  d },  {  w,  h,  -d }, { -w,  h, -d },
		{ -w, -h,  d }, {  w, -h,  d },  {  w, -h,  -d }, { -w, -h, -d }
	};
	uint32_t col = color;
	for (int i = 0; i < 3; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 1].x, v[i + 1].y , v[i + 1].z }, col);
	for (int i = 0; i < 3; ++i) DrawLine3D({ v[4 + i].x, v[4 + i].y , v[4 + i].z }, { v[4 + i + 1].x, v[4 + i + 1].y , v[4 + i + 1].z }, col);
	for (int i = 0; i < 4; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 4].x, v[i + 4].y , v[i + 4].z }, col);
	DrawLine3D({ v[3].x, v[3].y , v[3].z }, { v[0].x, v[0].y , v[0].z }, col);
	DrawLine3D({ v[7].x, v[7].y , v[7].z }, { v[4].x, v[4].y , v[4].z }, col);
}


//------------------------------------------------------------------------------------------------------------------------------
void DrawAABB(const Shared<dxe::Camera> camera, const tnl::Vector3& pos, const tnl::Vector3& size, const int color)
{
	MATRIX view, proj;
	memcpy(view.m, camera->view_.m, sizeof(float) * 16);
	memcpy(proj.m, camera->proj_.m, sizeof(float) * 16);
	SetCameraViewMatrix(view);
	SetupCamera_ProjectionMatrix(proj);

	// オブジェクトのワールド行列の作成
	tnl::Matrix mt_trans;
	tnl::Matrix mt_obj_world;
	mt_trans = tnl::Matrix::Translation(pos.x, pos.y, pos.z);
	MATRIX im;
	mt_obj_world = mt_trans;
	memcpy(im.m, mt_obj_world.m, sizeof(float) * 16);
	SetTransformToWorld(&im);

	float w = size.x * 0.5f;
	float h = size.y * 0.5f;
	float d = size.z * 0.5f;
	tnl::Vector3 v[8] = {
		{ -w,  h,  d }, {  w,  h,  d },  {  w,  h,  -d }, { -w,  h, -d },
		{ -w, -h,  d }, {  w, -h,  d },  {  w, -h,  -d }, { -w, -h, -d }
	};
	uint32_t col = color;
	for (int i = 0; i < 3; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 1].x, v[i + 1].y , v[i + 1].z }, col);
	for (int i = 0; i < 3; ++i) DrawLine3D({ v[4 + i].x, v[4 + i].y , v[4 + i].z }, { v[4 + i + 1].x, v[4 + i + 1].y , v[4 + i + 1].z }, col);
	for (int i = 0; i < 4; ++i) DrawLine3D({ v[i].x, v[i].y , v[i].z }, { v[i + 4].x, v[i + 4].y , v[i + 4].z }, col);
	DrawLine3D({ v[3].x, v[3].y , v[3].z }, { v[0].x, v[0].y , v[0].z }, col);
	DrawLine3D({ v[7].x, v[7].y , v[7].z }, { v[4].x, v[4].y , v[4].z }, col);

}


//------------------------------------------------------------------------------------------------------------------------------
void DrawBdSphere(const Shared<dxe::Camera> camera, const tnl::Vector3& pos, const float radius)
{
	MATRIX view, proj;
	memcpy(view.m, camera->view_.m, sizeof(float) * 16);
	memcpy(proj.m, camera->proj_.m, sizeof(float) * 16);
	SetCameraViewMatrix(view);
	SetupCamera_ProjectionMatrix(proj);

	// オブジェクトのワールド行列の作成
	tnl::Matrix mt_trans;
	tnl::Matrix mt_rot;
	tnl::Matrix mt_obj_world;
	mt_trans = tnl::Matrix::Translation(pos.x, pos.y, pos.z);
	MATRIX im;
	mt_obj_world = mt_rot * mt_trans;
	memcpy(im.m, mt_obj_world.m, sizeof(float) * 16);
	SetTransformToWorld(&im);

	const int ring_vtx_num = 16;
	int s = ring_vtx_num - 1;
	int e = 0;

	tnl::Vector3 vx[ring_vtx_num];
	tnl::Vector3 vy[ring_vtx_num];
	tnl::Vector3 vz[ring_vtx_num];

	// X軸リング
	for (int i = 0; i < ring_vtx_num; ++i) {
		vx[i].x = 0;
		vx[i].y = cos(tnl::ToRadian(360.0f / ring_vtx_num) * i) * radius;
		vx[i].z = sin(tnl::ToRadian(360.0f / ring_vtx_num) * i) * radius;
	}
	for (int i = 0; i < (ring_vtx_num - 1); ++i) {
		DrawLine3D({ vx[i].x, vx[i].y, vx[i].z },
			{ vx[i + 1].x, vx[i + 1].y, vx[i + 1].z }, 0xffff0000);
	}
	DrawLine3D({ vx[s].x, vx[s].y, vx[s].z },
		{ vx[e].x, vx[e].y, vx[e].z }, 0xffff0000);


	// Y軸リング
	for (int i = 0; i < ring_vtx_num; ++i) {
		vy[i].x = sin(tnl::ToRadian(360.0f / ring_vtx_num) * i) * radius;
		vy[i].y = 0;
		vy[i].z = cos(tnl::ToRadian(360.0f / ring_vtx_num) * i) * radius;
	}
	for (int i = 0; i < (ring_vtx_num - 1); ++i) {
		DrawLine3D({ vy[i].x, vy[i].y, vy[i].z },
			{ vy[i + 1].x, vy[i + 1].y, vy[i + 1].z }, 0xff00ff00);
	}
	DrawLine3D({ vy[s].x, vy[s].y, vy[s].z },
		{ vy[e].x, vy[e].y, vy[e].z }, 0xff00ff00);

	// Z軸リング
	for (int i = 0; i < ring_vtx_num; ++i) {
		vz[i].x = sin(tnl::ToRadian(360.0f / ring_vtx_num) * i) * radius;
		vz[i].y = cos(tnl::ToRadian(360.0f / ring_vtx_num) * i) * radius;
		vz[i].z = 0;
	}
	for (int i = 0; i < (ring_vtx_num - 1); ++i) {
		DrawLine3D({ vz[i].x, vz[i].y, vz[i].z },
			{ vz[i + 1].x, vz[i + 1].y, vz[i + 1].z }, 0xff2255ff);
	}
	DrawLine3D({ vz[s].x, vz[s].y, vz[s].z },
		{ vz[e].x, vz[e].y, vz[e].z }, 0xff2266ff);

}

//------------------------------------------------------------------------------------------------------------------------------
std::string GetDragFilePathReadTrigger() {
	std::string path = g_drag_file_path;
	g_drag_file_path.clear();
	return path;
}



//------------------------------------------------------------------------------------------------------------------------------
void DrawDefaultLightGuiController() {

	class DefaultLightParam {
	public :
		DefaultLightParam() {
			DxLib::VECTOR dir = GetLightDirection();
			DxLib::COLOR_F diff = GetLightDifColor();
			DxLib::COLOR_F ambi = GetLightAmbColor();
			DxLib::COLOR_F spec = GetLightSpcColor();

			dir_ = { dir.x, dir.y, dir.z };
			diff_ = { diff.r, diff.g, diff.b };
			ambi_ = { ambi.r, ambi.g, ambi.b };
			spec_ = { spec.r, spec.g, spec.b };
		}
		tnl::Vector3 dir_ = { 1.0f, -1.0f, 1.0f };
		tnl::Vector3 diff_ = { 1.0f, 1.0f, 1.0f };
		tnl::Vector3 ambi_ = { 0.33f, 0.33f, 0.33f };
		tnl::Vector3 spec_ = { 1.0f, 1.0f, 1.0f };

		tnl::Vector3 getDir() { return dir_; }
		tnl::Vector3 getDiff() { return diff_; }
		tnl::Vector3 getAmbi() { return ambi_; }
		tnl::Vector3 getSpec() { return spec_; }

		void setDir(const tnl::Vector3& dir) { 
			dir_ = dir ; 
			SetLightDirection({ dir_.x, dir_.y, dir_.z });
		}
		void setDiff(const tnl::Vector3& diff) { 
			diff_ = diff ; 
			SetLightDifColor({ diff_.x, diff_.y, diff_.z, 1.0f });
		}
		void setAmbi(const tnl::Vector3& ambi) { 
			ambi_ = ambi ; 
			SetLightAmbColor({ ambi_.x, ambi_.y, ambi_.z, 1.0f });
		}
		void setSpec(const tnl::Vector3& spec) { 
			spec_ = spec; 
			SetLightSpcColor({ spec_.x, spec_.y, spec_.z, 1.0f });
		}

	};

	static bool is_btn_save_forcus_ = false;
	static bool is_btn_save_select_ = false;
	static Shared<DefaultLightParam> param = nullptr ;
	static Shared<dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >> dir_ = nullptr;
	static Shared<dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >> diff_ = nullptr ;
	static Shared<dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >> ambi_ = nullptr ;
	static Shared<dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >> global_emis_ = nullptr ;
	static Shared<dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >> spec_ = nullptr ;
	if (!param) {
		param = std::make_shared<DefaultLightParam>();
		dir_  = Shared<dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >>(
				new dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >
				( param.get()
				, &DefaultLightParam::getDir
				, &DefaultLightParam::setDir
				, { DXE_WINDOW_WIDTH - 330, 30 }
				, { -1, -1, -1 }
				, { 1, 1, 1 }
				, "direction"));
		diff_ = Shared<dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >>(
				new dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >
				( param.get()
				, &DefaultLightParam::getDiff
				, &DefaultLightParam::setDiff
				, { DXE_WINDOW_WIDTH - 330, 90 }
				, { 0, 0, 0 }
				, { 1, 1, 1 }
				, "diffuse"));
		ambi_ = Shared<dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >>(
				new dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >
				( param.get()
				, &DefaultLightParam::getAmbi
				, &DefaultLightParam::setAmbi
				, { DXE_WINDOW_WIDTH - 330, 150 }
				, { 0, 0, 0 }
				, { 1, 1, 1 }
				, "ambient"));
		spec_ = Shared<dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >>(
				new dxe::GuiValueSlider< DefaultLightParam, tnl::Vector3 >
				( param.get()
				, &DefaultLightParam::getSpec
				, &DefaultLightParam::setSpec
				, { DXE_WINDOW_WIDTH - 330, 210 }
				, { 0, 0, 0 }
				, { 1, 1, 1 }
				, "specular"));
	}

	dir_->update();
	diff_->update();
	ambi_->update();
	spec_->update();

	dir_->draw();
	diff_->draw();
	ambi_->draw();
	spec_->draw();

	tnl::Vector3 ms = tnl::Input::GetMousePosition();
	int tx = DXE_WINDOW_WIDTH - 150 ;
	int ty = 0;
	int bx = DXE_WINDOW_WIDTH ;
	int by = 20;

	is_btn_save_forcus_ = false;
	if (ms.x > tx && ms.x < bx && ms.y > ty && ms.y < by) {
		is_btn_save_forcus_ = true;
		if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
			is_btn_save_select_ = true;
		}
	}
	if (is_btn_save_select_) {
		if (tnl::Input::IsMouseTrigger(eMouseTrigger::OUT_LEFT)) {
			is_btn_save_select_ = false;

			FILE* fp = nullptr;
			fopen_s(&fp, "directional_light_parameter.bin", "wb");
			if (fp) {
				fwrite(&param->dir_, sizeof(tnl::Vector3), 1, fp);
				fwrite(&param->diff_, sizeof(tnl::Vector3), 1, fp);
				fwrite(&param->ambi_, sizeof(tnl::Vector3), 1, fp);
				fwrite(&param->spec_, sizeof(tnl::Vector3), 1, fp);
				fclose(fp);
			}
		}
	}

	int color = (is_btn_save_forcus_) ? -1 : 0x88888888;
	color = (is_btn_save_select_) ? 0xff00ff00 : color;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(DXE_WINDOW_WIDTH - 330, ty, bx, by, 0, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawStringEx((float)(DXE_WINDOW_WIDTH - 320), (float)ty, -1, "light parameter");

	DrawBox(tx - 5, ty, bx, by, color, false);
	DrawStringEx((float)tx, (float)ty, color, "save to file");

}

//------------------------------------------------------------------------------------------------------------------------------
void SetDefaultLightParameter(const std::string& file_path) {

	tnl::Vector3 dir = { 1.0f, -1.0f, 1.0f };
	tnl::Vector3 diff = { 1.0f, 1.0f, 1.0f };
	tnl::Vector3 ambi = { 0.33f, 0.33f, 0.33f };
	tnl::Vector3 spec = { 1.0f, 1.0f, 1.0f };

	FILE* fp = nullptr;
	fopen_s(&fp, file_path.c_str(), "rb");
	if (fp) {
		fread(&dir, sizeof(tnl::Vector3), 1, fp);
		fread(&diff, sizeof(tnl::Vector3), 1, fp);
		fread(&ambi, sizeof(tnl::Vector3), 1, fp);
		fread(&spec, sizeof(tnl::Vector3), 1, fp);
		fclose(fp);

		SetLightDirection({ dir.x, dir.y, dir.z });
		SetLightDifColor({ diff.x, diff.y, diff.z, 1.0f });
		SetLightAmbColor({ ambi.x, ambi.y, ambi.z, 1.0f });
		SetLightSpcColor({ spec.x, spec.y, spec.z, 1.0f });
	}

}

void GetLookAtCameraFactor(const float axis_x_angle, const float axis_y_angle, const float distance, tnl::Vector3& out_target_to_position) {
	tnl::Quaternion  q = tnl::Quaternion::RotationAxis({ 0, 1, 0 }, axis_y_angle);
	tnl::Vector3 vz = tnl::Vector3::TransformCoord({ 0, 0, 1 }, q);
	q *= tnl::Quaternion::RotationAxis(tnl::Vector3::Cross({ 0, 1, 0 }, vz), axis_x_angle);
	out_target_to_position = tnl::Vector3::TransformCoord({ 0, 0, -distance }, q);
}


void GetFreeLookCameraFactor(const tnl::Vector3& position, const tnl::Vector3& target, float& out_axis_x_angle, float& out_axis_y_angle, float& out_distance) {
	tnl::Vector3 invlook = position - target;
	out_distance = invlook.length();
	tnl::Vector3 vx = tnl::Vector3::Normalize(invlook.xz());
	tnl::Vector3 vy = tnl::Vector3::Normalize(invlook);
	out_axis_y_angle = (invlook.x < 0) ? vx.angle({ 0, 0, -1 }) : -vx.angle({ 0, 0, -1 });
	out_axis_x_angle = (invlook.y < 0) ? -vy.angle(vx) : vy.angle(vx);
}

void GetTransformCameraFactor(const tnl::Vector3& position, const tnl::Vector3& target, const tnl::Vector3& up, tnl::Quaternion& out_rot) {
	out_rot = tnl::Quaternion::LookAt(position, target, up);
}

namespace dxe {

#if 1

	static ComPtr<ID3D11DepthStencilState> g_state_depth_stencil[static_cast<int>(eDepthStenclil::MAX)] ;
	static ComPtr<ID3D11BlendState> g_blend_states[static_cast<int>(eBlendState::MAX)];
	static ComPtr<ID3D11SamplerState> g_sampler_states[static_cast<int>(eSamplerState::MAX)];
	static ComPtr<ID3D11RasterizerState> g_rasterizer_states[static_cast<int>(eRasterizerState::MAX)];

	ID3D11DepthStencilState* GetDepthStencilState(const eDepthStenclil mode) { return g_state_depth_stencil[static_cast<int>(mode)].Get(); }
	ID3D11BlendState* GetBlendState(const eBlendState state) { return g_blend_states[static_cast<int>(state)].Get(); }
	ID3D11SamplerState* GetSamplerState(const eSamplerState state) { return g_sampler_states[static_cast<int>(state)].Get(); }
	ID3D11RasterizerState* GetRasterizerState(const eRasterizerState state) { return g_rasterizer_states[static_cast<int>(state)].Get(); }

	void CreateDepthStencil(const eDepthStenclil e_ds, const D3D11_DEPTH_WRITE_MASK depth_write_mask, bool depth_enable) {
		HRESULT hr = E_FAIL;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		D3D11_DEPTH_STENCIL_DESC dsState;
		::ZeroMemory(&dsState, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dsState.DepthWriteMask = depth_write_mask;
		dsState.DepthFunc = D3D11_COMPARISON_LESS;
		dsState.DepthEnable = depth_enable;
		dsState.StencilEnable = FALSE;
		ID3D11DepthStencilState* dss = nullptr;
		hr = pd3dDevice->CreateDepthStencilState(&dsState, &dss);
		g_state_depth_stencil[static_cast<int>(e_ds)].Attach(dss);
		if (S_OK != hr) {
			tnl::DebugTrace("Error : dxe::DirectXInitialize CreateDepthStencilState \n");
			return;
		}
	}

	void CreateBlendState(const eBlendState state) {
		HRESULT hr = E_FAIL;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();

		//-------------------------------------------------------------------------------------------------
		// ブレンドステートの作成
		D3D11_RENDER_TARGET_BLEND_DESC desc;
		desc.BlendEnable = TRUE;
		switch (state) {
		case eBlendState::NORMAL:
			desc.BlendEnable = FALSE;
			break;
		case eBlendState::ALPHA:
			desc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			break;
		case eBlendState::ADD:
			desc.SrcBlend = D3D11_BLEND_ONE;
			desc.DestBlend = D3D11_BLEND_ONE;
			break;
		case eBlendState::SUB:
			desc.SrcBlend = D3D11_BLEND_ZERO;
			desc.DestBlend = D3D11_BLEND_INV_SRC_COLOR;
			break;
		case eBlendState::MUL:
			desc.SrcBlend = D3D11_BLEND_DEST_COLOR;
			desc.DestBlend = D3D11_BLEND_ZERO;
			break;
		}
		desc.BlendOp = D3D11_BLEND_OP_ADD;
		desc.SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		D3D11_BLEND_DESC blend_desc;
		::ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
		::CopyMemory(&blend_desc.RenderTarget[0], &desc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
		ID3D11BlendState* bs = nullptr;
		hr = pd3dDevice->CreateBlendState(&blend_desc, &bs);
		g_blend_states[static_cast<int>(state)].Attach(bs);

		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : Particle Create Blend State \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}
	}


	void CreateSamplerState(const eSamplerState state) {

		HRESULT hr = E_FAIL;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();

		//-------------------------------------------------------------------------------------------------
		// サンプラーステートの作成
		D3D11_SAMPLER_DESC samplerDesc;
		::ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		switch (state) {
		case eSamplerState::ANISOTROPIC:
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;        
			samplerDesc.MaxAnisotropy = 16;                        // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
			break;
		case eSamplerState::BILINEAR:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.MaxAnisotropy = 1;                        
			break;
		case eSamplerState::NEAREST:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.MaxAnisotropy = 1;
			break;
		}

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;     // 0 ～ 1 の範囲外にある u テクスチャー座標の描画方法
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;     // 0 ～ 1 の範囲外にある v テクスチャー座標の描画方法
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 ～ 1 の範囲外にある w テクスチャー座標の描画方法
		samplerDesc.MipLODBias = 0;                            // 計算されたミップマップ レベルからのバイアス
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // 比較オプション。

		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;

		samplerDesc.MinLOD = 0;                                // アクセス可能なミップマップの下限値
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // アクセス可能なミップマップの上限値
		ID3D11SamplerState* st = nullptr;
		hr = pd3dDevice->CreateSamplerState(&samplerDesc, &st);
		g_sampler_states[static_cast<int>(state)].Attach(st);

		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : Instancing Create Sampler State \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}


	}


	void CreateRasterizerState(const eRasterizerState state) {

		HRESULT hr = E_FAIL;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();

		ID3D11RasterizerState* rs = nullptr;
		D3D11_RASTERIZER_DESC desc = {};

		switch (state) {
		case eRasterizerState::CULL_NONE :
			desc.CullMode = D3D11_CULL_NONE;
			desc.FillMode = D3D11_FILL_SOLID;
			break;
		case eRasterizerState::CULL_FRONT:
			desc.CullMode = D3D11_CULL_FRONT;
			desc.FillMode = D3D11_FILL_SOLID;
			break;
		case eRasterizerState::CULL_BACK:
			desc.CullMode = D3D11_CULL_BACK;
			desc.FillMode = D3D11_FILL_SOLID;
			break;
		case eRasterizerState::WIREFRAME:
			desc.CullMode = D3D11_CULL_NONE;
			desc.FillMode = D3D11_FILL_WIREFRAME;
			break;
		}

		desc.FrontCounterClockwise = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = false;
		hr = pd3dDevice->CreateRasterizerState(&desc, &rs);
		if (S_OK != hr) {
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			tnl::DebugTrace("Error : Particle Create Rasterizer State \n");
			tnl::DebugTrace("-----------------------------------------------------------------\n");
			return;
		}
		g_rasterizer_states[static_cast<int>(state)].Attach(rs);

	}



	//------------------------------------------------------------------------------------------------------------------------------
	float GetDeltaTime() {
		return g_delta_time;
	}


	//------------------------------------------------------------------------------------------------------------------------------
	static ID3D11VertexShader* default_vs = nullptr;
	static ID3D11PixelShader* default_ps = nullptr;
	static ID3D11HullShader* default_hs = nullptr;
	static ID3D11GeometryShader* default_gs = nullptr;
	static ID3D11DomainShader* default_ds = nullptr;
	static UINT32 default_sample_mask = 0;
	static ID3D11BlendState* default_blend_state = nullptr;
	static ID3D11RasterizerState* default_rasterize_state = nullptr;
	static ID3D11SamplerState* default_sampler_state = nullptr;
	static ID3D11ShaderResourceView* default_pixel_shader_resouce_view = nullptr;
	static ID3D11DepthStencilState* default_depth_stencil_state_ = nullptr;
	static UINT default_depth_stencil_ref_ = 0;

	void DirectXRenderBegin() {
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		pImmediateContext->OMGetDepthStencilState(&default_depth_stencil_state_, &default_depth_stencil_ref_);

		// DX ライブラリのシェーダバックアップ
		pImmediateContext->VSGetShader(&default_vs, NULL, 0);
		pImmediateContext->PSGetShader(&default_ps, NULL, 0);
		pImmediateContext->HSGetShader(&default_hs, NULL, 0);
		pImmediateContext->GSGetShader(&default_gs, NULL, 0);
		pImmediateContext->DSGetShader(&default_ds, NULL, 0);

		// ブレンドステートバックアップ
		pImmediateContext->OMGetBlendState(&default_blend_state, 0, &default_sample_mask);

		// ラスタライザステートバックアップ
		pImmediateContext->RSGetState(&default_rasterize_state);

		// サンプラステートバックアップ
		pImmediateContext->PSGetSamplers(0, 1, &default_sampler_state);

		// ピクセルシェーダリソースビュー
		pImmediateContext->PSGetShaderResources(0, 1, &default_pixel_shader_resouce_view);
	}

	//------------------------------------------------------------------------------------------------------------------------------
	void DirectXRenderEnd() {
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		pImmediateContext->OMSetDepthStencilState(default_depth_stencil_state_, default_depth_stencil_ref_);

		// DX ライブラリへシェーダバックアップを復帰
		pImmediateContext->VSSetShader(default_vs, NULL, 0);
		pImmediateContext->PSSetShader(default_ps, NULL, 0);
		pImmediateContext->HSSetShader(default_hs, NULL, 0);
		pImmediateContext->GSSetShader(default_gs, NULL, 0);
		pImmediateContext->DSSetShader(default_ds, NULL, 0);

		pImmediateContext->OMSetBlendState(default_blend_state, 0, default_sample_mask);
		pImmediateContext->RSSetState(default_rasterize_state);
		pImmediateContext->PSSetSamplers(0, 1, &default_sampler_state);
		pImmediateContext->PSSetShaderResources(0, 1, &default_pixel_shader_resouce_view);

		DxLib::RefreshDxLibDirect3DSetting();
	}

	//------------------------------------------------------------------------------------------------------------------------------
	void DirectXInitialize() {

		HRESULT hr = E_FAIL;
		ID3D11Device* pd3dDevice = (ID3D11Device*)DxLib::GetUseDirect3D11Device();
		ID3D11DeviceContext* pImmediateContext = (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext();

		//-------------------------------------------------------------------------------------------------
		// 深度ステンシルステート作成 ( ステンシル設定なし )
		CreateDepthStencil(eDepthStenclil::DEPTH_W_ON_T_ON, D3D11_DEPTH_WRITE_MASK_ALL, TRUE);
		CreateDepthStencil(eDepthStenclil::DEPTH_W_ON_T_OFF, D3D11_DEPTH_WRITE_MASK_ALL, FALSE);
		CreateDepthStencil(eDepthStenclil::DEPTH_W_OFF_T_ON, D3D11_DEPTH_WRITE_MASK_ZERO, TRUE);
		CreateDepthStencil(eDepthStenclil::DEPTH_W_OFF_T_OFF, D3D11_DEPTH_WRITE_MASK_ZERO, FALSE);

		//-------------------------------------------------------------------------------------------------
		// ブレンドステート作成 
		CreateBlendState(eBlendState::NORMAL);
		CreateBlendState(eBlendState::ALPHA);
		CreateBlendState(eBlendState::ADD);
		CreateBlendState(eBlendState::SUB);
		CreateBlendState(eBlendState::MUL);

		//-------------------------------------------------------------------------------------------------
		// サンプラテート作成 
		CreateSamplerState(eSamplerState::ANISOTROPIC);
		CreateSamplerState(eSamplerState::BILINEAR);
		CreateSamplerState(eSamplerState::NEAREST);

		//-------------------------------------------------------------------------------------------------
		// ラスタライザステート作成
		CreateRasterizerState(eRasterizerState::CULL_NONE);
		CreateRasterizerState(eRasterizerState::CULL_FRONT);
		CreateRasterizerState(eRasterizerState::CULL_BACK);
		CreateRasterizerState(eRasterizerState::WIREFRAME);
	}

	//------------------------------------------------------------------------------------------------------------------------------
	void DirectXRelease() {

		for (int i = 0; i < static_cast<int>(eDepthStenclil::MAX); ++i) {
			g_state_depth_stencil[i].Reset();
		}
		for (int i = 0; i < static_cast<int>(eBlendState::MAX); ++i) {
			g_blend_states[i].Reset();
		}
		for (int i = 0; i < static_cast<int>(eSamplerState::MAX); ++i) {
			g_sampler_states[i].Reset();
		}
		for (int i = 0; i < static_cast<int>(eRasterizerState::MAX); ++i) {
			g_rasterizer_states[i].Reset();
		}
	}
#endif
}
