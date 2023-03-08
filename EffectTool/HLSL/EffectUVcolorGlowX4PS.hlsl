#include "EffectCommonHeaderPS.hlsli"

float4 PS(PS_IN input) : SV_Target
{
	// Tex
	float4 tex_color = g_txOpacity.Sample(g_SampleWrap, input.uv);

	float alpha = max(max(tex_color.r, tex_color.g), tex_color.b);

	if (alpha < 0.1f)
	{
		discard;
	}
	float4 final_color = float4(tex_color.rgb * input.c.rgb, alpha * input.c.a);
	return float4(final_color.rgb * 4.0f, final_color.a);
}