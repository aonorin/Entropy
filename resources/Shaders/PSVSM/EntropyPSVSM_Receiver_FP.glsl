uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform sampler2D diffuseMap0;

uniform vec4 invShadowMapSize0;
uniform vec4 invShadowMapSize1;
uniform vec4 invShadowMapSize2;
uniform vec4 pssmSplitPoints;
uniform vec4 depthRange0;
uniform vec4 depthRange1;
uniform vec4 depthRange2;
uniform vec4 lightPos0;

varying vec3  vertexNormal;
varying vec4  vertexWorldPosition;
varying vec4  lightPosition0;
varying vec4  lightPosition1;
varying vec4  lightPosition2;
varying float shadowDistance;

float shadowPCF(in sampler2D shadowMap, in vec4 shadowMapPos, const in vec2 offset, float depth)
{
   shadowMapPos = shadowMapPos / shadowMapPos.w;
   vec2 uv = shadowMapPos.xy;
    
   vec2 o = offset;
   vec2 c = texture2D(shadowMap, uv.xy).rg; // center
    c += texture2D(shadowMap, uv.xy - o.xy).rg; // top left
    c += texture2D(shadowMap, uv.xy + o.xy).rg; // bottom right
    c += texture2D(shadowMap, vec2(uv.x - o.x, uv.y)).rg; // left
    c += texture2D(shadowMap, vec2(uv.x + o.x, uv.y)).rg; // right
    c += texture2D(shadowMap, vec2(uv.x, uv.y + o.y)).rg; // bottom
    c += texture2D(shadowMap, vec2(uv.x, uv.y - o.y)).rg; // top
    c += texture2D(shadowMap, vec2(uv.x - o.x, uv.y + o.y)).rg; // bottom left
    c += texture2D(shadowMap, vec2(uv.x + o.x, uv.y - o.y)).rg; // top right
    c /= 9.0;
        
    vec2 moments = c;
    float litFactor = (depth <= moments.x ? 1.0 : 0.0);

    // standard variance shadow mapping code
    float E_x2 = moments.y;
    float Ex_2 = moments.x * moments.x;
    float vsmEpsilon = 0.0001;
    float variance = min(max(E_x2 - Ex_2, 0.0) + vsmEpsilon, 1.0);
    float m_d = moments.x - depth;
    float p = variance / (variance + m_d * m_d);

    return smoothstep(0.4, 1.0, max(litFactor, p));
}

void main()
{
    vec3 lightDir0 = normalize(lightPos0.xyz - (lightPos0.w * vertexWorldPosition.xyz));
    vec3 normalizedNormal   = normalize(vertexNormal);
    float diffuseAmount = max(dot(normalizedNormal, lightDir0), 0.0);
        
   // calculate shadow
   float shadowAmount = 1.0;
    vec4 shadowColor = vec4(0.0, 0.0, 0.0, 1.0);
    
   if (shadowDistance <= pssmSplitPoints.y) {
        float depth = (length(lightPos0.xyz - vertexWorldPosition.xyz) - depthRange0.x) * depthRange0.w;

      shadowAmount = shadowPCF(shadowMap0, lightPosition0, invShadowMapSize0.xy, depth);
        shadowColor = vec4(1.0, 0, 0, 1.0);
   }
   else if (shadowDistance <= pssmSplitPoints.z) {
        float depth = (length(lightPos0.xyz - vertexWorldPosition.xyz) - depthRange1.x) * depthRange1.w;

      shadowAmount = shadowPCF(shadowMap1, lightPosition1, invShadowMapSize1.xy, depth);
        shadowColor = vec4(0.0, 1.0, 0, 1.0);        
   }
   else {
        float depth = (length(lightPos0.xyz - vertexWorldPosition.xyz) - depthRange2.x) * depthRange2.w;

      shadowAmount = shadowPCF(shadowMap2, lightPosition2, invShadowMapSize2.xy, depth);
        shadowColor = vec4(0.0, 0.0, 1.0, 1.0);        
   }
    
    const float shadowAmbient = 0.9;

    // if you do your own ambient pass you'll want to remove the ambient code from here
    const vec4 ambientColor = vec4 (0.05, 0.075, 0.1, 1.0);
    vec4 diffuseColor =  texture2D(diffuseMap0, gl_TexCoord[0].st);

    gl_FragColor = ((1.0 - shadowAmbient) * diffuseColor * shadowColor) + 
                         (diffuseColor * shadowAmbient * diffuseAmount * shadowAmount * shadowColor);
}