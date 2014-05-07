//------------------------------------------------------------------------------
// Shadow Map Filter(s) Vertex Shader
// -DNUM_TAPS value [1-8]
//------------------------------------------------------------------------------
struct blurVSOutput
{
    float4 Position   : POSITION;
    float2 TexCoord[8]: TEXCOORD0;
};


blurVSOutput GenUV_1D_NTaps_VS  (	float4 Position : POSITION,
                                    float2 TexCoord : TEXCOORD0,
                                    uniform float4 inverse_texture_size,
                                    uniform float2 direction	)
{      
    blurVSOutput output;
    output.Position = Position;
    float2 step = direction * inverse_texture_size.xy;
    float2 base = TexCoord - ((((float)NUM_TAPS - 1)*0.5f) * step);
    
    for (int i = 0; i < NUM_TAPS; i++)
    {
        output.TexCoord[i] = base;
        base += step;
    }
    
    return output;
}


struct BlurPSOutput
{
    float4 depth : COLOR;
};

//------------------------------------------------------------------------------
// Logarithmic filtering
//------------------------------------------------------------------------------

float log_conv ( float x0, float X, float y0, float Y )
{
    return (X + log(x0 + (y0 * exp(Y - X))));
}

//------------------------------------------------------------------------------
// 5 taps separable box filter
//------------------------------------------------------------------------------

BlurPSOutput LogBox1D_5Taps_PS( blurVSOutput input,                               
                               uniform sampler2D tex)						                    
{
    BlurPSOutput output;
    
    float  sample[5];
    for (int i = 0; i < 5; i++)
    {
        sample[i] = tex2D( tex, input.TexCoord[i] ).x;
    }
    
    const float c = (1.f/5.f);    
    
    float accum;
    accum = log_conv( c, sample[0], c, sample[1] );
    for (int i = 2; i < 5; i++)
    {
        accum = log_conv( 1.f, accum, c, sample[i] );
    }    
    
    output.depth = accum;

    return output;
}
