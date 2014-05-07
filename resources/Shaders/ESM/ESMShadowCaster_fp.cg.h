struct v2fConnector
{
    float4 pos		: POSITION;
	float4 worldPos	: TEXCOORD0;
};


float4 main	(	v2fConnector v2f,
                
                uniform float depth_scale

			 ) : COLOR
{
    return length(v2f.worldPos.xyz) * depth_scale;
}
