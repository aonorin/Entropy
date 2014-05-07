#ifndef DIAMONDSQUARED_H
#define DIAMONDSQUARED_H

#include <vector>
#include "boost/shared_ptr.hpp"

#ifndef WIN32
#include "tr1/random"
#else
#include <random>
#include <time.h>
#endif
#include "Core/global.h"

//typedef unsigned int uint32;

namespace alife {

class DiamondSquared
{
public:
    typedef boost::shared_ptr<DiamondSquared> ptr;

    struct Sample {
        entropy::uint32 x;
        entropy::uint32 y;
        double value;
    };
    typedef std::vector<double> SampleVector_t;

    DiamondSquared(entropy::uint32 width = 512, entropy::uint32 height = 512, entropy::uint32 featureSize = 16);
//    ~DiamondSquared();

    void generate();
    entropy::uint32 getWidth();
    entropy::uint32 getHeight();
    double getSample(int x, int y);
    double getBicubicSample(int x, int y);

protected:
    entropy::uint32 width;
    entropy::uint32 height;
    entropy::uint32 featureSize;
//    uint32 scale;

    SampleVector_t samples;

    inline void diamond(int x,int y, entropy::uint32 stepSize, double value);
    inline void square(int x,int y, entropy::uint32 stepSize, double value);

    inline void setSample(int x, int y, double value);
    inline void update(int stepsize, double scale);

    //random number generation
    std::tr1::mt19937 rng;
    std::tr1::normal_distribution<float> range;
#ifndef WIN32
    std::tr1::variate_generator< std::tr1::mt19937, std::tr1::normal_distribution<float> > frand;
#else
    float frand()
    {
        return (std::rand()%1000000)*0.0000001;
    }

#endif
};

}
#endif // DIAMONDSQUARED_H
