#include "EffectCommonHeaderPS.hlsli"

struct PS_OUT
{
	float4 a : SV_TARGET0;
	float4 b : SV_TARGET1;
};

PS_OUT PS(PS_IN input) : SV_Target
{
	PS_OUT vOut;

	float4 uv_color = g_txOpacity.Sample(g_SampleWrap, input.uv);
	float alpha = max(max(uv_color.r, uv_color.g), uv_color.b);
	vOut.b = float4(1.0f - alpha, 1.0f - alpha, 1.0f - alpha, 1.0f);
	vOut.a = uv_color * input.c;

	return vOut;
}