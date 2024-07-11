#include "util/math.hlsl"
#define TNL_DEP_FLOAT3(n) n, n, n

// 定数バッファ０
cbuffer cbBuffer0 : register( b0 )
{
    float4x4 c_view			: packoffset( c0 );
    float4x4 c_proj			: packoffset( c4 );
    float4x4 c_world		: packoffset( c8 );
    float4	c_sky_color		: packoffset( c12 );
    float4	c_base_color    : packoffset( c13 );
    float4	c_shallow_color	: packoffset( c14 );
    float4	c_specular		: packoffset( c15 );
    float4  c_wave_directionA    : packoffset( c16 );
    float4  c_wave_directionB    : packoffset( c17 );
    float4  c_wave_directionC    : packoffset( c18 );
    float4  c_wave_directionD    : packoffset( c19 );
    float3 c_light_direction    : packoffset( c20 ); 
    float  c_max_height         : packoffset( c20.w );
    float3 c_cam_pos            : packoffset( c21 );
    float  c_fresnel_base_alpha : packoffset( c21.w );
    float3 c_fog_color          : packoffset( c22 );
    float  c_base_col_str       : packoffset( c22.w );
    float  c_shallow_col_str    : packoffset( c23.x );
    float  c_darkness_contrast  : packoffset( c23.y );
    float  c_spec_power         : packoffset( c23.z);
    float  c_shininess          : packoffset( c23.w );
    float  c_fresnel_factor     : packoffset( c24.x );
    float  c_noise_strength     : packoffset( c24.y );
    float  c_base_amp_factor    : packoffset( c24.z );
    float  c_cube_amp_factor    : packoffset( c24.w );
    float  c_frequency_factor   : packoffset( c25.x );
    float  c_steepness_factor   : packoffset( c25.y );
    float  c_progress_time      : packoffset( c25.z );
    float  c_size_width         : packoffset( c25.w );
    float  c_size_depth         : packoffset( c26.x );
    float  c_pattern_col_factor                 : packoffset( c26.y );
    float  c_decoration_col_factor              : packoffset( c26.z );
    float  c_landing_projection_col_factor      : packoffset( c26.w );
    float  c_landing_projection_col_pow         : packoffset( c27.x );
    float  c_fog_start                          : packoffset( c27.y );
    float  c_fog_end                            : packoffset( c27.z );
    int    c_use_pattern_tex                    : packoffset( c27.w );
    int    c_use_decoration_tex                 : packoffset( c28.x );
    int    c_use_landing_projection_tex         : packoffset( c28.y );
    int    c_use_landing_cut_out_tex            : packoffset( c28.z );
    int    c_use_cube_map_tex                   : packoffset( c28.w );
    int    c_light_enable		                : packoffset( c29.x );
    int    c_wave_direction                     : packoffset( c29.y );
    int    c_gerstner_sampleing                 : packoffset( c29.z );
    int    c_far_fade                           : packoffset( c29.w );

};

// 定数バッファ１
cbuffer cbBuffer1 : register( b1 )
{
  float g_TessFactor       : packoffset( c0.x );   // パッチのエッジのテッセレーション係数
  float g_InsideTessFactor : packoffset( c0.y );   // パッチ内部のテッセレーション係数
};

// テクスチャー
Texture2D tex_decoration            : register( t0 ) ;
Texture2D tex_pattern               : register( t1 );
Texture2D tex_landing_projection    : register( t2 );
Texture2D tex_landing_cut_out       : register( t3 );
TextureCube tex_cube_map            : register( t4 );

// サンプラーステート
SamplerState  g_Sampler;
SamplerState cubeMapSampler {
    Filter = MIN_MAG_MIP_LINEAR; // テクスチャのフィルタリング方法
    AddressU = Mirror; // X軸方向のテクスチャアドレッシング方法
    AddressV = Mirror; // Y軸方向のテクスチャアドレッシング方法
    AddressW = Mirror; // Z軸方向のテクスチャアドレッシング方法
};

// 頂点シェーダーの入力パラメータ
struct VS_IN
{
  float4 pos   : POSITION;   // 頂点座標
  float2 texel : TEXCOORD0;  // テクセル
};

// 頂点シェーダーの出力パラメータ
struct VS_OUT
{
  float4 pos    : POSITION;
  float2 texel  : TEXCOORD0;
};

// ハルシェーダーのパッチ定数フェーズ用の出力パラメータ
struct CONSTANT_HS_OUT
{
  float Edges[4] : SV_TessFactor;        // パッチのエッジのテッセレーション係数
  float Inside[2] : SV_InsideTessFactor; // パッチ内部のテッセレーション係数
};

// ハルシェーダーのコントロール ポイント フェーズ用の出力パラメータ
struct HS_OUT
{
  float4 pos   : POSITION;
  float2 texel : TEXCOORD0;
};

// ドメインシェーダーの出力パラメータ
struct DS_OUT
{
    float4 pos   : SV_POSITION;
    float2 texel1 : TEXCOORD0;
    float2 texel2 : TEXCOORD1;
    float2 texel3 : TEXCOORD2;
    float4 fog_normal : TEXCOORD3;

    float4  view_pos : VIEW_POS;
    float4  Diffuse     : COLOR0;
    float4  Specular    : COLOR1;
};


//------------------------------------------------------------------------------------------------------------------------------------
static const int GELSTNER_SAMPLING_D4 = 0 ;
static const int GELSTNER_SAMPLING_D8 = 1 ;

static const int FAR_FADE_NONE = 0 ;
static const int FAR_FADE_FOG = 1 ;
static const int FAR_FADE_ALPHA = 2 ;

//static const float fresnel_factor = 0.01 ;
//static const float spec_power = 50 ;
//static const float _NoiseStrength = 3.5 ;                               // 粒度
static const float _NoiseSizeLerp = 0.26 ;
 
static const float4 _Amplitude = float4(0.78, 0.81, 0.6, 0.27) ;        // 波の荒れ具合
static const float4 _Frequency = float4(0.16, 0.18, 0.21, 0.27) ;       // 波頭の間隔
static const float4 _Steepness = float4(1.70, 1.60, 1.20, 1.80) ;       // 
static const float4 _Noise = float4(0.39, 0.31, 0.27, 0.57)  ;          // ノイズ

static const float4 _Speed = float4(24, 40, 48, 60) ;
 
static const float4 _Amplitude2 = float4(0.17, 0.12, 0.21, 0.06) ;
static const float4 _Frequency2 = float4(0.7, 0.84, 0.54, 0.80) ;
static const float4 _Steepness2 = float4(1.56, 2.18, 2.80, 1.90) ;
static const float4 _Speed2 = float4(32, 40, 48, 60) ;
static const float4 _Noise2 = float4(0.33, 0.81, 0.39, 0.45) ;
static const float _WaveSpeed = 0.05 ; 

//static const float4 _SkyColor = float4(0.1, 0.1, 0.1, 1);             // 空の色
//static const float4 _SeaBaseColor = float4(0.1, 0.1, 0.7, 1);         // 水のベースカラー
//static const float4 _SeaShallowColor = float4(0.1, 0.1, 0.1, 1);      // 深みの変化色
//static const float _BaseColorStrength = 0.5 ;                       // ベースカラーの強さ
//static const float _ShallowColorStrength = 1.36 ;                   // 深みの変化色の強さ
//static const float _ColorHightOffset = 0.15 ;                       // 色のコントラスト 
//static const float _Shininess = 0.27 ;                              // シャイネス

static const float amp[8] = {_Amplitude.x, _Amplitude.y, _Amplitude.z, _Amplitude.w, _Amplitude2.x, _Amplitude2.y, _Amplitude2.z, _Amplitude2.w};
static const float freq[8] = {_Frequency.x, _Frequency.y, _Frequency.z, _Frequency.w, _Frequency2.x, _Frequency2.y, _Frequency2.z, _Frequency2.w};
static const float steep[8] = {_Steepness.x, _Steepness.y, _Steepness.z, _Steepness.w, _Steepness2.x, _Steepness2.y, _Steepness2.z, _Steepness2.w};
static const float speed[8] = {_Speed.x, _Speed.y, _Speed.z, _Speed.w, _Speed2.x, _Speed2.y, _Speed2.z, _Speed2.w};
static const float noise_size[8] = {_Noise.x, _Noise.y, _Noise.z, _Noise.w, _Noise2.x, _Noise2.y, _Noise2.z, _Noise2.w};

float2 rand2d(float2 st, int seed)
{
	float2 s = float2(dot(st, float2(127.1, 311.7)) + seed, dot(st, float2(269.5, 183.3)) + seed);
	return -1.0 + 2.0 * frac(sin(s) * 43758.5453123);
}
 
float noise2(float2 st, int seed)
{
	float2 p = floor(st);
	float2 f = frac(st);
 
	float w00 = dot(rand2d(p, seed), f);
	float w10 = dot(rand2d(p + float2(1.0, 0.0), seed), f - float2(1.0, 0.0));
	float w01 = dot(rand2d(p + float2(0.0, 1.0), seed), f - float2(0.0, 1.0));
	float w11 = dot(rand2d(p + float2(1.0, 1.0), seed), f - float2(1.0, 1.0));
	
	float2 u = f * f * (3.0 - 2.0 * f);
 
	return lerp(lerp(w00, w10, u.x), lerp(w01, w11, u.x), u.y);
}
 
float3 rand3d(float3 p, int seed)
{
	float3 s = float3(dot(p, float3(127.1, 311.7, 74.7)) + seed,
					  dot(p, float3(269.5, 183.3, 246.1)) + seed,
					  dot(p, float3(113.5, 271.9, 124.6)) + seed);
	return -1.0 + 2.0 * frac(sin(s) * 43758.5453123);
}
 
float noise3(float3 st, int seed)
{
	float3 p = floor(st);
	float3 f = frac(st);
 
	float w000 = dot(rand3d(p, seed), f);
	float w100 = dot(rand3d(p + float3(1, 0, 0), seed), f - float3(1, 0, 0));
	float w010 = dot(rand3d(p + float3(0, 1, 0), seed), f - float3(0, 1, 0));
	float w110 = dot(rand3d(p + float3(1, 1, 0), seed), f - float3(1, 1, 0));
	float w001 = dot(rand3d(p + float3(0, 0, 1), seed), f - float3(0, 0, 1));
	float w101 = dot(rand3d(p + float3(1, 0, 1), seed), f - float3(1, 0, 1));
	float w011 = dot(rand3d(p + float3(0, 1, 1), seed), f - float3(0, 1, 1));
	float w111 = dot(rand3d(p + float3(1, 1, 1), seed), f - float3(1, 1, 1));
	
	float3 u = f * f * (3.0 - 2.0 * f);
 
	float r1 = lerp(lerp(w000, w100, u.x), lerp(w010, w110, u.x), u.y);
	float r2 = lerp(lerp(w001, w101, u.x), lerp(w011, w111, u.x), u.y);
 
	return lerp(r1, r2, u.z);
}
 
float fbm(float2 st, int seed){
	float val = 0.0;
	float a = 0.5;
 
	for(int i = 0; i < 6; i++){
		val += a * noise2(st, seed);
		st *= 2.0;
		a *= 0.5;
	}
	return val;
}


float3 GerstnerWave(float2 amp, float freq, float steep, float speed, float noise, float2 dir, float2 v, float time, int seed)
{
//    amp *= c_amplitude_factor ; 
    freq *= c_frequency_factor ;
    steep *= c_steepness_factor ;

	float3 p;
	float2 d = normalize(dir.xy);
	float q = steep;

	seed *= 3;
	v +=  noise2(v * noise + time, seed) * c_noise_strength ;
    float f = dot(d, v) * freq + time * speed;
    p.xz = q * amp * d.xy * cos(f) ;
	p.y = amp * sin(f) ;
 
    return p;
}
 
float3 GerstnerWave_Cross(float2 amp, float freq, float steep, float speed, float noise, float2 dir, float2 v, float time, int seed)
{
//    amp *= c_amplitude_factor ;
    freq *= c_frequency_factor ;
    steep *= c_steepness_factor ;

	float3 p = float3(0, 0, 0);
	float2 d = normalize(dir.xy);
	float q = steep;
 
	float noise_strength = c_noise_strength;
	seed *= 3;
 
	float3 p1 = float3(0, 0, 0);
	float3 p2 = float3(0, 0, 0);
	float2 d1 = normalize(dir.xy);
	float2 d2 = float2(-d.y, d.x);
 
	float2 v1 = v + noise2(v * noise + time * d * 10.0, seed) * noise_strength;
	float2 v2 = v + noise2(v * noise + time * d * 10.0, seed + 12) * noise_strength;
	float2 f1 = dot(d1, v1) * freq + time * speed;
	float2 f2 = dot(d2, v2) * freq + time * speed;
	p1.xz = q * amp * d1.xy * cos(f1);
	p1.y = amp * sin(f1);
	p2.xz = q * amp * d2.xy * cos(f2);
	p2.y = amp * sin(f2); 
	p = lerp(p1, p2, noise2(v * _NoiseSizeLerp + time, seed) * 0.5 + 0.5);

	//p = lerp(p1, p2, 0.5);
 
	return p;
}

// Parallax-Corrected Cubemap
float3 calcParallaxCorrectedCubemapReflect(float3 worldPos, float3 worldReflect,
                                           float3 cubemapPos, float3 boxMin, float3 boxMax)
{
    // AABB と 反射ベクトル の交差点 worldIntersectPos を計算します
    float3 firstPlaneIntersect  = (boxMax - worldPos) / worldReflect;
    float3 secondPlaneIntersect = (boxMin - worldPos) / worldReflect;
    float3 furthestPlane        = max(firstPlaneIntersect, secondPlaneIntersect);
    float dist                  = min(min(furthestPlane.x, furthestPlane.y),
                                          furthestPlane.z);
    float3 worldIntersectPos    = worldPos + worldReflect * dist;

    // Cubemapから交差点に向かうベクトルが ParallaxCorrected された反射ベクトルになります
    return worldIntersectPos - cubemapPos;
}

float3 GetSkyColor(float3 dir, float3 c){
    dir.y *= c_darkness_contrast ;
	dir.y = max(0.0, dir.y);
	float et = 1.0 - dir.y;
	return (1.0 - c) * et + c;
}
			

float4 OceanColor(float3 world_pos, float wave_height, float3 normal, float3 normal2, float4 proj_coord){

	//lighting
    //float3 lightDir = normalize(normalize(-world_pos) - c_light_direction);
    float3 lightDir = float3( c_light_direction.x, -c_light_direction.y, c_light_direction.z ) ;
    float3 viewDir = normalize(world_pos - c_cam_pos);    
	float3 halfDir = normalize(lightDir + viewDir);
	
	//fresnel
    float r = 0.2 ;
	float facing = saturate(1.0 - dot(normal, viewDir));
	//★float fresnel = r + (1.0 - r) * pow(facing, 5.0);
    float fresnel = r + (1.0 - r) * pow(facing, 2.0) * c_fresnel_factor ;

	//★float3 reflectDir = reflect(-viewDir, normal);
    float3 reflectDir = reflect(viewDir, normal);
    float3 reflectDir2 = reflect(viewDir, normal2);

    float w = 525 ;
    float h = 750 ;
    float d = 525 ;
    float3 MinBox = float3(-w, 0, -d);
    float3 MaxBox = float3( w,  h,  d);

    // 反射ベクトルを求める
    //reflectDir = normalize( calcParallaxCorrectedCubemapReflect(world_pos, reflectDir, float3(0, 1, 0), MinBox, MaxBox) );
    reflectDir = normalize(reflectDir) ;
	
	//★float diff = saturate(dot(normal, lightDir)) * c_diffuse ;
	float diff = saturate(dot(normal, lightDir)) ;
	//float spec = pow(max(0.0, dot(normal, halfDir)), _Shininess * 128.0) * _LightColor0;	//Blinn-Phong
	
	//https://www.gamedev.net/articles/programming/graphics/rendering-water-as-a-post-process-effect-r2642/
	float dotSpec = saturate(dot(reflectDir, lightDir) * 0.5 + 0.5);
	float spec = saturate(lightDir.y) * pow(dotSpec, c_spec_power ) * (c_shininess * 1.8 + 0.2); 
    spec += spec * 25.0 * saturate(c_shininess - 0.05) ;    
	
	//reflection
	float3 sea_reflect_color = GetSkyColor(reflectDir, c_sky_color) ;
    if(c_use_cube_map_tex){
        float3 ref_col1 = tex_cube_map.Sample(cubeMapSampler, reflectDir) ;
        float3 ref_col2 = tex_cube_map.Sample(cubeMapSampler, reflectDir2) ;
        //sea_reflect_color = ( sea_ref1 + sea_ref2 ) * 0.5f ;
        //sea_reflect_color = sea_ref1 ;
        sea_reflect_color = GetSkyColor(reflectDir, ( ref_col1 + ref_col2 ) * 0.5f) ;
    }

	float3 sea_base_color = c_base_color * diff * c_base_col_str + lerp(c_base_color, c_shallow_color * c_shallow_col_str, diff);
	float3 water_color = lerp(sea_base_color, sea_reflect_color, fresnel);

    float col_height_offset = 0.15f ;
    float3 sea_color = water_color + c_shallow_color * (wave_height * 0.5 + 0.2) * col_height_offset;

    //float alpha = c_fresnel_base_alpha + ( sea_reflect_color.x + sea_reflect_color.y + sea_reflect_color.z ) / 3.0f ;
    //alpha = clamp(alpha, c_fresnel_base_alpha, 1.0f) ;

    float alpha = c_fresnel_base_alpha + dot(reflectDir, viewDir) ;
    alpha = clamp(alpha, c_fresnel_base_alpha, 1.0f) ;

	return float4( sea_color + ( float3(TNL_DEP_FLOAT3(spec)) * c_specular ), alpha ) ;
}



//------------------------------------------------------------------------------------------------------------------------------------



// *****************************************************************************************
// 頂点シェーダー
// *****************************************************************************************
VS_OUT VS_Main( VS_IN In )
{
   VS_OUT Out;

  // 頂点シェーダーではそのまま渡す
  Out.pos = In.pos;
  Out.texel = In.texel;
  return Out;
}

// *****************************************************************************************
// ハルシェーダーのパッチ定数フェーズ
// *****************************************************************************************
CONSTANT_HS_OUT ConstantsHS_Main( InputPatch<VS_OUT, 4> ip, uint PatchID : SV_PrimitiveID )
{
  CONSTANT_HS_OUT Out;

  // 定数バッファの値をそのまま渡す
  Out.Edges[0] = Out.Edges[1] = Out.Edges[2] = Out.Edges[3] = g_TessFactor;  // パッチのエッジのテッセレーション係数
  Out.Inside[0] = g_InsideTessFactor;  // パッチ内部の横方法のテッセレーション係数
  Out.Inside[1] = g_InsideTessFactor;  // パッチ内部の縦方法のテッセレーション係数
 
  return Out;
}

// *****************************************************************************************
// ハルシェーダーのコントロール ポイント フェーズ
// *****************************************************************************************
[domain("quad")]                             // テッセレートするメッシュの形状を指定する
[partitioning("integer")]                    // パッチの分割に使用するアルゴリズムを指定する
[outputtopology("triangle_ccw")]             // メッシュの出力方法を指定する
[outputcontrolpoints(4)]                     // ハルシェーダーのコントロール ポイント フェーズがコールされる回数
[patchconstantfunc("ConstantsHS_Main")]      // 対応するハルシェーダーのパッチ定数フェーズのメイン関数
HS_OUT HS_Main( InputPatch<VS_OUT, 4> In, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID )
{
  HS_OUT Out;

  // そのまま渡す
  Out.pos = In[i].pos;
  Out.texel = In[i].texel;
  return Out;
}

static const int wave_number = 8;
// *****************************************************************************************
// ドメインシェーダー
// *****************************************************************************************
[domain("quad")]
DS_OUT DS_Main( CONSTANT_HS_OUT In, float2 uv : SV_DomainLocation, const OutputPatch<HS_OUT, 4> patch )
{
    DS_OUT Out;

    float2 dir[8] = {c_wave_directionA.xy, c_wave_directionA.zw, c_wave_directionB.xy, c_wave_directionB.zw, c_wave_directionC.xy, c_wave_directionC.zw, c_wave_directionD.xy, c_wave_directionD.zw};
 

    // テクセル
    float2 t1 = lerp( patch[1].texel, patch[0].texel, uv.x );
    float2 t2 = lerp( patch[3].texel, patch[2].texel, uv.x );
    float2 texel = lerp( t1, t2, uv.y );

    // 頂点座標
    float3 p1 = lerp( patch[1].pos.xyz, patch[0].pos.xyz, uv.x );
    float3 p2 = lerp( patch[3].pos.xyz, patch[2].pos.xyz, uv.x );
    float4 pos = float4( lerp( p1, p2, uv.y ), 1.0f ) ;

    float time = c_progress_time * _WaveSpeed;

    float3 p = 0.0;
    int count = 4;
    for(int i = 0; i < count; i++){
        p += GerstnerWave(amp[i], freq[i], steep[i], speed[i], noise_size[i], dir[i], pos.xz, time, i);
    }
    for(int j = wave_number - count; j < wave_number; j++){
        p += GerstnerWave_Cross(amp[j], freq[j], steep[j], speed[j], noise_size[j], dir[j], pos.xz, time, j);
    }
    pos += float4(p.x, p.y, p.z, 0) * (c_max_height / wave_number);

    //ワールド変換
    pos = mul(pos, c_world);

    Out.view_pos = pos ;
    Out.texel1.x = texel.x + ( sin(time) / c_size_width ) * 100 ;
    Out.texel1.y = texel.y + ( cos(time) / c_size_depth ) * 100 ;

    Out.texel2.x = texel.x - ( sin(PI * 0.5 + time * 1.25) / c_size_width ) * 100;
    Out.texel2.y = texel.y - ( cos(PI * 0.5 + time * 1.35) / c_size_depth ) * 100;    

    Out.texel3.x = texel.x ;
    Out.texel3.y = texel.y ;        

    //ビュー変換
    pos = mul(pos, c_view);
    Out.fog_normal.xyz = pos ;

    //射影変換
    pos = mul(pos, c_proj);
    Out.pos = pos ;

    float v = c_fog_end - c_fog_start ;
    float z = Out.fog_normal.z - c_fog_start ;
    Out.fog_normal.w = z / v ;
    Out.fog_normal.w = clamp(Out.fog_normal.w, 0, 1) ;


    return Out;
}


// *****************************************************************************************
// ピクセルシェーダ
// *****************************************************************************************
float4 PS_Main( DS_OUT In ) : SV_TARGET
{

    float2 dir[8] = {c_wave_directionA.xy, c_wave_directionA.zw, c_wave_directionB.xy, c_wave_directionB.zw, c_wave_directionC.xy, c_wave_directionC.zw, c_wave_directionD.xy, c_wave_directionD.zw};

    float4 col_pattern1 = float4(1,1,1,1) ;
    float4 col_pattern2 = float4(1,1,1,1) ;
    float4 col_pattern = float4(1,1,1,1) * c_pattern_col_factor ;
    float4 col_decoration = float4(0,0,0,0) ;
    float4 col_landing_projection = float4(0, 0, 0, 0) ;
    float4 col_landing_cut_out = float4(1, 1, 1, 1) ;

    if(c_use_pattern_tex){
        col_pattern1 = tex_pattern.Sample( g_Sampler, In.texel1 ) ;
        col_pattern2 = tex_pattern.Sample( g_Sampler, In.texel2 ) ;
        col_pattern = ( col_pattern1 + col_pattern2 ) * 0.5f ;    
        col_pattern *= c_pattern_col_factor ;
        col_pattern.x = clamp(col_pattern.x, 0, 1) ;
        col_pattern.y = clamp(col_pattern.y, 0, 1) ;
        col_pattern.z = clamp(col_pattern.z, 0, 1) ;
        col_pattern.w = clamp(col_pattern.w, 0, 1) ;
    }
    if(c_use_decoration_tex){
        col_decoration = tex_decoration.Sample( g_Sampler, In.texel3 ) ;
        col_decoration *= c_decoration_col_factor ;
    }
    if(c_use_landing_projection_tex){
        col_landing_projection = tex_landing_projection.Sample( g_Sampler, In.texel3 ) ;
        col_landing_projection *= c_landing_projection_col_factor ;
        col_landing_projection = pow(col_landing_projection, c_landing_projection_col_pow) ;
    }
    if(c_use_landing_cut_out_tex){
        col_landing_cut_out = tex_landing_cut_out.Sample( g_Sampler, In.texel3 ) ;        
        col_landing_cut_out.a = 1.0f - col_landing_cut_out.r ;
    }

    //CalcNormal
    float3 worldPos = In.view_pos ;
    float time = c_progress_time * _WaveSpeed ;
    //time = 1.55;

    float3 p = 0.0;
    float3 pb = float3(0.05, 0.0, 0.0);
    float3 pt =float3(0.0, 0.0, 0.05);
    float3 v_bi = worldPos.xyz + float3(0.05, 0.0, 0.0);
    float3 v_tan = worldPos.xyz + float3(0.0, 0.0, 0.05);

    float3 p2 = 0.0;
    float3 pb2 = float3(0.05, 0.0, 0.0);
    float3 pt2 =float3(0.0, 0.0, 0.05);

    float3 normal1 ;
    float3 normal2 ;
    
    int count = 4;

    float amplitude = 0 ;
    for(int m = 0; m < count; m++){
        amplitude = amp[m] * c_base_amp_factor ;
        p += GerstnerWave(amplitude, freq[m], steep[m], speed[m], noise_size[m], dir[m], worldPos.xz, time, m);
        pb += GerstnerWave(amplitude, freq[m], steep[m], speed[m], noise_size[m], dir[m], v_bi.xz, time, m);
        pt += GerstnerWave(amplitude, freq[m], steep[m], speed[m], noise_size[m], dir[m], v_tan.xz, time, m);
    }

    if( c_gerstner_sampleing == GELSTNER_SAMPLING_D8 ){
        for(int n = wave_number - count; n < wave_number; n++){
            amplitude = amp[m] * c_base_amp_factor ;
            p += GerstnerWave_Cross(amplitude, freq[n], steep[n], speed[n], noise_size[n], dir[n], worldPos.xz, time, n) ;
            pb += GerstnerWave_Cross(amplitude, freq[n], steep[n], speed[n], noise_size[n], dir[n], v_bi.xz, time, n) ;
            pt += GerstnerWave_Cross(amplitude, freq[n], steep[n], speed[n], noise_size[n], dir[n], v_tan.xz, time, n) ;
        } 
    }

    for(int m = 0; m < count; m++){
        amplitude = amp[m] * c_cube_amp_factor ;
        p2 += GerstnerWave(amplitude, freq[m], steep[m], speed[m], noise_size[m], dir[m], worldPos.xz, time, m);
        pb2 += GerstnerWave(amplitude, freq[m], steep[m], speed[m], noise_size[m], dir[m], v_bi.xz, time, m);
        pt2 += GerstnerWave(amplitude, freq[m], steep[m], speed[m], noise_size[m], dir[m], v_tan.xz, time, m);
    }
    normal1 = normalize(cross(pt - p, pb - p));
    normal2 = normalize(cross(pt2 - p2, pb2 - p2));        
        

    worldPos += p;
    float wave_height = worldPos.y - In.view_pos.y;
    float4 col_ochean = OceanColor(worldPos, wave_height, normal1, normal2, In.pos) ;
    float4 col_result = col_ochean * float4( col_pattern.xyz, 1.0f ) + float4(col_decoration.xyz, 0) + float4(col_landing_projection.x, col_landing_projection.x, col_landing_projection.x, 0) ;

    if(c_use_landing_cut_out_tex){
        col_result.a *= col_landing_cut_out.a ;
    }
    col_result.a *= ( 1.0f - col_landing_projection.y * 2.0f ) ;


    if(c_far_fade == 1){
        col_result.rgb = lerp( col_result.rgb, c_fog_color, saturate( In.fog_normal.w ) );    
    }    
    if(c_far_fade == 2){
        col_result.a *= ( 1.0f - In.fog_normal.w ) ;
    }

    return col_result;

}

