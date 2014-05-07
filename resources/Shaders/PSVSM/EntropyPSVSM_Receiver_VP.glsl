uniform vec4 lightPosition;           // world space
uniform mat4 worldViewProjMatrix;
uniform mat4 worldMatrix;
uniform mat4 texViewProjMatrix0;
uniform mat4 texViewProjMatrix1;
uniform mat4 texViewProjMatrix2;

uniform mat4 worldInverseTranspose; 

varying vec4 lightPosition0;
varying vec4 lightPosition1;
varying vec4 lightPosition2;
varying vec3 vertexNormal;
varying vec4  vertexWorldPosition;
varying float shadowDistance;

void main()
{
    // get normal in world space
    vertexNormal = vec3(worldMatrix * vec4(normalize(gl_Normal.xyz), 0.0));
    
    // pass the vertex position, transforming it to clip space
   gl_Position = ftransform();

   // pass texture co-ords through unchanged
   gl_TexCoord[0] = gl_MultiTexCoord0;
    
    // shadowDistance is in clip space
   shadowDistance = gl_Position.z;
    
    // get vertex position in world space
    vertexWorldPosition = worldMatrix * gl_Vertex;
    
   // Calculate the position of vertex in light space for shadowing
   lightPosition0 = texViewProjMatrix0 * vertexWorldPosition;
   lightPosition1 = texViewProjMatrix1 * vertexWorldPosition;
   lightPosition2 = texViewProjMatrix2 * vertexWorldPosition;
}