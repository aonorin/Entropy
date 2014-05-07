#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include "boost/shared_ptr.hpp"

namespace boost
{
    namespace chrono
    {
        class steady_clock; // Forward declaration
    }
}

namespace entropy
{
    typedef char int8;
    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef short int16;
    typedef unsigned int uint32;
    typedef unsigned int uint;
    typedef int int32;
    typedef std::vector<int8> Int8Array_t;
    typedef boost::shared_ptr<Int8Array_t> Int8ArrayPtr_t;
    typedef boost::shared_ptr<std::vector<float> > FloatArrayPtr_t;
    extern boost::chrono::steady_clock clock;
    enum Direction{UP=1,DOWN=2,LEFT=4,RIGHT=8,BACK=16,FORWARD=32};
}

extern int MAPSIZE_X_NONSCALED;
extern int MAPSIZE_Y_NONSCALED;
extern int MAPSIZE_Z_NONSCALED;
extern int MAPSIZE_SCALE;
extern int MAPSIZE_X;
extern int MAPSIZE_Y;
extern int MAPSIZE_Z;
extern int GRAPHICS_SCALE;

#endif // GLOBAL_H
