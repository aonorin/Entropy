struct v2fConnector
{
	float4 position_screen  : POSITION;
	float3 position_world   : TEXCOORD0;
	float2 uv               : TEXCOORD1;
    float3 normal           : TEXCOORD2;

    //float4 positon_shadow_map_0 : TEXCOORD3;
};

void main	(	float4 pos			: POSITION,
                float3 normal		: NORMAL,
                float3 uv           : TEXCOORD0,
             
                uniform float4x4 worldviewproj_matrix,
                uniform float4x4 world_matrix,
             
                uniform float4x4 texture_worldviewproj_matrix,

                out v2fConnector v2f
			)
{
	v2f.position_screen = mul(worldviewproj_matrix, pos);
    v2f.position_world = mul(world_matrix, pos).xyz;
    v2f.uv = uv;
    v2f.normal = normal;

    //v2f.positon_shadow_map_0 = mul(texture_worldviewproj_matrix, pos);
}

