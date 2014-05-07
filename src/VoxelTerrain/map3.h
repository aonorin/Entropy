#ifndef MAP3_H
#define MAP3_H

#include "src/VoxelTerrain/voxel.h"
#include "Core/global.h"

//typedef unsigned int uint;

namespace voxel {

class Map3
{
public:
    struct DeltaPos
    {
        DeltaPos(int x, int y, int z)
            : deltaX(x)
            , deltaY(y)
            , deltaZ(z)
        {
        }

        int deltaX;
        int deltaY;
        int deltaZ;
    };

    Map3();
    Map3(int offsetX, int offsetY, int offsetZ, entropy::uint32 sizeX, entropy::uint32 sizeY, entropy::uint32 sizeZ);
    ~Map3();

    Voxel *getVoxel(int x, int y, int z);

    // void setVoxel (int32 x, int32 y, int32 z, voxel data);
    void setVoxelIfLowerZero(int x, int y, int z, Voxel data);
    // void setVoxelIfHigherEqualOne(int32 x, int32 y, int32 z, voxel data);
    void setVoxelIfHigherZero(int x, int y, int z, Voxel data);
    // void removeVoxel(int32 x, int32 y, int32 z);

    entropy::uint32 getNumAboveZero();

    void begin();
    void end();

    int getOffsetX();
    int getOffsetY();
    int getOffsetZ();
    entropy::uint32 size();

private:
    entropy::uint32 posMap(int x, int y, int z);
    void calcNumAboveZero(entropy::uint32 ind, const Voxel &data);


    entropy::uint32 sizeX;
    entropy::uint32 sizeY;
    entropy::uint32 sizeZ;

    int offsetX;
    int offsetY;
    int offsetZ;

    Voxel* voxels;

    entropy::uint32 numAboveZero;
};

}

#endif // MAP3_H
