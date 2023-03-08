#include "EffectCommonHeaderPS.hlsli"

float4 PS(PS_IN input) : SV_Target
{
	// Tex
	float4 tex_color = g_txOpacity.Sample(g_SampleWrap, input.uv);

	float mask = min(min(tex_color.r, tex_color.g), tex_color.b);

	if (mask > 0.9f)
	{
		discard;
	}

	return float4(tex_color.rgb * input.c.rgb, (1 - mask) * input.c.a);
}