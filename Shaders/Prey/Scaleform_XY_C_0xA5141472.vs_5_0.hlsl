#include "Includes/Common.hlsl"
#include "Includes/Scaleform.hlsl"
#include "../Includes/LensDistortion.hlsl"

#include "Includes/CBuffer_PerViewGlobal.hlsl"

cbuffer PER_BATCH : register(b0)
{
  row_major float4x4 cCompositeMat : packoffset(c0);
}

void main(
  int4 v0 : POSITION0,
  float4 v1 : COLOR0,
  out float4 o0 : SV_Position0,
  out float4 o1 : COLOR0)
{
  float4 r0;
  r0.xy = (int2)v0.xy;
  r0.z = 1;
  o0.x = dot(cCompositeMat._m00_m01_m03, r0.xyz);
  o0.y = dot(cCompositeMat._m10_m11_m13, r0.xyz);
  o0.z = dot(cCompositeMat._m20_m21_m23, r0.xyz);
  o0.w = dot(cCompositeMat._m30_m31_m33, r0.xyz);
  o1.xyzw = v1.xyzw;

  // LUMA FT: There shouldn't be any other vertices that draw black on edge vertices (hopefully...)
  // The vertices borders check is a bit random, I'm not sure about the math, but it should be safe enough.
  bool isBlackBar = all(o1.rgb == 0) && ((abs(o0.x) > 1.0 - FLT_EPSILON) || (abs(o0.y) > 1.0 - FLT_EPSILON)) && o0.z == 1 && o0.w == 1;
  
#if ENABLE_SCREEN_DISTORTION
  // Inverse lens distortion
  if (LumaUIData.TargetingSwapchain && !LumaUIData.FullscreenMenu && LumaSettings.GameSettings.LensDistortion && isLinearProjectionMatrix(cCompositeMat) && !isBlackBar) // Workaround to disable shifting invisible black bars (flattened at the edge) with lens distortion
  {
    o0.xyz /= o0.w; // From clip to NDC space
    o0.w = 1; // no need to convert it back to clip space, the GPU would do it again anyway
    o0.y = -o0.y; // Adapt to normal NDC coordinates
    o0.xy = PerfectPerspectiveLensDistortion_Inverse(o0.xy, 1.0 / CV_ProjRatio.z, CV_ScreenSize.xy, true);
    o0.y = -o0.y;
  }
#endif

#if 0 // Option to disable black bars on borders with menu at aspect ratios below 16:9 (if ever needed)
  if (isBlackBar)
  {
    o1.a == 0;
  }
#endif
}