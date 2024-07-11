cbuffer VSMatrix: register(b0) {
	float4x4 World:		packoffset(c0);
	float4x4 WorldView:	packoffset(c4);
	float4x4 WorldViewProj:	packoffset(c8);
} ;

// VSInput structure
struct VSInput {
	float3 Position:	POSITION;
	float3 Normal:		NORMAL;
	float2 Texture:     TEXCOORD0;
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

	output.Position = mul(float4(input.Position, 1.0f), WorldViewProj);
	output.Normal   = normalize(mul(input.Normal, (float3x3)World));
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
