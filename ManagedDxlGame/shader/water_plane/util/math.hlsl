#include "util/matrix.hlsl"
#include "util/quaternion.hlsl"

// float 型の最小正規化値
static const float FLT_EPSILON = 1.175494351e-38;
// 円周率
static const float PI = 3.14159265358979323846 ;
// ネイピア数
static const float EULER_NUMBER = 2.71828182845904523536 ;

//--------------------------------------------------------------------------------------------------------
// 乱数生成器
float Random(float seed)
{
    return frac(sin(seed) * 43758.5453);
}
float RandomInRange(float minValue, float maxValue, float seed)
{
    float randomValue = Random( seed ) ; 
    return lerp(minValue, maxValue, randomValue);
}

//--------------------------------------------------------------------------------------------------------
// カメラのビュー行列からカメラの座標を取得する関数
float3 GetCameraPosition(float4x4 viewMatrix)
{
    // ビュー行列の逆行列を取得
    float4x4 inverseViewMatrix = MatrixInverse(viewMatrix) ;

    // カメラの座標を取得
    float4 cameraPosition = inverseViewMatrix[3];
    
    return cameraPosition.xyz;
}

//--------------------------------------------------------------------------------------------------------
float3 TransformCoord(float3 v, float4 quaternion)
{
    // クォータニオンにベクトルを掛けて変換
    float4 transformedQuaternion = QuaternionMultiply(float4(v.x, v.y, v.z, 1), quaternion);
    
    // 変換されたクォータニオンを元のクォータニオンの共役で逆変換
    transformedQuaternion = QuaternionMultiply(QuaternionConjugate(quaternion), transformedQuaternion);

    // 変換されたベクトルの x, y, z 成分を取り出して返す
    return float3(transformedQuaternion.x, transformedQuaternion.y, transformedQuaternion.z);
}

//--------------------------------------------------------------------------------------------------------
float3 Vector3UniformLerp(float3 s, float3 e, float time_limit, float ct) {
    if (time_limit <= 0 || length(e - s) <= FLT_EPSILON) return s;
    if (ct >= time_limit) return e;
    return s + (e - s) * (ct / time_limit);
}

//------------------------------------------------------------------------------------------------------------------------
float3 Vector3SmoothLerp(float3 s, float3 e, float time_limit, float ct, int strength) {
    if (time_limit <= 0 || length(e - s) <= FLT_EPSILON) return s;
    if (ct >= time_limit) return e;
    strength = (strength > 5) ? 5 : strength;

    float t = 0;
    float si = 0;
    float cct = ct;

    for (int i = 0; i < strength; ++i) {
        float ss = 0;
        float ee = time_limit;

        t = cct / time_limit * 180;
        si = 0.5f + sin(radians(-90) + radians(t)) * 0.5f;
        cct = ss + si * (ee - ss);
    }

    t = cct / time_limit * 180;
    si = 0.5f + sin(radians(-90) + radians(t)) * 0.5f;
    return s + (e - s) * si;

}

//------------------------------------------------------------------------------------------------------------------------
float3 Vector3AccelLerp(float3 s, float3 e, float time_limit, float ct, int strength) {
    // ゼロまたは負の時間制限、または初期位置と終了位置が同じ場合
    if (time_limit <= 0 || length(e - s) <= FLT_EPSILON) return s;
    if (ct >= time_limit) return e;
    strength = (strength > 5) ? 5 : strength;

    float distance = 0;
    float fluctuation = 0;
    float a = 0;
    float n = 0;
    float cct = ct;
    float3 vn ;

    for (int i = 0; i < strength; ++i) {
        float ss = 0;
        float ee = time_limit;

        distance = ee - ss;
        fluctuation = (2.0f * distance) / pow(time_limit, 2.0f);

        a = fluctuation;
        n = (ee - ss) / abs(ee - ss);
        cct = n * ((fluctuation * pow(cct, 2)) * 0.5f);
    }

    // 自由落下の公式を利用して加速度を計算 
    // 公式 t = sqrt( 2h / g )
    // 変形して g = 2h / t^2
    // [ t:時間  h:高さ  g:重力加速度 ]
    distance = length(e - s) ;
    fluctuation = (2.0f * distance) / pow(time_limit, 2.0f);

    // 等加速直線運動の公式 x = x0 + ( v0 * t ) + ( 1/2 * a * t^2 )
    // ( v0 * t ) は初速だが今回は使わないので省略
    // [ x:現在位置  x0:初期位置  t:時間  a:加速度 ]
    a = fluctuation;
    vn = normalize( e - s );
    return s + vn * ((fluctuation * pow(cct, 2)) * 0.5f);

}

//------------------------------------------------------------------------------------------------------------------------
float Vector3DecelLerp(float3 s, float3 e, float time_limit, float ct) {
    // ゼロまたは負の時間制限、または初期位置と終了位置が同じ場合
    if (time_limit <= 0 || length(e - s) <= FLT_EPSILON) return s;
    if (ct >= time_limit) return e;

    float distance = length(e - s) ;
    float fluctuation = (2.0f * distance) / pow(time_limit, 2.0f);

    float a = fluctuation;
    float t = ct;
    float3 vn = normalize(e-s);
    return e - vn * ((a * pow(time_limit - t, 2)) * 0.5f);
}

//--------------------------------------------------------------------------------------------------------
float Angle(float3 v1, float3 v2) {
    float3 t1 = normalize(v1);
    float3 t2 = normalize(v2);
    return acos(dot(t1,t2));
}

//--------------------------------------------------------------------------------------------------------
float3 RandomAxis(float3 base_axis, float limit_angle, float random_seed) 
{
    float3 cv = cross(base_axis, float3(1, 0, 0));
    if (length(cv) <= FLT_EPSILON) cv = cross(base_axis, float3(0, 1, 0));
    if (length(cv) <= FLT_EPSILON) cv = cross(base_axis, float3(0, 0, 1));
    if (length(cv) <= FLT_EPSILON) return float3(0, 0, 0);

    float angle = RandomInRange(-limit_angle, limit_angle, random_seed);

    float4 rot = QuaternionRotationAxis( normalize(cv), radians( angle ) );

    float3 random_v = TransformCoord( base_axis, rot );
    angle = RandomInRange( -90, 90, random_seed + 1 );

    rot = QuaternionRotationAxis( base_axis, radians( angle ) );
    random_v = TransformCoord( random_v, rot );
    return random_v;    
}

//--------------------------------------------------------------------------------------------------------
float3 RandomPointInsidePlane(float3 plane_position, float3 plane_normal, float radius, float random_0_to_1) {

    if (length(plane_normal) <= FLT_EPSILON) return plane_position;

    float3 cv = cross(plane_normal, float3(-1, 0, 0));
    if (length(cv) <= FLT_EPSILON) cv = cross(plane_normal, float3(0, 1, 0));
    if (length(cv) <= FLT_EPSILON) cv = cross(plane_normal, float3(0, 0, 1));

    float4 rot = QuaternionRotationAxis( cv, radians(90) );
    float3 random_v = TransformCoord( plane_normal, rot );

    float angle = RandomInRange(-180, 180, random_0_to_1 );
    rot = QuaternionRotationAxis( plane_normal, radians(angle) );
    random_v = TransformCoord(random_v, rot);
    return plane_position + random_v * radius * random_0_to_1 ;
}

//--------------------------------------------------------------------------------------------------------
float3 BezierSpline(float3 _a1, float3 _a2, float3 _a3, float3 _a4, float t)
{

    t = (1.0f < t) ? 1.0f : t;
    t = (0.0f > t) ? 0.0f : t;

    float wkT = (1.0f - t);
    float wkX, wkY, wkZ;
    wkX = wkY = wkZ = 0.0f;

    float T2 = (t * t);
    float T3 = (t * t * t);
    float wkT2 = (wkT * wkT);
    float wkT3 = (wkT * wkT * wkT);

    wkX = ((wkT3) * _a1.x);
    wkX = wkX + ((3 * (wkT2)) * (t * _a2.x));
    wkX = wkX + (((3 * wkT) * (T2)) * (_a3.x));
    wkX = wkX + ((T3) * _a4.x);

    wkY = ((wkT3) * _a1.y);
    wkY = wkY + ((3 * (wkT2)) * (t * _a2.y));
    wkY = wkY + (((3 * wkT) * (T2)) * (_a3.y));
    wkY = wkY + ((T3) * _a4.y);

    wkZ = ((wkT3) * _a1.z);

    wkZ = wkZ + ((3 * (wkT2)) * (t * _a2.z));
    wkZ = wkZ + (((3 * wkT) * (T2)) * (_a3.z));
    wkZ = wkZ + ((T3) * _a4.z);

    return float3(wkX, wkY, wkZ);
}

//--------------------------------------------------------------------------------------------------------
// 自由落下の座標計算
// arg1... 重力方向
// arg1... 重力係数
// arg2... 移動方向
// arg3... 初速
// arg4... 経過時間
// ret.... 自由落下座標
float3 FreeFallPosition( float3 gravity_dir, float gravity_factor, float3 direction, float velocity, float time ){
   float gravity_x = 9.80665f * gravity_factor * gravity_dir.x ;
   float gravity_y = 9.80665f * gravity_factor * gravity_dir.y ;
   float gravity_z = 9.80665f * gravity_factor * gravity_dir.z ;
   float3 free_fall_position ;
   free_fall_position.x = gravity_x * pow(time, 2) + direction.x * velocity * time;
   free_fall_position.y = gravity_y * pow(time, 2) + direction.y * velocity * time;
   free_fall_position.z = gravity_z * pow(time, 2) + direction.z * velocity * time;
   return free_fall_position ;
}
