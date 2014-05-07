#ifndef TRIANGLE_H
#define TRIANGLE_H

// Bullet includes
#include "LinearMath/btVector3.h"

// Local includes
#include "src/Utility/Vector.h"

namespace geometry {

struct Triangle
{
    Vec3f v0, v1, v2;


    Triangle(Vec3f v0, Vec3f v1, Vec3f v2) : v0(v0), v1(v1), v2(v2) {}

    /*
    Triangle(btVector3 v0, btVector3 v1, btVector3 v2) :
        v0(v0.x(), v0.y(), v0.z()),
        v1(v1.x(), v1.y(), v1.z()),
        v2(v2.x(), v2.y(), v2.z())
    {

    }
    */

    inline Vec3f operator[](int index)
    {
        switch(index % 3)
        {
        case 0:
            return v0;
        case 1:
            return v1;
        case 2:
            return v2;
        }
    }


};

} // geometry namesapce

#endif // TRIANGLE_H
