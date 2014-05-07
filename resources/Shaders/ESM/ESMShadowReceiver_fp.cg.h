//////////////////////////////////////////////////////////////////////////////
// Light Types
// -DLIGHT_TYPE=
//////////////////////////////////////////////////////////////////////////////
#define LIGHT_POINT 		1
#define LIGHT_SPOT			2
#define LIGHT_DIRECTIONAL	3
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
#define LIGHT_TYPE LIGHT_SPOT
// Since using derived_light_diffuse_colour and derived_light_specular_colour
#define DERIVED_LIGHT	
#include "Phong.cg.h"
//////////////////////////////////////////////////////////////////////////////


struct v2fConnector
{
	float4 position_screen  : POSITION;
	float3 position_world   : TEXCOORD0;
	float2 uv               : TEXCOORD1;
    float3 normal           : TEXCOORD2;
    //float4 positon_shadow_map_0 : TEXCOORD3;
};


float4 main	(	v2fConnector v2f,
                
             
                uniform float4 light_position,
             
                uniform float depth_scale,
                uniform float over_darkening_factor,
                uniform float light_shadow_bias,
             
                uniform float4x4 texture_viewproj_matrix0,
                uniform float4 texture_size,
             
                uniform sampler2D shadowMap0 : register(s0)

             
            // Additional params for spotlight
            #if LIGHT_TYPE == LIGHT_SPOT
                , uniform float4 light_attenuation
                , uniform float4 spotlight_params
                , uniform float4 light_direction
            #endif
                , uniform float4 derived_light_diffuse_colour
                , uniform float3 camera_position
                , uniform sampler2D diffuseMap : register(s1)

                
			 ) : COLOR
{
    
    // compute distance from current fragment to the main light
    float3 fragment_to_light = light_position.xyz - v2f.position_world;
    float light_distance = length(fragment_to_light);
    
    // rescale light distance and apply bias
    float receiver = depth_scale * light_distance - light_shadow_bias;
    
    // compute shadow map sampling coordinates
    float4 surf_tex = mul(texture_viewproj_matrix0, float4(v2f.position_world, 1.0));
    //float4 surf_tex = v2f.positon_shadow_map_0; 
    
    //float2 shadow_tex = (surf_tex.xy / surf_tex.ww) * float2(0.5, -0.5) + 0.5; // For DX
    float2 shadow_tex = surf_tex.xy/surf_tex.w;
    
    // emulate bilinear filtering (not needed if your GPU can filter FP32 textures)
    float2 unnormalized = shadow_tex * texture_size.x;
    float2 fractional = frac(unnormalized); 
    unnormalized = floor(unnormalized);
    
    float4 exponent;                   
    exponent.x = tex2D(shadowMap0, (unnormalized + float2( -0.5f, 0.5f ))/ texture_size.x ).x;
    exponent.y = tex2D(shadowMap0, (unnormalized + float2( 0.5f, 0.5f ))/ texture_size.x ).x;
    exponent.z = tex2D(shadowMap0, (unnormalized + float2( 0.5f, -0.5f ))/ texture_size.x ).x;
    exponent.w = tex2D(shadowMap0, (unnormalized + float2( -0.5f, -0.5f ))/ texture_size.x ).x;
    
    
    float occlusion;
    /*
    // Filtering mode 0 -> compute occlusion on four samples and filter them together (sort of PCF..)
    if ( filtering_mode == 0 )
    {
        float4 occlusion4 = exp( over_darkening_factor.xxxx * (exponent - receiver.xxxx) );    
        occlusion = (occlusion4.w + (occlusion4.x - occlusion4.w) * fractional.y);
        occlusion = occlusion + ((occlusion4.z + (occlusion4.y - occlusion4.z) * fractional.y) - occlusion)*fractional.x;     
        return occlusion;
    }
    // Filtering mode 1 -> filter depth and compute occlusion (looks generally better and it's cheaper too)
    else
    */
    {
        float4 occluder4 = exponent;       
        float occluder = (occluder4.w + (occluder4.x - occluder4.w) * fractional.y);
        occluder = occluder + ((occluder4.z + (occluder4.y - occluder4.z) * fractional.y) - occluder)*fractional.x;    
        
        occlusion = exp(over_darkening_factor * ( occluder - receiver ));             
    }    
    //return occlusion;
    
    
    
    
    float3 total_light_contrib;
    
    GetLit	(  derived_light_diffuse_colour
             , light_attenuation
             , spotlight_params
             , light_direction  // might have to normalise this
             , light_position.xyz
             , camera_position
             , v2f.position_world.xyz
             , v2f.normal
             , total_light_contrib);
    
    float4 diffuse = tex2D(diffuseMap, v2f.uv.xy);
    return float4(diffuse.xyz * total_light_contrib * saturate(occlusion), diffuse.w) + // diffuse term 
                  float4(diffuse.xyz * 0.3, 0.0); // ambient term;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
