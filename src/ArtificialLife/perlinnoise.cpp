#include "perlinnoise.h"

namespace alife
{

Perlin::Perlin(uint16 width, uint16 height, uint16 depth, uint16 m, bool preComputeTable) :
    width(width),
    height(height),
    depth(depth),
    m(m),
    n(3),
    preComputeTable(preComputeTable),
    noise(new std::vector<float>()),
    dist(0.0,1.0)
{
    if(preComputeTable)
    {
        seed();
        computeEasingTable();
    }
}

void Perlin::calculateIndex(uint x, uint y, uint z)
{
    // Temporary results are stored on the following stack
    float stackbuf[3+1];
    float* stack = stackbuf - 1;

    // Precomputation buffers of the hypercube within which we'll interpolate
    float hyperCubeBuf[1<<3];
    float* hyperCube = hyperCubeBuf + (1<<3);

    // Variables used to control whether an interpolation must be performed
    uint looplogic;
    // Our interpolation is based on the coordinate in the current dimension
    uint *curdim;
    // And on the current dimension on which we are operating.
    // bit0 = dim-1, bit1 = dim-2, bit n-1 = dim n
    // When a bit carries, then an interpolation can be performed
    uint  dimIt;

    // Calculate the value of the vertex of the N dimensional cube
    // We implemented this by looking up in N 1d noise vectors and
    // then averaging. This is the killer since we are doing mem
    // lookups into 64kb arrays, so we're precomputing: (Note loop termination occurs when the unsigned variable underflows)
    for((unsigned)dimIt = (1<<n) - 1; dimIt < (1<<n) ; --dimIt)
    {
        *--hyperCube = 0;
        curdim = coords;
        for(uint i = 0; i < N; ++i)
        {
            *hyperCube += dim(i)[(*curdim>>m) + ((dimIt>>i)&1)];
            ++curdim;
        }
        *hyperCube /= n;
    }
}

// Performs the interpolation in one dimension
inline float Perlin::interpolate(float flow, float fhi, uint adj)
{
    float offs = ease[adj];
    return (1.0f - offs) * flow + offs * fhi;
}

void Perlin::computeEasingTable()
{
    int size = pow((float)2,(float)16);
    ease.resize(size);
    for(uint i=0;i<size;++i)
    {
        ease.push_back(computeEase(((float)(float)i/(float)size)));
    }
}

void Perlin::seed()
{
    noise->reserve(width*height*depth);
    for(uint16 x=0;x<width;++x)
    {
        for(uint16 y=0;y<height;++y)
        {
            for(uint16 z=0;z<depth;++z)
            {
                noise->push_back(rng());
            }
        }
    }
}

float Perlin::getValue(uint16 x, uint16 y, uint16 z)
{
    int oneDpos = x + width  * (y + depth * z);
    return noise->at(oneDpos);
}

FloatVectorPtr_t Perlin::dim(uint n)
{
    if(n==0)
    {
        return noiseX;
    }
    else if(n==1)
    {
        return noiseY;
    }
    else
    {
        return noiseZ;
    }
}

}
