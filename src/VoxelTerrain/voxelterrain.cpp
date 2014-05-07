#include "voxelterrain.h"

using namespace voxel;

VoxelTerrain::VoxelTerrain(entropy::uint16 numLod, entropy::uint32 sizeX, entropy::uint32 sizeY, entropy::uint32 sizeZ)
{
    double factor(1.0);
    for (entropy::uint16 ind = 0; ind < numLod; ++ind)
    {
        VoxelMap3 *newOne = new VoxelMap3(sizeX, sizeY, sizeZ, ind > 0);
        newOne->scale(factor);
        factor*=2.;
        m_maps.push_back(newOne);
        newOne->addPartChangedListener(boost::bind(&VoxelTerrain::partChanged, this, ind, _1));
        newOne->addPartAddedOrRemovedListener(boost::bind(&VoxelTerrain::partRemovedOrAdded, this, ind, _1, _2));
    }
}

VoxelTerrain::~VoxelTerrain()
{
    //    BOUT("~voxelTerrain()");
    std::cout << "~VoxelTerrain()" << std::endl;
    for (entropy::uint16 ind = 0; ind < m_maps.size(); ++ind)
    {
        delete m_maps[ind];
    }
}

void VoxelTerrain::partChanged(entropy::uint16 map, int part)
{
#ifdef DEBUG
    std::cout << "VoxelTerrain::partChanged("<<map<<","<<part<<")" << std::endl;
#endif
    m_sigPartChanged(map, part);
}

void VoxelTerrain::partRemovedOrAdded(entropy::uint16 map, int part, bool remove)
{
    m_sigPartAddedOrRemoved(map, part, remove);
}

void VoxelTerrain::edit(boost::shared_ptr<Edit> ed, bool calcGeometry)
{
    for (entropy::uint16 ind = 0; ind < m_maps.size(); ++ind)
    {
        VoxelMap3 *work = m_maps.at(ind);
        work->edit(ed, calcGeometry);
    };
}

VoxelMap3* VoxelTerrain::voxelMap(entropy::uint16 lod)
{
    return m_maps.at(lod);
}

VoxelTerrain::t_part3Ptr VoxelTerrain::part(entropy::uint16 lod, int par)
{
    return voxelMap(lod)->getPart(par);
}

entropy::uint16 VoxelTerrain::numLod()
{
    return m_maps.size();
}
