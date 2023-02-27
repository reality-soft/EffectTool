struct PS_IN
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

cbuffer cb_light : register(b0)
{
	float4 default_light;
	float light_bright;
}

Texture2D    g_txDiffuse			: register(t0);
Texture2D    g_txNormalMap			: register(t1);
Texture2D    g_txMetalic			: register(t2);
Texture2D    g_txRoughness			: register(t3);
Texture2D    g_txSepcular			: register(t4);
Texture2D    g_txAmbient			: register(t5);
Texture2D    g_txOpacity			: register(t6);

SamplerState g_SampleWrap		: register(s0);

float4 PS(PS_IN input) : SV_Target
{
	return input.c;
}