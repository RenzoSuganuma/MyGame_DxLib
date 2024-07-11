// #include "../PixelShader.h"

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

struct PS_INPUT
{
	float2 TexCoords       : TEXCOORD0 ;	
	float4 WorldPosition   : POSITION ;
	float4 Position        : SV_POSITION ;	
} ;

struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;
} ;
 
static const int PROJECTION_MODE_VOLUME_Y = 0;
static const int PROJECTION_MODE_VOLUME_Z = 1;
static const int PROJECTION_MODE_NEGATIVE_OVER_Y = 2;
static const int PROJECTION_MODE_NEGATIVE_OVER_Z = 3;
static const int PROJECTION_MODE_POSITIVE_OVER_Y = 4;
static const int PROJECTION_MODE_POSITIVE_OVER_Z = 5;

static const int USE_COLOR_FLAG_RED = 1 ;
static const int USE_COLOR_FLAG_GREEN = 2 ;
static const int USE_COLOR_FLAG_BLUE = 4 ;

SamplerState g_DiffuseMapSampler            : register( s0 ) ;		
Texture2D    g_DiffuseMapTexture            : register( t0 ) ;		


PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT	PSOutput ;
	//float4		TextureDiffuseColor ;
	// TextureDiffuseColor =  g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords ) ;
	//PSOutput.Color0 = float4(c_col_density, c_col_density, c_col_density, 0) ;
	
	PSOutput.Color0 = float4(0, 0, 0, 0) ;
	if( c_use_color_flag & USE_COLOR_FLAG_RED ){
		PSOutput.Color0.x = c_col_density ;
	}
	if( c_use_color_flag & USE_COLOR_FLAG_GREEN ){
		PSOutput.Color0.y = c_col_density ;
	}
	if( c_use_color_flag & USE_COLOR_FLAG_BLUE ){
		PSOutput.Color0.z = c_col_density ;
	}

	float vp = 0 ;
	float negative = c_border_position - c_area_volume ;
	float positive = c_border_position + c_area_volume ;

	if( c_projection_mode == PROJECTION_MODE_VOLUME_Y ){
		vp = PSInput.WorldPosition.y ;
		if( vp > negative && vp < positive ){
			PSOutput.Color0.a = 1.0f ;
		}
	}else if( c_projection_mode == PROJECTION_MODE_VOLUME_Z ){
		vp = PSInput.WorldPosition.z ;
		if( vp > negative && vp < positive ){
			PSOutput.Color0.a = 1.0f ;
		}
	}else if( c_projection_mode == PROJECTION_MODE_NEGATIVE_OVER_Y ){
		vp = PSInput.WorldPosition.y ;
		if( vp < c_border_position ){
			PSOutput.Color0.a = 1.0f ;
		}
	}else if( c_projection_mode == PROJECTION_MODE_NEGATIVE_OVER_Z ){
		vp = PSInput.WorldPosition.z ;
		if( vp < c_border_position ){
			PSOutput.Color0.a = 1.0f ;
		}
	}else if( c_projection_mode == PROJECTION_MODE_POSITIVE_OVER_Y ){
		vp = PSInput.WorldPosition.y ;
		if( vp > c_border_position ){
			PSOutput.Color0.a = 1.0f ;
		}
	}else if( c_projection_mode == PROJECTION_MODE_POSITIVE_OVER_Z ){
		vp = PSInput.WorldPosition.z ;
		if( vp > c_border_position ){
			PSOutput.Color0.a = 1.0f ;
		}
	}

	// if(abs(PSInput.WorldPosition.y) < c_area_volume){
	// 	PSOutput.Color0.a = 1.0f ;
	// }
			
	return PSOutput ;
}

