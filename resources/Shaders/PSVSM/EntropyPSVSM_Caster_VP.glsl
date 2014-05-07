uniform mat4 wvMat;

varying vec4 vertexDepth;

void main()
{
    vertexDepth = wvMat * gl_Vertex;
    
    // pass the vertex position, transforming it to clip space
   gl_Position = ftransform();
}