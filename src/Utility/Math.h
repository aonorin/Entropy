#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <cstdlib>

namespace utility {

namespace math {

extern double TWO_PI;
extern double PI;
extern double HALF_PI;
extern double ONE_FOURTH_PI;
extern double TWO_FIFTHS_PI;
extern double THREE_FOURTHS_PI;
extern double SQUARE_ROOT_ZERO_POINT_FIVE;
extern double SQUARE_ROOT_ZERO_POINT_SEVEN_FIVE;


inline double mod(double x, double y)
{
    if(0 == y)
    {
        return x;
    }

    return x - y * floor(x / y);
}

inline double wrapPosNegPI(double angle)
{
    return mod(angle + PI, TWO_PI) - PI;
}

inline double wrapTwoPI(double angle)
{
    return mod(angle, TWO_PI);
}

inline double wrap360(double angle)
{
    return mod(angle, 360);
}

// Map a number from a linear range to an exponential range
inline double linexp(double val, double inMin, double inMax, double outMin, double outMax)
{
    if(val <= outMin)
        return val;
    else if(val >= outMax)
        return val;
    else
        return pow(outMax / outMin, (val - inMin) / (inMax - inMin)) * outMin;
}


inline int randomRange(int minValue, int maxValue)
{
    return minValue + (rand() % (maxValue - minValue + 1));
}

inline double cubicInterpolate (double p[4], double x) {
    return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

inline double bicubicInterpolate (double p[4][4], double x, double y) {
    double arr[4];
    arr[0] = cubicInterpolate(p[0], y);
    arr[1] = cubicInterpolate(p[1], y);
    arr[2] = cubicInterpolate(p[2], y);
    arr[3] = cubicInterpolate(p[3], y);
    return cubicInterpolate(arr, x);
}

inline double tricubicInterpolate (double p[4][4][4], double x, double y, double z)
{
    double arr[4];
    arr[0] = bicubicInterpolate(p[0], y, z);
    arr[1] = bicubicInterpolate(p[1], y, z);
    arr[2] = bicubicInterpolate(p[2], y, z);
    arr[3] = bicubicInterpolate(p[3], y, z);
    return cubicInterpolate(arr, x);
}

inline double cosineInterpolate(double p[2],double x)
{
   double mu2;
   mu2 = (1-cos(x*PI))/2;
   return(p[0]*(1-mu2)+p[1]*mu2);
}

inline double bicosineInterpolate(double p[2][2], double x, double y)
{
    double p2[2];
    p2[0] = cosineInterpolate(p[0],y);
    p2[1] = cosineInterpolate(p[1],y);
    return cosineInterpolate(p2,x);
}

inline double triCosineInterpolate(double p[2][2][2],double x, double y, double z)
{
    double p2[2];
    p2[0] = bicosineInterpolate(p[0],y,z);
    p2[1] = bicosineInterpolate(p[1],y,z);
    return cosineInterpolate(p2,x);
}

inline double easeOutCubic(double t,double b,double c,double d)
{
    t /= d;
    t--;
    return c*(t*t*t + 1) + b;
}

/*
double nCubicInterpolate (int n, double* p, double coordinates[]) {
//    assert(n > 0);
    if (n == 1) {
        return cubicInterpolate(p, *coordinates);
    }
    else {
        double arr[4];
        int skip = 1 << (n - 1) * 2;
        arr[0] = nCubicInterpolate(n - 1, p, coordinates + 1);
        arr[1] = nCubicInterpolate(n - 1, p + skip, coordinates + 1);
        arr[2] = nCubicInterpolate(n - 1, p + 2*skip, coordinates + 1);
        arr[3] = nCubicInterpolate(n - 1, p + 3*skip, coordinates + 1);
        return cubicInterpolate(arr, *coordinates);
    }
}
*/

} // math namespace

} // utility namespace


#endif // MATH_H
