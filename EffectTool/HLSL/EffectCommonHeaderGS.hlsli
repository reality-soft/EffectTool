struct GS_IN
{
	float4 p			: SV_POSITION;
	float4 c			: COLOR0;
	float2 t			: TEXCOORD0;
};

struct GS_OUT
{
	float4 p	: SV_POSITION;
	float4 c	: COLOR0;
	float2 t	: TEXCOORD0;
	float2 uv	: TEXCOORD1;
};

cbuffer cb_matrix : register(b0)
{
	float4x4 g_world;
	float4x4 g_view_proj;
}

cbuffer cb_sprite : register(b1)
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
	matrix  g_billboard;
}

cbuffer cb_particle : register(b2)
{
	// x : timer
	// y : frame_ratio
	float4	g_particle_values;
	float4	g_color;
	matrix  g_mat_particle;
}