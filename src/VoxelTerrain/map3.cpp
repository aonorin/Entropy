#include "map3.h"
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "Core/global.h"

namespace voxel {

Map3::Map3()
{
}

Map3::Map3(int offsetX, int offsetY, int offsetZ, entropy::uint sizeX, entropy::uint sizeY, entropy::uint sizeZ)
    : sizeX(sizeX)
    , sizeY(sizeY)
    , sizeZ(sizeZ)
    , offsetX(offsetX)
    , offsetY(offsetY)
    , offsetZ(offsetZ)
    , voxels(0)
    , numAboveZero(0)
{
}

Map3::~Map3()
{
    std::cout << "Map3::~Map3()" << std::endl;
    try{
    if (voxels != 0)
        delete [] voxels;
    } catch(std::exception& e) {
        std::cout << "Error deleting Map3::voxels: " << e.what() << std::endl;
    }
}

entropy::uint Map3::posMap(int x, int y, int z)
{
    x+=offsetX;y+=offsetY;z+=offsetZ;
    entropy::uint res = x + y*(sizeX) + z*(sizeX)*(sizeY);
    return res;
}

Voxel *Map3::getVoxel(int x, int y, int z)
{
    entropy::uint id = posMap(x,y,z);
    assert(id < size());
    return &voxels[id];
}

void Map3::calcNumAboveZero(entropy::uint ind, const Voxel&data)
{
    if (data.scalar > 0. && voxels[ind].scalar <= 0)
        ++numAboveZero;
    if (data.scalar <= 0. && voxels[ind].scalar > 0)
        --numAboveZero;
}

/*
void Map3::setVoxel(int x, int y, int z, Voxeldata)
{
    entropy::uint pos = posMap(x, y, z);
    if (pos >= this->size())
        assert(0);
    calcNumAboveZero(pos, data);
    voxel[pos] = data;
    addToDelta(x, y, z);
}
*/

void Map3::setVoxelIfLowerZero(int x, int y, int z, Voxel data)
{
    entropy::uint pos = posMap(x, y, z);
    assert(pos < this->size());
    if (voxels[pos].scalar < 0.)
    {
        float oldScalar = voxels[pos].scalar;
        calcNumAboveZero(pos, data);
        voxels[pos] = data;
        if (oldScalar >= 0.)
        {
            voxels[pos].scalar = (oldScalar + voxels[pos].scalar) / 2.;
        }
    }
}

/*void Map3::setVoxelIfHigherEqualOne(int x, int y, int z, Voxeldata)
{
    entropy::uint pos = posMap(x, y, z);
    if (pos >= this->size())
        assert(0);
    if (voxel[pos].scalar >= 1.)
    {
        // data.scalar = ((-voxel[pos].scalar) + data.scalar) / 2.;
        calcNumAboveZero(pos, data);
        voxel[pos] = data;
        addToDelta(x, y, z);
    }
}*/

void Map3::setVoxelIfHigherZero(int x, int y, int z, Voxel data)
{
    entropy::uint pos = posMap(x, y, z);
    assert(pos < this->size());
    if (voxels[pos].scalar > 0.)
    {
        calcNumAboveZero(pos, data);
        voxels[pos] = data;
    }
}
/*
void Map3::removeVoxel(int x, int y, int z)
{
    entropy::uint pos = posMap(x, y, z);
    Voxelrem;
    rem.scalar = -1;
    calcNumAboveZero(pos, rem);
    voxel[pos] = rem;
    addToDelta(x, y, z);
}
*/

entropy::uint Map3::getNumAboveZero()
{
    return numAboveZero;
}

void Map3::begin()
{
#ifdef DEBUG
    std::cout << "Map3::begin()" << std::endl;
#endif

    entropy::uint size = Map3::size();
    if (voxels) {
        std::cerr << "VoxelMemory Leak?" << std::endl;
    }
#ifdef DEBUG
    std::cout << "Map3 begin size and voxel check." << std::endl;
#endif

    voxels = new Voxel[size];

#ifdef DEBUG
    std::cout << "Map3 voxel array created." << std::endl;
#endif

    for (entropy::uint ind = 0; ind < size; ++ind)
        voxels[ind].scalar = -1.;
    numAboveZero = 0;
#ifdef DEBUG
    std::cout << "Voxels initialized." << std::endl;
#endif
}

void Map3::end()
{
#ifdef DEBUG
    std::cout << "Map3::end()" << std::endl;
#endif
    if(voxels)
    {
        delete [] voxels;
    }
    voxels = 0;
}

entropy::uint Map3::size()
{
    return (sizeX)*(sizeY)*(sizeZ);
}

int Map3::getOffsetX()
{
    return offsetX;
}

int Map3::getOffsetY()
{
    return offsetY;
}

int Map3::getOffsetZ()
{
    return offsetZ;
}

}
