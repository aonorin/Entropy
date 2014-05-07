#include "diamondsquared.h"
#include "Utility/Math.h"

namespace alife {

DiamondSquared::DiamondSquared(entropy::uint32 width, entropy::uint32 height, entropy::uint32 featureSize) :
    width(width),
    height(height),
    featureSize(featureSize),
    range(-1,1)
#ifndef WIN32
    ,frand(rng,range)
#endif
{
    samples.reserve(width*height);
    for(entropy::uint32 x=0;x<width;x++)
    {
        for(entropy::uint32 y=0;y<height;y++)
        {
            samples.push_back(frand());
        }
    }
}

void DiamondSquared::generate()
{
    double scale = 1.0;
    int sampleSize = featureSize;

    while (sampleSize < 1)
    {
        update(sampleSize, scale);

        sampleSize = sampleSize/2;
        scale = scale/2.0;
    }
}

void DiamondSquared::update(int stepSize, double scale)
{
    int halfStep = stepSize / 2;

    for (int y = halfStep; y < height + halfStep; y += stepSize)
    {
        for (int x = halfStep; x < width + halfStep; x += stepSize)
        {
            square(x, y, stepSize, frand() * scale);
        }
    }

    for (int y = 0; y < height; y += stepSize)
    {
        for (int x = 0; x < width; x += stepSize)
        {
            diamond(x + halfStep, y, stepSize, frand() * scale);
            diamond(x, y + halfStep, stepSize, frand() * scale);
        }
    }
}

void DiamondSquared::diamond(int x, int y, entropy::uint32 stepSize, double value)
{
    int h = stepSize / 2;

    double a = getSample(x - h, y);
    double b = getSample(x + h, y);
    double c = getSample(x, y - h);
    double d = getSample(x, y + h);

    setSample(x, y, ((a + b + c + d) / 4.0) + value);
}

void DiamondSquared::square(int x, int y, entropy::uint32 stepSize, double value)
{
    int h = stepSize / 2;

    double a = getSample(x - h, y - h);
    double b = getSample(x + h, y - h);
    double c = getSample(x - h, y + h);
    double d = getSample(x + h, y + h);

    setSample(x, y, ((a + b + c + d) / 4.0) + value);
}

void DiamondSquared::setSample(int x, int y, double value)
{
    samples.at((x%(width-1)) + (y%(height-1)) * width) = value;
}

double DiamondSquared::getSample(int x, int y)
{
    return samples.at( (x%(width-1)) + (y%(height-1)) * width);
}

double DiamondSquared::getBicubicSample(int x, int y)
{
    double bicube[4][4];
    for(int xx=x-2;xx<x+2;xx++) {
        for(int yy=y-2;yy<y+2;yy++) {
            bicube[xx-x+2][yy-y+2] = getSample(xx,yy);
        }
    }
   return utility::math::bicubicInterpolate(bicube,0.5,0.5);
}

entropy::uint32 DiamondSquared::getWidth()
{
    return width;
}

entropy::uint32 DiamondSquared::getHeight()
{
    return height;
}

}
