
cbuffer cbBuffer0 : register( b0 )
{
    float4x4 c_view			: packoffset( c0 );
    float4x4 c_proj			: packoffset( c4 );
    float4x4 c_world		: packoffset( c8 );
	float c_area_volume		: packoffset( c12.x );
	float c_col_density     : packoffset( c12.y );
	float c_border_position : packoffset( c12.z );
	int c_projection_mode   : packoffset( c12.w );	
	int c_use_color_flag	: packoffset( c13.x );
} ;

struct VS_INPUT
{
	float3 Position        : POSITION ;			
	float3 Normal          : NORMAL ;			
	float4 Diffuse         : COLOR0 ;			
	float4 Specular        : COLOR1 ;			
	float4 TexCoords0      : TEXCOORD0 ;		
	float4 TexCoords1      : TEXCOORD1 ;		
} ;

struct VS_OUTPUT
{
	float2 TexCoords	   : TEXCOORD0 ;	
	float4 WorldPosition   : POSITION ;
	float4 Position        : SV_POSITION ;	
} ;

VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT	VSOutput ;
	float4		lLocalWorldMatrix[ 3 ] ;
	float4		lLocalPosition ;
	float4		lWorldPosition ;
	float4		lViewPosition ;
	
	lLocalPosition.xyz = VSInput.Position ;
	lLocalPosition.w = 1.0f ;

	lWorldPosition.x = dot( lLocalPosition, c_world[ 0 ] ) ;
	lWorldPosition.y = dot( lLocalPosition, c_world[ 1 ] ) ;
	lWorldPosition.z = dot( lLocalPosition, c_world[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;
	VSOutput.WorldPosition = lWorldPosition ;

	lViewPosition.x = dot( lWorldPosition, c_view[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, c_view[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, c_view[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	VSOutput.Position.x = dot( lViewPosition, c_proj[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, c_proj[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, c_proj[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, c_proj[ 3 ] ) ;

	VSOutput.TexCoords = VSInput.TexCoords0.xy ;

	return VSOutput ;
}

