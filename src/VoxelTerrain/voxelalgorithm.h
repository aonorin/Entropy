#ifndef VOXELALGORITHM_H
#define VOXELALGORITHM_H

#include "blobedit.h"

namespace voxel
{

class Algorithm
{
public:
    typedef boost::shared_ptr<Algorithm> ptr;
    Algorithm(int xSize, int ySize, int zSize);

    virtual void calculateVoxels(Ogre::Vector3 pos, entropy::uint32 seed);

    virtual float getValueAtIndex(Ogre::Vector3 index);

    BlobEdit::ptr getBlobEdit();

protected:
    int xSize,ySize,zSize;
    entropy::uint32 seed;
    Ogre::Vector3 pos;

    BlobEdit::ptr blobEdit;
};

}
#endif // VOXELALGORITHM_H
