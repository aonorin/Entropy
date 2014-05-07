#include "voxelalgorithm.h"

using namespace voxel;

Algorithm::Algorithm(int xSize, int ySize, int zSize) :
    xSize(xSize),
    ySize(ySize),
    zSize(zSize),
    seed(seed)
{
}

void Algorithm::calculateVoxels(Ogre::Vector3 pos, entropy::uint32 seed)
{

}

float Algorithm::getValueAtIndex(Ogre::Vector3 index)
{

}

BlobEdit::ptr Algorithm::getBlobEdit()
{
    return blobEdit;
}
