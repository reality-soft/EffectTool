#include "EffectHeader.hlsli"

cbuffer cb_data : register(b0)
{
	matrix g_matWorld;
};

cbuffer cb_viewproj : register(b1)
{
	matrix g_matView;
	matrix g_matProj;
}

cbuffer cb_sprite : register(b2)
{
	int		g_sprite_type;
	int		g_max_frame;
	int		g_uv_list_size;
	int		padding;
	int		padding2;
	float	g_start_u[255];
	float	g_start_v[255];
	float	g_end_u[255];
	float	g_end_v[255];
}

cbuffer cb_particle : register(b3)
{
	float4	g_vel;
	float4	g_size;
	float4	g_color;
	float	g_rotation;
	float	g_timer;
}

VS_OUT VS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	float4 vLocal = float4(input.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	vWorld += g_vel * g_timer;
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);

	output.p = vProj;
	output.n = input.n;
	output.c = input.c;

	int frame = (int)g_timer % g_uv_list_size;

	if (input.t.x == 0.0f && input.t.y == 0.0f)
	{
		output.t.x = g_start_u[frame];
		output.t.y = g_start_v[frame];
	}
	else if (input.t.x == 1.0f && input.t.y == 0.0f)
	{
		output.t.x = g_end_u[frame];
		output.t.y = g_start_v[frame];
	}
	else if (input.t.x == 0.0f && input.t.y == 1.0f)
	{
		output.t.x = g_start_u[frame];
		output.t.y = g_end_v[frame];
	}
	else if (input.t.x == 1.0f && input.t.y == 1.0f)
	{
		output.t.x = g_end_u[frame];
		output.t.y = g_end_v[frame];
	}

	return output;
}