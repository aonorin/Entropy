struct v2fConnector
{
    float4 pos		: POSITION;
	float4 worldPos	: TEXCOORD0;
};

void main	(	float4 pos			: POSITION,
             
				uniform float4x4 worldviewproj_matrix,
                uniform float4x4 worldview_matrix,

                out v2fConnector v2f
			)
{
	v2f.pos = mul(worldviewproj_matrix, pos);
    v2f.worldPos = mul(worldview_matrix, pos);
}

