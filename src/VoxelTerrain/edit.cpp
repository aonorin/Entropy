#include "edit.h"
#include "map3.h"

namespace voxel {


void Edit::createVoxel(Map3 *mp,
                 const int _startX,
                 const int _startY,
                 const int _startZ,
                 const int _endX,
                 const int _endY,
                 const int _endZ,
                 const double scale)
{
    Ogre::AxisAlignedBox aabb = aab(scale);
    int startX = (aabb.getMinimum().x)-1;
    if (startX < _startX)
        startX = _startX;
    int endX = aabb.getMaximum().x+1;
    if (endX > _endX)
        endX = _endX;

    int startY = (aabb.getMinimum().y)-1;
    if (startY < _startY)
        startY = _startY;
    int endY = aabb.getMaximum().y+1;
    if (endY > _endY)
        endY = _endY;

    int startZ = (aabb.getMinimum().z)-1;
    if (startZ < _startZ)
        startZ = _startZ;
    int endZ = aabb.getMaximum().z+1;
    if (endZ > _endZ)
        endZ = _endZ;

    // real buffer = 1./scale;

    int localX((startX-_startX)-mp->getOffsetX());
    // loop voxel in sphere
    for (int x = startX; x < endX; ++x, ++localX)
    {
        int localY((startY-_startY)-mp->getOffsetY());
        for (int y = startY; y < endY; ++y, ++localY)
        {
            int localZ((startZ-_startZ)-mp->getOffsetZ());
            for (int z = startZ; z < endZ; ++z, ++localZ)
            {
                double result = calcVoxel(Ogre::Vector3(x, y, z), scale);
//                 result*=scale;
                if (result < -1.)
                    continue;
                if (result > 1.)
                    result = 1.;

                Voxel newVoxel;
                if (!cut)
                {
                    newVoxel.scalar = result;
                    mp->setVoxelIfLowerZero(localX, localY, localZ, newVoxel);
                }
                else
                {
                    newVoxel.scalar = -result;
                    mp->setVoxelIfHigherZero(localX, localY, localZ, newVoxel);
                }
            }
        }
    }
}
}
