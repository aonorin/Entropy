uniform vec4 depthRange;

varying vec4 vertexDepth;

void main()
{
    // derive a per-pixel depth and depth squared
    // (length of the view space position == distance from camera)
    // (this is linear space, not post-projection quadratic space)
    float d = (length(vertexDepth.xyz) - depthRange.x) * depthRange.w;
    
    gl_FragColor = vec4(d, d * d, 1, 1);
}