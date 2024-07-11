#include <string>
#include "dxlib_ext.h"
#include "../library/tnl_rect.h"

namespace dxe {

	tnl::Rect WINDOW_RECT({ DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2 }, { DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT }) ;

	std::string g_x_file_hedder_str = "\
xof 0302txt 0064\
\n	template Header{\
\n	 <3D82AB43-62DA-11cf-AB39-0020AF71E433>\
\n	 WORD major;\
\n	 WORD minor;\
\n	 DWORD flags;\
\n }\
\n template Vector{\
\n	 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>\
\n		FLOAT x;\
\n		FLOAT y;\
\n		FLOAT z;\
\n }\
\n template Coords2d{\
\n		<F6F23F44-7686-11cf-8F52-0040333594A3>\
\n		FLOAT u;\
\n		FLOAT v;\
\n }\
\n template Matrix4x4{\
\n		<F6F23F45-7686-11cf-8F52-0040333594A3>\
\n		array FLOAT matrix[16];\
\n }\
\n template ColorRGBA{\
\n		<35FF44E0-6C7C-11cf-8F52-0040333594A3>\
\n		FLOAT red;\
\n		FLOAT green;\
\n		FLOAT blue;\
\n		FLOAT alpha;\
\n }\
\n template ColorRGB{\
\n		<D3E16E81-7835-11cf-8F52-0040333594A3>\
\n		FLOAT red;\
\n		FLOAT green;\
\n		FLOAT blue;\
\n }\
\n template IndexedColor{\
\n		<1630B820-7842-11cf-8F52-0040333594A3>\
\n		DWORD index;\
\n		ColorRGBA indexColor;\
\n }\
\n template Boolean{\
\n		<4885AE61-78E8-11cf-8F52-0040333594A3>\
\n		WORD truefalse;\
\n }\
\n template Boolean2d{\
\n		<4885AE63-78E8-11cf-8F52-0040333594A3>\
\n		Boolean u;\
\n		Boolean v;\
\n }\
\n template MaterialWrap{\
\n		<4885AE60-78E8-11cf-8F52-0040333594A3>\
\n		Boolean u;\
\n		Boolean v;\
\n }\
\n template TextureFilename{\
\n		<A42790E1-7810-11cf-8F52-0040333594A3>\
\n		STRING filename;\
\n }\
\n template Material{\
\n		<3D82AB4D-62DA-11cf-AB39-0020AF71E433>\
\n		ColorRGBA faceColor;\
\n		FLOAT power;\
\n		ColorRGB specularColor;\
\n		ColorRGB emissiveColor;\
\n		[...]\
\n }\
\n template MeshFace{\
\n		<3D82AB5F-62DA-11cf-AB39-0020AF71E433>\
\n		DWORD nFaceVertexIndices;\
\n		array DWORD faceVertexIndices[nFaceVertexIndices];\
\n }\
\n template MeshFaceWraps{\
\n		<4885AE62-78E8-11cf-8F52-0040333594A3>\
\n		DWORD nFaceWrapValues;\
\n		Boolean2d faceWrapValues;\
\n }\
\n template MeshTextureCoords{\
\n		<F6F23F40-7686-11cf-8F52-0040333594A3>\
\n		DWORD nTextureCoords;\
\n		array Coords2d textureCoords[nTextureCoords];\
\n }\
\n template MeshMaterialList{\
\n		<F6F23F42-7686-11cf-8F52-0040333594A3>\
\n		DWORD nMaterials;\
\n		DWORD nFaceIndexes;\
\n		array DWORD faceIndexes[nFaceIndexes];\
\n		[Material]\
\n }\
\n template MeshNormals{\
\n		<F6F23F43-7686-11cf-8F52-0040333594A3>\
\n		DWORD nNormals;\
\n		array Vector normals[nNormals];\
\n		DWORD nFaceNormals;\
\n		array MeshFace faceNormals[nFaceNormals];\
\n }\
\n template MeshVertexColors{\
\n		<1630B821-7842-11cf-8F52-0040333594A3>\
\n		DWORD nVertexColors;\
\n		array IndexedColor vertexColors[nVertexColors];\
\n }\
\n template Mesh{\
\n		<3D82AB44-62DA-11cf-AB39-0020AF71E433>\
\n		DWORD nVertices;\
\n		array Vector vertices[nVertices];\
\n		DWORD nFaces;\
\n		array MeshFace faces[nFaces];\
\n		[...]\
\n }\
\n Header{\
\n	1;\
\n	0;\
\n	1;\
\n }";


}