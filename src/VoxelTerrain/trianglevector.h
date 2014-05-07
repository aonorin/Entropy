#ifndef TRIANGLEVECTOR_H
#define TRIANGLEVECTOR_H

#include "OGRE/OgreVector3.h"
#include "OGRE/OgrePlane.h"
#include "Line.h"
#include "stdlib.h"

namespace voxel {

struct triangleVector3
{
    triangleVector3()
    {
        ind[0] = Ogre::Vector3(0,0,0);
        ind[1] = Ogre::Vector3(0,0,0);
        ind[2] = Ogre::Vector3(0,0,0);
    }
    triangleVector3(Ogre::Vector3 a, Ogre::Vector3 b, Ogre::Vector3 c)
    {
        ind[0] = a;
        ind[1] = b;
        ind[2] = c;
    }

    bool operator == (const triangleVector3 & other) const
    {
        return other.ind[0] == ind[0]
                && other.ind[1] == ind[1]
                && other.ind[2] == ind[2];
    }

    Ogre::Plane getPlane()
    {
        return Ogre::Plane(ind[0], ind[1], ind[2]);
    }

    Ogre::Vector3 midPoint()
    {
        return (ind[0] + ind[1] + ind[2]) / 3.0;

        line3 ln1(ind[0], ind[1] - ind[0]);
        line3 ln2(ind[1], ind[2] - ind[1]);
        line3 ln3(ln1.getPoint(0.5), ind[2] - ln1.getPoint(0.5));
        line3 ln4(ln2.getPoint(0.5), ind[0] - ln2.getPoint(0.5));
        Ogre::Vector3 mid;
        if (ln3.intersects(ln4, &mid))
        {
            return mid;
        }
        else
//            BERROR("FATAL ln3.intersects(ln4, &mid");
            std::cout << "FATAL ln3.intersects(ln4, &mid)" << std::endl;
        return ind[0];
    }

    Ogre::Vector3 ind[3];
};


}

#endif // TRIANGLEVECTOR_H
