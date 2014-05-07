/*
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

varying vec3 N;
varying vec3 v;
void main (void)
{
   vec3 L = normalize(gl_LightSource[0].position.xyz - v);
   vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)

   vec3 R = normalize(-reflect(L,N)+ (sin(v*0.05 + sin(reflect(L,N)*0.5))*0.1));

   //calculate Ambient Term:
   vec4 Iamb = gl_FrontLightProduct[0].ambient;

   //calculate Diffuse Term:
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);

   Idiff = clamp(Idiff, 0.0, 1.0);

   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[0].specular
           * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);

   Ispec = clamp(Ispec, 0.0, 1.0);

   // write Total Color:
   gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
}
*/

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform sampler2D occlusionTexture;

// New bumpmapping
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;


void main()
{

    // lookup normal from normal map, move from [0,1] to  [-1, 1] range, normalize
    vec3 normal = 2.0 * texture2D (normalTexture, gl_TexCoord[0].st).rgb - 1.0;
    normal = normalize (normal);

    // compute diffuse lighting
    float lamberFactor= max (dot (lightVec, normal), 0.0);
    vec4 diffuseMaterial =  vec4(0.0,0.0,0.0,0.0);
    vec4 diffuseLight  = vec4(0.0,0.0,0.0,0.0);

    // compute specular lighting
    vec4 specularMaterial = vec4(0.0,0.0,0.0,0.0) ;
    vec4 specularLight = vec4(0.0,0.0,0.0,0.0);
    float shininess ;

    vec4 diffuseColor;
    vec4 specularColor;
    vec4 outputColor;

    // compute ambient
    vec4 ambientLight = gl_LightSource[0].ambient;

    if (lamberFactor > 0.0)
    {
        diffuseMaterial = texture2D (diffuseTexture, gl_TexCoord[0].st);
        diffuseLight  = gl_LightSource[0].diffuse;

        // In doom3, specular value comes from a texture
        specularMaterial = texture2D (specularTexture, gl_TexCoord[0].st);
        specularMaterial = normalize(specularMaterial);
        specularLight = gl_LightSource[0].specular;
        shininess = pow (max (dot (halfVec, normal), 0.0), 4.0) ;

        diffuseColor =	diffuseMaterial * diffuseLight * lamberFactor;
        specularColor = (specularMaterial * specularLight * shininess);
        gl_FragColor = specularColor+diffuseColor ;
        gl_FragColor *= texture2D(occlusionTexture, gl_TexCoord[0].st);
    }

    gl_FragColor += ambientLight;
}
