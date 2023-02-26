#include "EffectHeader.hlsli"


cbuffer cb_light : register(b0)
{
	float4 default_light;
	float light_bright;
}

Texture2D    g_txTex			: register(t0);
SamplerState g_SampleWrap		: register(s0);

float4 PS(VS_OUT input) : SV_Target
{
	// Tex
	float4 tex_color = g_txTex.Sample(g_SampleWrap, input.t);
	return tex_color;
}