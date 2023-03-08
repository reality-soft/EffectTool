#include "EffectCommonHeaderPS.hlsli"

float4 PS(PS_IN input) : SV_Target
{
	// Tex
	float4 tex_color = g_txOpacity.Sample(g_SampleWrap, input.uv);
	float4 final_color = tex_color * input.c;
	return float4(final_color.rgb * 2.0f, final_color.a);
}