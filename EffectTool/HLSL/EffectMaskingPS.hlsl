#include "EffectCommonHeaderPS.hlsli"


float PS(PS_IN input) : SV_Target
{
	float4 tex_color = g_txDiffuse.Sample(g_SampleWrap, input.t);

	float4 uv_color = g_txOpacity.Sample(g_SampleWrap, input.uv);

	float alpha = max(max(uv_color.r, uv_color.g), uv_color.b);

	if (alpha < 0.3f)
	{
		discard;
	}

	return float4(tex_color.rgb * uv_color.rgb, alpha);
}