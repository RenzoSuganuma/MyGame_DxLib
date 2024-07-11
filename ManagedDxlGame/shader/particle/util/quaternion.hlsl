//--------------------------------------------------------------------------------------------------------
// 長さ
float QuaternionLength( float4 qtn )
{
    return sqrt(qtn.x * qtn.x + 
                qtn.y * qtn.y +
                qtn.z * qtn.z +
                qtn.w * qtn.w);    
}

//--------------------------------------------------------------------------------------------------------
// 正規化
float4 QuaternionNormalize( float4 qtn )
{
    float scale = 1.0f / QuaternionLength(qtn);
    return float4( qtn.x * scale, qtn.y * scale, qtn.z * scale, qtn.w * scale ) ;
}

//--------------------------------------------------------------------------------------------------------
// 共役クォータニオン
float4 QuaternionConjugate( float4 _q )
{
   return float4( -_q.x, -_q.y, -_q.z, _q.w ) ;
}

//--------------------------------------------------------------------------------------------------------
// 指定軸による回転
float4 QuaternionRotationAxis(float3 axis, float angle)
{
    float halfAngle = 0.5f * angle;
    float sinHalf = sin(halfAngle);
    return float4(axis * sinHalf, cos(halfAngle));
}

//--------------------------------------------------------------------------------------------------------
// 乗算
float4 QuaternionMultiply(float4 q1, float4 q2)
{
    return float4(
        q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
        q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
        q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
        );
}

//--------------------------------------------------------------------------------------------------------
// 回転行列を作成
float4x4 QuaternionToMatrix( float4 quat )
{
    float4x4 m = float4x4(float4(0, 0, 0, 0), float4(0, 0, 0, 0), float4(0, 0, 0, 0), float4(0, 0, 0, 0));

    float x = quat.x, y = quat.y, z = quat.z, w = quat.w;
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    m[0][0] = 1.0 - (yy + zz);
    m[0][1] = xy - wz;
    m[0][2] = xz + wy;

    m[1][0] = xy + wz;
    m[1][1] = 1.0 - (xx + zz);
    m[1][2] = yz - wx;

    m[2][0] = xz - wy;
    m[2][1] = yz + wx;
    m[2][2] = 1.0 - (xx + yy);

    m[3][3] = 1.0;
   
    return m ;
}

//--------------------------------------------------------------------------------------------------------
// 行列から作成
float4 QuaternionFromMatrix(float3x3 mat)
{
    float4 result;

    // 行列から回転軸と回転角を抽出
    float trace = mat._11 + mat._22 + mat._33 + 1.0f;
    float epsilon = 0.000001f;

    if (trace > epsilon)
    {
        float s = 0.5f / sqrt(trace);
        result.w = 0.25f / s;
        result.x = (mat._32 - mat._23) * s;
        result.y = (mat._13 - mat._31) * s;
        result.z = (mat._21 - mat._12) * s;
    }
    else
    {
        if (mat._11 > mat._22 && mat._11 > mat._33)
        {
            float s = 2.0f * sqrt(1.0f + mat._11 - mat._22 - mat._33);
            result.w = (mat._32 - mat._23) / s;
            result.x = 0.25f * s;
            result.y = (mat._12 + mat._21) / s;
            result.z = (mat._13 + mat._31) / s;
        }
        else if (mat._22 > mat._33)
        {
            float s = 2.0f * sqrt(1.0f + mat._22 - mat._11 - mat._33);
            result.w = (mat._13 - mat._31) / s;
            result.x = (mat._12 + mat._21) / s;
            result.y = 0.25f * s;
            result.z = (mat._23 + mat._32) / s;
        }
        else
        {
            float s = 2.0f * sqrt(1.0f + mat._33 - mat._11 - mat._22);
            result.w = (mat._21 - mat._12) / s;
            result.x = (mat._13 + mat._31) / s;
            result.y = (mat._23 + mat._32) / s;
            result.z = 0.25f * s;
        }
    }

    return normalize(result);
}

//--------------------------------------------------------------------------------------------------------
// ビルボードクォータニオンを求める関数
float4 QuaternionLookAt(float3 eye, float3 look, float3 upVector)
{
    float3 forward = normalize( look - eye ) ;
    float3 right = cross(upVector, forward);
    float3 up = cross(forward, right);

    float3x3 rotationMatrix = {right, up, forward};
    return QuaternionFromMatrix(rotationMatrix);
}

