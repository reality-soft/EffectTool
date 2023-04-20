struct PS_IN
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float2 t : TEXCOORD0;
};

Texture2D textures[7] : register(t0);
SamplerState samper_state : register(s0);

float4 PS(PS_IN input) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}