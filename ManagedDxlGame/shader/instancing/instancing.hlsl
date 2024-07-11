
cbuffer VSMatrix : register(b0)
{
	float4x4 c_view			: packoffset( c0 );
	float4x4 c_proj			: packoffset( c4 );
	float4	c_diffuse		: packoffset( c8 );
	float4	c_ambient		: packoffset( c9 );
	float4	c_emissive		: packoffset( c10 );
	float4	c_specular		: packoffset( c11 );

	float3  c_light_direction	: packoffset(c12);
	float	c_alpha				: packoffset(c12.w);
	float4	c_light_diffuse		: packoffset(c13);
	float4	c_light_ambient		: packoffset(c14);
	float4	c_light_specular	: packoffset(c15);
	int     c_light_enable		: packoffset(c16.x);
}

Texture2DArray textures : register(t0);
SamplerState samLinear : register(s0);

struct VSInput
{
	float4 Position 	: SV_Position;
	float3 Normal		: NORMAL;
	float2 Texture		: TEXCOORD0;
	column_major float4x4 world		: MATRIX;
	uint tex_index 		: TEX_INDEX ;
	uint InstanceID 	: SV_InstanceID;
};
struct PSInput
{
	float4	Position	: SV_POSITION ; 
	float2	Texture		: TEXCOORD0 ;
	float4  Diffuse     : COLOR0;
	float4  Specular    : COLOR1;
	uint    tex_index   : TEX_INDEX ;
};

float4 VSShadowMap(VSInput input) : SV_POSITION {
	float4 position = float4(input.Position.xyz, 1.0f);
	position = mul(position, input.world);
	position = mul(position, c_view);
	position = mul(position, c_proj);
	return position;
}

PSInput VSMain(VSInput input)
{

	PSInput result = (PSInput)0;

	float4 lWorldPosition ;
	float4 lViewPosition;
	float3 lViewNrm;
	float3 lLightHalfVec;
	float4 lLightLitParam;
	float4 lLightLitDest;

	//頂点の位置を変換
	float4 pos = float4(input.Position.xyz, 1.0f);
	//ワールド変換
	pos = mul(pos, input.world);
	//ビュー変換
	pos = mul(pos, c_view);
	//射影変換
	pos = mul(pos, c_proj);
	result.Position = pos;

	// ローカル座標をワールド座標に変換
	lWorldPosition.x = dot( input.Position.xyz, input.world[ 0 ].xyz ) ;
	lWorldPosition.y = dot( input.Position.xyz, input.world[ 1 ].xyz ) ;
	lWorldPosition.z = dot( input.Position.xyz, input.world[ 2 ].xyz ) ;
	lWorldPosition.w = 1.0f ;

	// ワールド座標をビュー座標に変換
	lViewPosition.x = dot( lWorldPosition, c_view[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, c_view[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, c_view[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// ローカルベクトルをワールドベクトルに変換
	lViewNrm.x = dot( input.Normal, input.world[ 0 ].xyz ) ;
	lViewNrm.y = dot( input.Normal, input.world[ 1 ].xyz ) ;
	lViewNrm.z = dot( input.Normal, input.world[ 2 ].xyz ) ;

	// ライトディフューズカラーとライトスペキュラカラーの角度減衰計算 =======( 開始 )

	// 法線とライトの逆方向ベクトルとの内積を lLightLitParam.x にセット
	lLightLitParam.x = dot(lViewNrm, -c_light_direction);

	// ハーフベクトルの計算 norm( ( norm( 頂点位置から視点へのベクトル ) + ライトの方向 ) )
	lLightHalfVec = normalize(normalize(-lViewPosition.xyz) - c_light_direction);

	// 法線とハーフベクトルの内積を lLightLitParam.y にセット
	lLightLitParam.y = dot(lLightHalfVec, lViewNrm);

	// スペキュラ反射率を lLightLitParam.w にセット
	lLightLitParam.w = c_specular.w;

	// ライトパラメータ計算
	lLightLitDest = lit(lLightLitParam.x, lLightLitParam.y, lLightLitParam.w);

	// ライトディフューズカラーとライトスペキュラカラーの角度減衰計算 =======( 終了 )

	// ライトの処理 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ディフューズカラー =
	//            ディフューズ角度減衰計算結果 *
	//            ライトのディフューズカラー *
	//            マテリアルのディフューズカラー +
	//            ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの +
	//            マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの
	result.Diffuse = lLightLitDest.y * c_light_diffuse * c_diffuse + c_light_ambient * c_ambient + c_emissive;

	// ディフューズアルファはマテリアルのディフューズカラーのアルファをそのまま使う
	result.Diffuse.w = c_diffuse.w;

	// スペキュラカラー = スペキュラ角度減衰計算結果 * ライトのスペキュラカラー * マテリアルのスペキュラカラー
	result.Specular = lLightLitDest.z * c_light_specular * c_specular;

	result.Texture = input.Texture;
	result.tex_index = input.tex_index;
	return result;

}


float4 PSMain(PSInput input) : SV_TARGET
{
	float4 out_color ;
	if(c_light_enable == 1){
		// テクスチャカラーの読み込み
		float4 TextureDiffuseColor = textures.Sample(samLinear, float3(input.Texture, input.tex_index));

		// 出力カラー = ディフューズカラー * テクスチャカラー + スペキュラカラー
		out_color = input.Diffuse * TextureDiffuseColor + input.Specular;

		// 出力アルファ = ディフューズアルファ * テクスチャアルファ * 不透明度
		out_color.a = input.Diffuse.a * TextureDiffuseColor.a * c_alpha;
	}else{
		out_color = textures.Sample(samLinear, float3(input.Texture, input.tex_index)) * float4( c_emissive.xyz, 1 );
	}
	return out_color;
}

