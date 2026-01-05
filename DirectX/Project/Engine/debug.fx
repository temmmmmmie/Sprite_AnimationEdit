#ifndef _DEBUG
#define _DEBUG

#include "value.fx"

struct VS_IN
{
    float2 vUV : TEXCOORD; // Sementic
    float3 vPos : POSITION; // Sementic
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Debug(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Debug(VS_OUT _in) : SV_Target
{    
    //float Thickness = 0.02f;
    
    //if (Thickness < _in.vUV.x && _in.vUV.x < (1.f - Thickness)
    //    && Thickness < _in.vUV.y && _in.vUV.y < 1.f - Thickness)
    //{
    //    discard;
    //}
    
    return g_vec4_0;
}

#endif