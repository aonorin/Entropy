//////////////////////////////////////////////////////////////////////////////
// -DLIGHT_TYPE=
#define LIGHT_POINT 		1
#define LIGHT_SPOT			2
#define LIGHT_DIRECTIONAL	3
//////////////////////////////////////////////////////////////////////////////
//#define IS_SPECULAR
//////////////////////////////////////////////////////////////////////////////
//#define BUMP_MAPPING
//////////////////////////////////////////////////////////////////////////////
//#define DERIVED_LIGHT
// light_diffuse_colour = derived_light_diffuse_colour
// light_specular_colour = derived_light_specular_colour
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// @note light_position, camera_position, position etc should be in the same space (using object space)
// need to set surface_specular_colour.w = surface_shinness before GetLit() called.
//////////////////////////////////////////////////////////////////////////////
void GetLit
(
	float4 light_diffuse_colour
#ifdef IS_SPECULAR
	, float4 light_specular_colour
#endif
#if LIGHT_TYPE == LIGHT_POINT || LIGHT_TYPE == LIGHT_SPOT
	, float4 light_attenuation
#endif
#if LIGHT_TYPE == LIGHT_SPOT
	, float4 spotlight_params 
	, float4 light_direction 
#endif

#ifndef DERIVED_LIGHT
		, float4 surface_diffuse_colour 
	#ifdef IS_SPECULAR
		, float4 surface_specular_colour
	#endif
#endif
 
	, float3 light_position
	, float3 camera_position
	, float3 position
	, float3 normal
	
#ifdef BUMP_MAPPING
	, float3 tangent
#endif
	, out float3 total_light_contrib
)
{	
#if LIGHT_TYPE == LIGHT_DIRECTIONAL
	// in a directional light context light position == light direction
	float3 objToLightDir = light_position;
#else
	float3 objToLightVec = light_position - position;
	float len_sq = dot(objToLightVec, objToLightVec);
	float len = sqrt(len_sq);
	float3 objToLightDir = objToLightVec/len;
#endif
	
#ifdef BUMP_MAPPING
	// Calculate the binormal (NB we assume both normal and tangent are already normalised)
	// NB looks like nvidia cross params are BACKWARDS to what you'd expect
	// this equates to NxT, not TxN
	#if TANGENTS_HAVE_PARITY
		float3 binormal = cross(tangent.xyz, normal) * tangent.www;
	#else
		// fixed handedness
		float3 binormal = cross(tangent.xyz, normal);
	#endif
	
	// Form a rotation matrix out of the vectors
	float3x3 rotation = float3x3(tangent.xyz, binormal, normal);
	
	// Transform the light vector according to this matrix
	objToLightDir.xyz = mul(rotation, objToLightDir);
#endif
	
#ifdef DERIVED_LIGHT
	total_light_contrib = max(0.0, dot(normal, objToLightDir)) * light_diffuse_colour.xyz;
#else
	total_light_contrib = max(0.0, dot(normal, objToLightDir)) * light_diffuse_colour.xyz * surface_diffuse_colour.xyz;
#endif
	
#ifdef IS_SPECULAR
	float3 viewDir = normalize(camera_position - position);
	float3 h = normalize(objToLightDir + viewDir);
	
	#ifdef DERIVED_LIGHT
		total_light_contrib += pow(dot(normal, h), light_specular_colour.w) * light_specular_colour.rgb;
	#else
		total_light_contrib += pow(dot(normal, h), surface_specular_colour.w) * light_specular_colour.rgb * surface_specular_colour.xyz;
	#endif
#endif
	
#if LIGHT_TYPE == LIGHT_POINT || LIGHT_TYPE == LIGHT_SPOT
	float attenuation = dot(light_attenuation.yzw, float3(1.0, len, len_sq));
	total_light_contrib /= attenuation;
#endif
	
#if LIGHT_TYPE == LIGHT_SPOT
	float spotlightAngle = saturate(dot(light_direction, -objToLightDir));
	float spotFalloff = saturate((spotlightAngle - spotlight_params.x) / (spotlight_params.y - spotlight_params.x));
	total_light_contrib *= (1-spotFalloff);
#endif
	
}