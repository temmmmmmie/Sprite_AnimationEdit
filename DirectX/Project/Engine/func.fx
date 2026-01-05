#ifndef _FUNC
#define _FUNC

#include "value.fx"

#define Light2D g_Light2D[_LightIdx]






void CalcLight2D(int _LightIdx, float3 _vWorldPixelPos, inout float3 _LightColor)
{   
    // Directional Light
    if (0 == Light2D.Type)
    {
        _LightColor += Light2D.LightColor;
    }
    
    // Point Light
    else if (1 == Light2D.Type)
    {
        // 거리에 따른 빛의 감쇄 비율
        float DistRatio = 1.f;
        
        float Dist = distance(_vWorldPixelPos.xy, Light2D.WorldPos.xy);       
                
        if (Dist < Light2D.Radius)
        {
            //DistRatio = saturate(1.f - (Dist / Light2D.Radius));
            DistRatio = saturate(cos(Dist * ((PI / 2.f) / Light2D.Radius)));            
            _LightColor += Light2D.LightColor * DistRatio;
        }
    }
    
    // Spot Light
    else
    {
        
    }   
}

void CalcLight2D(float3 _WorldPixelPos, inout float3 _LightColor)
{
    for (int i = 0; i < Light2DCount; ++i)
    {
        CalcLight2D(i, _WorldPixelPos, _LightColor);
    }
}

#endif