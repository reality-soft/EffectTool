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
	//int		g_sprite_type;
	//int		g_max_frame;
	//int		g_uv_list_size;
	//int		padding;
	int4	g_sprite_values;
	//float		g_start_u[255];
	//float		g_start_v[255];
	//float		g_end_u[255];
	//float		g_end_v[255];
	float4  g_sprite_values2[255];
	matrix g_billboard;
}

cbuffer cb_particle : register(b3)
{
	// x : timer
	// y : frame_ratio
	float4	g_particle_values;
	float4	g_color;
	matrix  g_mat_particle;
}

VS_OUT VS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	float4 vLocal = float4(input.p, 1.0f);
	matrix final_matrix = mul(g_matWorld, g_mat_particle);
	float4 vWorld = mul(vLocal, final_matrix);
	float4 vBill = mul(vWorld, g_billboard);
	float4 vView = mul(vBill, g_matView);
	float4 vProj = mul(vView, g_matProj);

	output.p = vProj;
	output.n = input.n;
	output.c = input.c * g_color;

	if (g_sprite_values.x == 0)
	{
		int frame = (int)(g_sprite_values.z * g_particle_values.y) - 1;

		if (input.t.x == 0.0f && input.t.y == 0.0f)
		{
			output.t.x = g_sprite_values2[frame].x;
			output.t.y = g_sprite_values2[frame].y;
		}
		else if (input.t.x == 1.0f && input.t.y == 0.0f)
		{
			output.t.x = g_sprite_values2[frame].z;
			output.t.y = g_sprite_values2[frame].y;
		}
		else if (input.t.x == 0.0f && input.t.y == 1.0f)
		{
			output.t.x = g_sprite_values2[frame].x;
			output.t.y = g_sprite_values2[frame].w;
		}
		else if (input.t.x == 1.0f && input.t.y == 1.0f)
		{
			output.t.x = g_sprite_values2[frame].z;
			output.t.y = g_sprite_values2[frame].w;
		}
	}
	
	return output;
}