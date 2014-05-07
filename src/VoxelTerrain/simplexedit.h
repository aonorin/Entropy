#ifndef SIMPLEXEDIT_H
#define SIMPLEXEDIT_H

#include "algoedit.h"

namespace voxel
{

class SimplexEdit : public AlgoEdit
{
public:
    typedef boost::shared_ptr<SimplexEdit> ptr;
    SimplexEdit(Ogre::Vector3 dimensions, Ogre::Vector3 pos, int seed);

    double calcVoxel(const Ogre::Vector3 &pos, const double &scale);

private:
    int xRand,yRand,zRand;
    int octaves;
    float featureFreq,xMul,yMul,zMul;
};

}

#endif // SIMPLEXEDIT_H
