#include "util/math.hlsl"

// 定数バッファ
cbuffer ConstantBuffer : register( b0 )
{
   // 列優先
   column_major  float4x4 c_matView : packoffset( c0 );
   column_major  float4x4 c_matProj : packoffset( c4 );

   float3 c_emitter_position            : packoffset( c8 );
   float1 c_size_x                      : packoffset( c8.w );
   float3 c_emissive                    : packoffset( c9 );
   float1 c_time_limit                  : packoffset( c9.w );   
   float3 c_direction                   : packoffset( c10 );
   float1 c_delta_time                  : packoffset( c10.w );
   float3 c_gravity_dir                 : packoffset( c11 );
   float1 c_disp_size_factor            : packoffset( c11.w );
   float3 c_camera_position             : packoffset( c12 ) ;
   float1 c_start_velocity              : packoffset( c12.w );
   float3 c_conv_position               : packoffset( c13 ) ;
   float1 c_gravity_factor              : packoffset( c13.w );
   float3 c_origin_axis                 : packoffset( c14 ) ;
   float1 c_random_seed                 : packoffset( c14.w );
   float1 c_eject_random_angle_range    : packoffset( c15.x );
   float1 c_waver_factor                : packoffset( c15.y );
   float1 c_lumi_wave_factor            : packoffset( c15.z );
   float1 c_rotate_factor               : packoffset( c15.w );
   float1 c_distortion_range            : packoffset( c16.x ) ;
   float1 c_distortion_velocity         : packoffset( c16.y ) ;
   float1 c_scale                       : packoffset( c16.z ) ;
   float1 c_alpha                       : packoffset( c16.w ) ;
   float1 c_disp_alpha_factor           : packoffset( c17.x ) ;
   float1 c_on_emitte_size              : packoffset( c17.y ) ;
   float1 c_moving_decay_factor         : packoffset( c17.z ) ;
   float1 c_near_alpha_distance         : packoffset( c17.w ) ;
   float1 c_origin_range                : packoffset( c18.x ) ;
   float1 c_size_y                      : packoffset( c18.y ) ;
   int    c_origin_mode                 : packoffset( c18.z ) ;
   int    c_play_type                   : packoffset( c18.w ) ;
   int    c_adulation_emitter           : packoffset( c19.x ) ;
   int    c_distortion_mode             : packoffset( c19.y ) ;
   int    c_move_sruface                : packoffset( c19.z ) ;
   int    c_posture_type                : packoffset( c19.w ) ;
   int    c_disp_size_mode              : packoffset( c20.x ) ;
   int    c_disp_alpha_mode             : packoffset( c20.y ) ;
   int    c_eject_type                  : packoffset( c20.z ) ;
};

// ループ or 単発再生モード
static const int PLAY_TYPE_LOOP = 0;
static const int PLAY_TYPE_ONCE = 1;
static const int PLAY_TYPE_RE_PLAY = 2 ;

// エミッターへの追従
static const int TRUE_ADULATION_EMITTER = 1;

// 歪みのモード
static const int DISTORTION_MODE_CHAOS = 0 ;
static const int DISTORTION_MODE_ORDER = 1 ;

// 生成の起点モード
static const int ORIGIN_MODE_XYZ = 0 ;
static const int ORIGIN_MODE_XY = 1 ;
static const int ORIGIN_MODE_XZ = 2 ;
static const int ORIGIN_MODE_YZ = 3 ;
static const int ORIGIN_MODE_AXIS = 4 ;
static const int ORIGIN_MODE_SPHERE = 5 ;
static const int ORIGIN_MODE_CXY = 6 ;
static const int ORIGIN_MODE_CXZ = 7 ;
static const int ORIGIN_MODE_CYZ = 8 ;

// 放出・拡散モード
static const int EJECT_TYPE_DIFF = 0 ;
static const int EJECT_TYPE_CONV = 1 ;
static const int EJECT_TYPE_ICONV = 2 ;

// 移動平面
static const int MOVE_SURFACE_XYZ = 0 ;
static const int MOVE_SURFACE_XY = 1 ;
static const int MOVE_SURFACE_XZ = 2 ;
static const int MOVE_SURFACE_YZ = 3 ;

// 姿勢タイプ
static const int POSTURE_TYPE_BXYZ = 0 ;
static const int POSTURE_TYPE_BY = 1 ;
static const int POSTURE_TYPE_OBY = 2 ;
static const int POSTURE_TYPE_UpYD = 3 ;
static const int POSTURE_TYPE_X = 4 ;
static const int POSTURE_TYPE_Y = 5 ;
static const int POSTURE_TYPE_Z = 6 ;

// サイズ変位モード
static const int DISP_SIZE_MODE_NONE = 0 ;
static const int DISP_SIZE_MODE_EXIST = 1 ;
static const int DISP_SIZE_MODE_BIGGER = 2 ;
static const int DISP_SIZE_MODE_SMALLER = 3 ;

// 透明度変位モード
static const int DISP_ALPHA_MODE_NONE = 0 ;
static const int DISP_ALPHA_MODE_EXIST = 1 ;
static const int DISP_ALPHA_MODE_INC = 2 ;
static const int DISP_ALPHA_MODE_DEC = 3 ;


// テクスチャー
Texture2D g_texture : register( t0 );

// サンプラーステート
SamplerState  g_sampler : register( s0 );

// 頂点シェーダーの入力パラメータ
struct VS_IN
{
    float3 spown_position        : SPOWN_POSITION ;      // 出現座標
    float3 before_position       : BEFORE_POSITION ;     // 更新前座標
    float3 update_position       : UPDATE_POSITION ;     // 更新座標
    float3 direction             : DIRECTION ;           // 放出向き
    float move_velocity          : MOVE_VELOCITY ;       // 移動速度
    float progress_time          : PROGRESS_TIME ;       // 経過時間
    float moving_progress_time   : MOVING_PROGRESS_TIME ;// 移動計算用経過時間
    float alpha                  : ALPHA ;               // 透明度 
    float size_x                 : SIZE_X ;              // サイズ
    float size_y                 : SIZE_Y ;              // サイズ
    float rand_value             : RAND_VALUE ;          // 各頂点の固有乱数( 0 ～ 1.0 )
    int process_counter          : PROCESS_COUNTER ;     // プロセスカウンター
    int vertex_index             : VERTEX_INDEX;         // 各頂点の固有インデックス
};


//--------------------------------------------------------------------------------------------------------
// 頂点シェーダーではそのまま出力
typedef VS_IN VS_OUT;
VS_OUT VS0_Main( VS_IN In )
{
   return In;
}

//--------------------------------------------------------------------------------------------------------
typedef VS_OUT GS_IN;
typedef GS_IN GS0_OUT;
[maxvertexcount(1)]   // ジオメトリシェーダーで出力する最大頂点数
// ジオメトリシェーダー
void GS0_Main( point GS_IN In[1],                   // ポイント プリミティブの入力情報
               inout PointStream<GS0_OUT> PStream   // ポイント プリミティブの出力ストリーム
             )
{
   GS0_OUT Out;

   float cc_size_x = c_size_x * c_scale ;
   float cc_size_y = c_size_y * c_scale ;
   float cc_start_velocity = c_start_velocity * c_scale ;
   float cc_gravity_factor = c_gravity_factor * c_scale ;
   float1 cc_waver_factor = c_waver_factor * c_scale ;
   float1 cc_distortion_range = c_distortion_range * c_scale ;
   float3 cc_emitter_position = c_emitter_position ;

   float1 reset_rand_seed = ( c_random_seed + In[0].vertex_index + In[0].rand_value ) / ( c_time_limit + 1.0 ) ;
   float1 reset_rand_value = RandomInRange(0, 1, reset_rand_seed) ;

   if(c_origin_mode == ORIGIN_MODE_XYZ){
      float3 rv = RandomAxis( float3(0, 1, 0), 180, reset_rand_seed );
      float1 ro1 = RandomInRange(-c_origin_range/2, c_origin_range/2, In[0].rand_value) ;
      float1 ro2 = RandomInRange(-c_origin_range/2, c_origin_range/2, reset_rand_value) ;
      cc_emitter_position += rv * (ro1 + ro2)  ;
   }else if(c_origin_mode == ORIGIN_MODE_XY){
      float3 rxy1 = RandomPointInsidePlane(float3(0, 0, 0), float3(0, 0, 1), c_origin_range / 2, In[0].rand_value);
      float3 rxy2 = RandomPointInsidePlane(float3(0, 0, 0), float3(0, 0, 1), c_origin_range / 2, reset_rand_value);
      cc_emitter_position += rxy1 + rxy2 ;
   }else if(c_origin_mode == ORIGIN_MODE_XZ){
      float3 rxz1 = RandomPointInsidePlane(float3(0, 0, 0), float3(0, 1, 0), c_origin_range / 2, In[0].rand_value);
      float3 rxz2 = RandomPointInsidePlane(float3(0, 0, 0), float3(0, 1, 0), c_origin_range / 2, reset_rand_value);
      cc_emitter_position += rxz1 + rxz2 ;
   }else if(c_origin_mode == ORIGIN_MODE_YZ){
      float3 ryz1 = RandomPointInsidePlane(float3(0, 0, 0), float3(1, 0, 0), c_origin_range / 2, In[0].rand_value);
      float3 ryz2 = RandomPointInsidePlane(float3(0, 0, 0), float3(1, 0, 0), c_origin_range / 2, reset_rand_value);
      cc_emitter_position += ryz1 + ryz2  ;
   }else if(c_origin_mode == ORIGIN_MODE_AXIS){
      cc_emitter_position += RandomInRange(-c_origin_range, c_origin_range, reset_rand_value) * normalize(c_origin_axis) ;
   }else if(c_origin_mode == ORIGIN_MODE_SPHERE){
      float3 rv = RandomAxis( float3(0, 1, 0), 180, reset_rand_seed );
      float1 ro1 = RandomInRange(-c_origin_range/2, c_origin_range/2, In[0].rand_value) ;
      float1 ro2 = RandomInRange(-c_origin_range/2, c_origin_range/2, reset_rand_value) ;
      cc_emitter_position += normalize( rv * (ro1 + ro2) ) * c_origin_range ;
   }else if(c_origin_mode == ORIGIN_MODE_CXY){
      float3 rxy1 = RandomPointInsidePlane(float3(0, 0, 0), float3(0, 0, 1), c_origin_range / 2, In[0].rand_value);
      float3 rxy2 = RandomPointInsidePlane(float3(0, 0, 0), float3(0, 0, 1), c_origin_range / 2, reset_rand_value);
      cc_emitter_position += normalize( rxy1 + rxy2 ) * c_origin_range ;
   }else if(c_origin_mode == ORIGIN_MODE_CXZ){
      float3 rxz1 = RandomPointInsidePlane(float3(0, 0, 0), float3(0, 1, 0), c_origin_range / 2, In[0].rand_value);
      float3 rxz2 = RandomPointInsidePlane(float3(0, 0, 0), float3(0, 1, 0), c_origin_range / 2, reset_rand_value);
      cc_emitter_position += normalize( rxz1 + rxz2 ) * c_origin_range;
   }else if(c_origin_mode == ORIGIN_MODE_CYZ){
      float3 ryz1 = RandomPointInsidePlane(float3(0, 0, 0), float3(1, 0, 0), c_origin_range / 2, In[0].rand_value);
      float3 ryz2 = RandomPointInsidePlane(float3(0, 0, 0), float3(1, 0, 0), c_origin_range / 2, reset_rand_value);
      cc_emitter_position += normalize( ryz1 + ryz2 ) * c_origin_range ;
   }


   if(c_move_sruface == MOVE_SURFACE_XY){
      cc_emitter_position.z = c_emitter_position.z ;
   } else if(c_move_sruface == MOVE_SURFACE_XZ){
      cc_emitter_position.y = c_emitter_position.y ;
   } else if(c_move_sruface == MOVE_SURFACE_YZ){
      cc_emitter_position.x = c_emitter_position.x ;
   }

   Out.spown_position = In[0].spown_position ;
   Out.vertex_index = In[0].vertex_index ;
   Out.rand_value = In[0].rand_value ;

   //
   // 各頂点に対し、初回の実行時のみ適用される初期化処理
   //
   if(0 == In[0].process_counter){
      Out.spown_position = cc_emitter_position ;
      In[0].move_velocity = cc_start_velocity;
      In[0].direction = RandomAxis( c_direction, c_eject_random_angle_range, c_random_seed + In[0].vertex_index );
      In[0].size_x = c_size_x * c_on_emitte_size ;
      In[0].size_y = c_size_y * c_on_emitte_size ;

      if( c_play_type == PLAY_TYPE_ONCE ){
         In[0].progress_time = c_time_limit + FLT_EPSILON ;
      }
      In[0].process_counter = 1 ; 
   }


   //
   // パーティクルの時間が規定時間を超えた場合初期化する
   //
   if( In[0].progress_time > c_time_limit )
   {

      Out.spown_position = cc_emitter_position ;
      Out.before_position = cc_emitter_position ;
      Out.update_position = cc_emitter_position ;  
      Out.direction = RandomAxis(normalize(c_direction), c_eject_random_angle_range, c_random_seed + In[0].vertex_index) ;
      Out.move_velocity = cc_start_velocity;
      Out.process_counter = 1 ;

      if(c_play_type == PLAY_TYPE_LOOP ){
         Out.progress_time = In[0].progress_time - c_time_limit ;
         Out.moving_progress_time = In[0].progress_time - c_time_limit ;
         Out.size_x = c_size_x * c_on_emitte_size ;
         Out.size_y = c_size_y * c_on_emitte_size ;
      }else if( c_play_type == PLAY_TYPE_ONCE ){
         Out.progress_time = In[0].progress_time ;
         Out.moving_progress_time = In[0].progress_time ; 
         Out.size_x = 0 ;
         Out.size_y = 0 ;
      }else if( c_play_type == PLAY_TYPE_RE_PLAY ){
         Out.progress_time = 0 ;
         Out.moving_progress_time = 0 ;
         Out.size_x = c_size_x * c_on_emitte_size ;
         Out.size_y = c_size_y * c_on_emitte_size ;
      }

      Out.alpha = 0 ;
   }
   else{
      float3 base_position = (c_adulation_emitter == TRUE_ADULATION_EMITTER)? cc_emitter_position : Out.spown_position ;
      float3 to_conv_v = ( c_emitter_position + c_conv_position ) - base_position ;

      // ベジェスプラインに使用するアンカーポイントの計算
      float3 start = base_position;
      float3 end ;
      
      if(c_eject_type == EJECT_TYPE_DIFF){
         end = base_position + FreeFallPosition( c_gravity_dir, cc_gravity_factor, In[0].direction, In[0].move_velocity, c_time_limit ) ;
      }else if(c_eject_type == EJECT_TYPE_CONV){
         end = base_position + to_conv_v ;
      }else if(c_eject_type == EJECT_TYPE_ICONV){
         end = base_position + FreeFallPosition( c_gravity_dir, cc_gravity_factor, normalize(-to_conv_v), In[0].move_velocity, c_time_limit ) ;
      }

      float3 anchor1 = start + (end - start) * 0.25;
      float3 anchor2 = start + (end - start) * 0.75;
      float3 nml = normalize(end - start);
      anchor1 = RandomPointInsidePlane(anchor1, nml, cc_waver_factor, In[0].rand_value);
      anchor2 = RandomPointInsidePlane(anchor2, nml, cc_waver_factor, c_random_seed + In[0].rand_value );

      // ベジェスプライン補間によって揺らぎの影響を受けた座標
      float3 waver_affected_position ;
      float3 bz1 = base_position ;
      float3 bz2 = anchor1 ;
      float3 bz3 = anchor2 ;
      float3 bz4 ;

      if(c_eject_type == EJECT_TYPE_DIFF){
         bz4 = base_position + FreeFallPosition( c_gravity_dir, cc_gravity_factor, In[0].direction, In[0].move_velocity, c_time_limit ) ; 
      }else if(c_eject_type == EJECT_TYPE_CONV){
         bz4 = base_position + to_conv_v ;
      }else if(c_eject_type == EJECT_TYPE_ICONV){
         bz4 = base_position + FreeFallPosition( c_gravity_dir, cc_gravity_factor, normalize(-to_conv_v), In[0].move_velocity, c_time_limit ) ;
      }

      waver_affected_position = BezierSpline( bz1, bz2, bz3, bz4, ( In[0].moving_progress_time / c_time_limit ) );

      float3 adv_position ;
      // 自由落下の計算で得られる座標
      if(c_eject_type == EJECT_TYPE_DIFF){
         adv_position = base_position + FreeFallPosition( c_gravity_dir, cc_gravity_factor, In[0].direction, In[0].move_velocity, In[0].moving_progress_time );
      // 収束座標までの移動座標
      }else if(c_eject_type == EJECT_TYPE_CONV){
         float ct = ( In[0].moving_progress_time / c_time_limit ) ;
         ct = (ct <= FLT_EPSILON)? 0.001 : ct ;
         adv_position = Vector3UniformLerp(base_position, base_position + to_conv_v, c_time_limit, ct) ;
      }else if(c_eject_type == EJECT_TYPE_ICONV){
         adv_position = base_position + FreeFallPosition( c_gravity_dir, cc_gravity_factor, normalize(-to_conv_v), In[0].move_velocity, In[0].moving_progress_time );
      }

      // 双方の影響を受けた座標を計算
      Out.before_position = In[0].update_position ;
      Out.update_position = ( adv_position + waver_affected_position ) * 0.5 ;
      if(c_move_sruface == MOVE_SURFACE_XY){
         Out.update_position.z = base_position.z ;
      } else if(c_move_sruface == MOVE_SURFACE_XZ){
         Out.update_position.y = base_position.y ;
      } else if(c_move_sruface == MOVE_SURFACE_YZ){
         Out.update_position.x = base_position.x ;
      }

      float moving_delta_rate = 1.0 - ( In[0].progress_time / c_time_limit );
      moving_delta_rate = (c_moving_decay_factor > FLT_EPSILON) ? pow(moving_delta_rate, c_moving_decay_factor) : 1.0 ;

      Out.direction = In[0].direction;
      Out.move_velocity = In[0].move_velocity;
      Out.moving_progress_time = In[0].moving_progress_time + c_delta_time * moving_delta_rate ;
      Out.progress_time = In[0].progress_time + c_delta_time;
      Out.process_counter = In[0].process_counter+1 ;
      Out.size_x = In[0].size_x ;
      Out.size_y = In[0].size_y ;

      //
      // カメラに近すぎる場合に透明度を上げる
      //
      float near_alpha_dist = c_near_alpha_distance ;
      float dist = length( c_camera_position - Out.update_position ) ;
      float fluct_dist = (near_alpha_dist > FLT_EPSILON)? near_alpha_dist * 2 : 1.0 ;
      float clamp_dist = clamp(dist, 0, near_alpha_dist + fluct_dist) - near_alpha_dist ;
      float fluct = sin(PI / 2 * clamp_dist / fluct_dist) ;
      float dist_alpha = (dist < (fluct_dist + near_alpha_dist) )? (dist < near_alpha_dist )? 0 : fluct : 1.0 ;

      if(1 == In[0].process_counter){
         Out.alpha = 0;
      }else{
         Out.alpha = c_alpha * dist_alpha;
      }

   }


   PStream.Append( Out );
   PStream.RestartStrip();
}


//--------------------------------------------------------------------------------------------------------
// ジオメトリシェーダによって書き出される増幅された頂点情報
struct GS1_OUT
{
   float4 pos    : SV_POSITION;  // パーティクルの位置
   float4 color  : COLOR;        // パーティクルの色
   float2 texel  : TEXCOORD0;    // テクセル
};


[maxvertexcount(16)]   // ジオメトリシェーダーで出力する最大頂点数
// ジオメトリシェーダー
void GS1_Main( point GS_IN In[1],                       // ポイント プリミティブの入力情報
               inout TriangleStream<GS1_OUT> TriStream  // トライアングル プリミティブの出力ストリーム
              )
{
   GS1_OUT Out;

   float cc_size_x = In[0].size_x * c_size_x * c_scale ;
   float cc_size_y = In[0].size_y * c_size_y * c_scale ;
   float cc_start_velocity = c_start_velocity * c_scale ;
   float cc_gravity_factor = c_gravity_factor * c_scale ;
   float cc_waver_factor = c_waver_factor * c_scale ;
   float cc_distortion_range = c_distortion_range * c_scale ;

   float4x4 matVP = mul(c_matView, c_matProj) ;
   float time_rate = In[0].progress_time / c_time_limit;

   float disp_size = 1 ;
   if( c_disp_size_mode == DISP_SIZE_MODE_EXIST ){
      disp_size = sin( PI * time_rate ) * c_disp_size_factor ;
      disp_size = clamp(disp_size, 0, 1.0) ;
   } else if( c_disp_size_mode == DISP_SIZE_MODE_BIGGER ){
      disp_size = sin( PI/2 * time_rate ) * c_disp_size_factor ;
   } else if( c_disp_size_mode == DISP_SIZE_MODE_SMALLER ){
      disp_size = c_disp_size_factor - sin( PI/2 * time_rate ) * c_disp_size_factor ;
   }

   float disp_alpha = 1.0 ;
   if( c_disp_alpha_mode == DISP_ALPHA_MODE_EXIST ){
      disp_alpha = sin( PI * time_rate ) * c_disp_alpha_factor ;
      disp_alpha = clamp(disp_alpha, 0, 1.0) ;
   } else if( c_disp_alpha_mode == DISP_ALPHA_MODE_INC ){
      disp_alpha = sin( PI/2 * time_rate ) * c_disp_alpha_factor ;
   } else if( c_disp_alpha_mode == DISP_ALPHA_MODE_DEC ){
      disp_alpha = c_disp_alpha_factor - sin( PI/2 * time_rate ) * c_disp_alpha_factor ;
   }

   // 頂点カラー
   float lw = cos( PI * RandomInRange(0, 1.0, In[0].vertex_index) * c_lumi_wave_factor + In[0].progress_time * c_lumi_wave_factor ) ;
   //float4 color = float4( c_emissive, 1 ) * float4(lw, lw, lw, 1) * float4(1,1,1,In[0].alpha) * float4(1,1,1,disp_alpha) ;
   lw = ( c_lumi_wave_factor <= FLT_EPSILON )? 1 : clamp(lw, 0.1, 1) ;
   float4 color = ( float4( c_emissive, 1 ) * float4(lw, lw, lw, 1) ) * In[0].alpha * disp_alpha ;

   float random = RandomInRange(-1.0, 1.0, In[0].vertex_index) ;

   float4 rot = float4(0, 0, 0, 1);
   float4 posture_qt = float4(0, 0, 0, 1);
   float4 roll_qt = float4(0, 0, 0, 1);
   float3 roll_axis = float3(0, 0, 1) ;

   if(c_posture_type == POSTURE_TYPE_BXYZ){
      roll_axis = normalize(c_camera_position - In[0].update_position) ;
      posture_qt = QuaternionLookAt(In[0].update_position, c_camera_position, float3(0, 1, 0)) ;
      roll_qt = QuaternionRotationAxis(roll_axis, PI * In[0].progress_time * random * c_rotate_factor ) ;
      rot = QuaternionMultiply( posture_qt, roll_qt ) ;
   }else if(c_posture_type == POSTURE_TYPE_BY){
      float3 cam = float3(c_camera_position.x, 0, c_camera_position.z) ;
      float3 opos = float3(In[0].update_position.x, 0, In[0].update_position.z) ;
      roll_axis = normalize(cam - opos) ;
      posture_qt = QuaternionLookAt(opos, cam, float3(0, 1, 0)) ;
      roll_qt = QuaternionRotationAxis(roll_axis, PI * In[0].progress_time * random * c_rotate_factor ) ;
      rot = QuaternionMultiply( posture_qt, roll_qt ) ;
   }else if(c_posture_type == POSTURE_TYPE_OBY){
      roll_axis = float3(0, 1, 0) ;
      posture_qt = QuaternionRotationAxis(float3(1, 0, 0), PI/2) ;
      float3 cam = float3(c_camera_position.x, 0, c_camera_position.z) ;
      float3 opos = float3(In[0].update_position.x, 0, In[0].update_position.z) ;
      float4 lookat = QuaternionLookAt(opos, cam, float3(0, 1, 0)) ;
      posture_qt = QuaternionMultiply(posture_qt, lookat) ;
      roll_qt = QuaternionRotationAxis(roll_axis, PI * In[0].progress_time * random * c_rotate_factor ) ;
      rot = QuaternionMultiply( posture_qt, roll_qt ) ;

   }else if(c_posture_type == POSTURE_TYPE_UpYD){
      // UpYD
      float veye = normalize(c_camera_position - In[0].update_position) ;
      float dir_roll_axis = normalize(In[0].update_position - In[0].before_position) ;
      float4 qA = QuaternionRotationAxis(float3(-1, 0, 0), PI/2) ;
      float4 qB = QuaternionLookAt(In[0].before_position, In[0].update_position, float3(0, 1, 0)) ;
      float4 qC = QuaternionMultiply( qA, qB ) ;
      rot = QuaternionMultiply( qA, qB ) ;

   }else if(c_posture_type == POSTURE_TYPE_X){
      roll_axis = float3(1, 0, 0) ;
      posture_qt = QuaternionRotationAxis(float3(0, 1, 0), PI/2) ;
      roll_qt = QuaternionRotationAxis(roll_axis, PI * In[0].progress_time * random * c_rotate_factor ) ;
      rot = QuaternionMultiply( posture_qt, roll_qt ) ;
   }else if(c_posture_type == POSTURE_TYPE_Y){
      roll_axis = float3(0, 1, 0) ;
      posture_qt = QuaternionRotationAxis(float3(-1, 0, 0), PI/2) ;
      roll_qt = QuaternionRotationAxis(roll_axis, PI * In[0].progress_time * random * c_rotate_factor ) ;
      rot = QuaternionMultiply( posture_qt, roll_qt ) ;
   }else if(c_posture_type == POSTURE_TYPE_Z){
      roll_axis = float3(0, 0, 1) ;
      posture_qt = float4(0, 0, 0, 1);
      roll_qt = QuaternionRotationAxis(roll_axis, PI * In[0].progress_time * random * c_rotate_factor ) ;
      rot = QuaternionMultiply( posture_qt, roll_qt ) ;
   }

   float size_x = (color.w >= FLT_EPSILON)? (cc_size_x * disp_size) : 0 ;
   float size_y = (color.w >= FLT_EPSILON)? (cc_size_y * disp_size) : 0 ;
   float3 tvp = In[0].update_position ;
   float3 size_wave ; 

   float wave_range_x = size_x * cc_distortion_range * 0.5f ;
   float wave_range_y = size_y * cc_distortion_range * 0.5f ;
   float wave_velocity = c_distortion_velocity ;
 
   // 歪みの混沌モード
   if(DISTORTION_MODE_CHAOS == c_distortion_mode){
      float2 vmark[4] = { {1, -1}, {-1, -1}, {1, 1}, {-1, 1} } ;
      float2 texcoord[4] = { {1, 1}, {0, 1}, {1, 0}, {0, 0} } ;
      for (int i = 0; i < 4; ++i){
         random = RandomInRange(-1.0, 1.0, In[0].vertex_index + i*2 + 0 ) ;
         size_wave.x = size_x + wave_range_x * sin( PI * In[0].progress_time * random * wave_velocity ) * disp_size;
         random = RandomInRange(-1.0, 1.0, In[0].vertex_index + i*2 + 1 ) ;
         size_wave.y = size_y + wave_range_y * sin( PI * In[0].progress_time * random * wave_velocity ) * disp_size;
         size_wave.x *= vmark[i].x ;
         size_wave.y *= vmark[i].y ;

         float3 rvp = TransformCoord( float3( size_wave.x, size_wave.y, 0), rot ) ;
         Out.pos =  mul( float4( In[0].update_position + rvp, 1.0f ), matVP ) ;

         Out.color = color;
         Out.texel = texcoord[i] ;
         TriStream.Append( Out );
      }
      TriStream.RestartStrip();

   // 歪みの秩序モード
   }else{
      float2 vmark[9] = { 
         {-1, 1}, { 0, 1}, { 1, 1},
         {-1, 0}, { 0, 0}, { 1, 0},
         {-1, -1}, {0, -1}, {1, -1}
      } ;
      float2 texcoord[9] = { 
         {0, 0}, {0.5, 0}, {1, 0},
         {0, 0.5}, {0.5, 0.5}, {1, 0.5},
         {0, 1}, {0.5, 1}, {1, 1}
      } ;

      int vi[4][4] = {
         { 0, 1, 4, 2 },
         { 2, 5, 4, 8 },
         { 8, 7, 4, 6 },
         { 6, 3, 4, 0 }
      };
      
      float wave_range = (size_x < size_y)? wave_range_x : wave_range_y ;
      for(int k = 0 ; k < 4 ; ++k){
         for (int i = 0; i < 4; ++i){

            random = RandomInRange(-1.0, 1.0, In[0].vertex_index + vi[k][i] ) ;
            float3 vn = normalize(float3(vmark[vi[k][i]].x, vmark[vi[k][i]].y, 0)) ;
            float vl = length(float3(vmark[vi[k][i]].x, vmark[vi[k][i]].y, 0)) ;
            size_wave = ( vn * vl ) + ( vn * wave_range * abs(sin( PI * In[0].progress_time * random * wave_velocity )) * disp_size ) ;
            size_wave.x += size_x * vmark[vi[k][i]].x ;
            size_wave.y += size_y * vmark[vi[k][i]].y ;

            float3 rvp = TransformCoord( float3( size_wave.x, size_wave.y, 0), rot ) ;
            Out.pos =  mul( float4( In[0].update_position + rvp, 1.0f ), matVP ) ;

            Out.color = color;
            Out.texel = texcoord[vi[k][i]] ;
            TriStream.Append( Out );
         }
         TriStream.RestartStrip();
      }

   }

}


//--------------------------------------------------------------------------------------------------------
typedef GS1_OUT PS_IN;
// ピクセルシェーダ
float4 PS1_Main( PS_IN In ) : SV_TARGET
{
    return g_texture.Sample( g_sampler, In.texel ) * In.color  ;
}
