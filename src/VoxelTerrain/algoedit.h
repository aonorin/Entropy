#ifndef ALGOEDIT_H
#define ALGOEDIT_H

#include "VoxelTerrain/edit.h"

namespace voxel
{

class AlgoEdit : public Edit
{
public:
//    typedef boost::shared_ptr<AlgoEdit> ptr;
    AlgoEdit(Ogre::Vector3 dimensions, Ogre::Vector3 pos, int seed);

    virtual double calcVoxel(const Ogre::Vector3 &pos, const double &scale);
    Ogre::AxisAlignedBox abb;
    Ogre::AxisAlignedBox aab(double scale) const;

protected:
    Ogre::Vector3 dimensions;
    Ogre::Vector3 pos;
    int seed;


};

}

#endif // ALGOEDIT_H
