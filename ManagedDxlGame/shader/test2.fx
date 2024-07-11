cbuffer VSMatrix: register(b0) {
	float4x4 view : packoffset( c0 );
	float4x4 proj : packoffset( c4 );
} ;

// VSInput structure
struct VSInput {
	float4 Position:	POSITION;
	float3 Normal:		NORMAL;
	float2 Texture:     TEXCOORD0;
	float4x4 mat : MATRIX;
	uint InstanceID : SV_InstanceID;	
} ;

// VSOutput structure
struct VSOutput {
	float4 Position:	SV_POSITION;
	float3 Normal:		NORMAL;
	float2 Texture:     TEXCOORD0;
} ;

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

//---------------------------------------------------------------------
// vertex shader
//---------------------------------------------------------------------
VSOutput vs_main(VSInput input)
{
	VSOutput output = (VSOutput)0;

	//頂点の位置を変換
	float4 pos = float4(input.Position.xyz, 1.0f);
	//ワールド変換
	pos = mul(pos, input.mat);
	//ビュー変換
	pos = mul(pos, view);
	//射影変換
	pos = mul(pos, proj);

	output.Position = pos ;
	output.Normal   = normalize(input.Normal);
	output.Texture  = input.Texture;

	return output;
}

//---------------------------------------------------------------------
// pixel shader
//---------------------------------------------------------------------
float4 ps_main(VSOutput input): SV_TARGET
{
	//return saturate(color_linear);
	return txDiffuse.Sample( samLinear, input.Texture ) ;
}
