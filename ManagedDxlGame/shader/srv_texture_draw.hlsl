// 定数バッファ０
cbuffer cbBuffer0 : register( b0 )
{
    float4x4 c_view			: packoffset( c0 );
    float4x4 c_proj			: packoffset( c4 );
    float4x4 c_world		: packoffset( c8 );
} ;

// テクスチャー
Texture2D g_texture : register( t0 ) ;
SamplerState  g_sampler : register( s0 ) ;

// 頂点シェーダーの入力パラメータ
struct VS_IN
{
  float4 pos   : POSITION;   // 頂点座標
  float2 texel : TEXCOORD0;  // テクセル
};

// 頂点シェーダーの出力パラメータ
struct VS_OUT
{
  float4 pos    : SV_POSITION;
  float2 texel  : TEXCOORD0;

  float4  view_pos : VIEW_POS;
};

// *****************************************************************************************
// 頂点シェーダー
// *****************************************************************************************
VS_OUT VS_Main( VS_IN In )
{
    VS_OUT Out = (VS_OUT)0;

    //ワールド変換
    float4 pos = mul(float4(In.pos.xyz, 1), c_world);

    Out.view_pos = pos ;

    //ビュー変換
    pos = mul(pos, c_view);    

    //射影変換
    pos = mul(pos, c_proj);

    Out.pos = pos ;   
    Out.texel = In.texel;
    return Out;

}

// *****************************************************************************************
// ピクセルシェーダ
// *****************************************************************************************
typedef VS_OUT PS_IN;
float4 PS_Main( PS_IN In ) : SV_TARGET
{
    float4 col = g_texture.Sample( g_sampler, In.texel ) ;
    return col ;
}