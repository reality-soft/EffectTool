struct VS_IN
{
	float3 p : F3_POSITION;
	float3 n : F3_NORMAL;
	float2 t : F2_TEXTURE;
};
struct VS_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float2 t : TEXCOORD0;
};

cbuffer CbCameraInfo : register(b0)
{
	matrix view_proj;
	matrix camera_translation;
	float4 camera_world;
	float4 camera_look;
};

cbuffer cb_transform : register(b1)
{
	matrix g_matWorld;
}


VS_OUT VS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	float4 vLocal = float4(input.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vProj = mul(vWorld, view_proj);
	//float4 vView = mul(vWorld, g_matView);
	//float4 vProj = mul(vView, g_matProj);

	output.p = vProj;
	output.n = input.n;
	output.t = input.t;

	return output;
}