#ifndef VOXELTERRAIN_H
#define VOXELTERRAIN_H

#include "voxelmap.h"
#include "Core/global.h"

namespace voxel {

class VoxelMap3;

class VoxelTerrain
{
public:
    VoxelTerrain(entropy::uint16 numLod, entropy::uint32 sizeX, entropy::uint32 sizeY, entropy::uint32 sizeZ);
    ~VoxelTerrain();

    // threadsafe
    void edit(boost::shared_ptr<Edit> ed, bool calcGeometry = false);
    // void edit (axisAlignedBox aab, bool cut);

    // lod count does not change, so threadsafe
    entropy::uint16 numLod();

    // threadsafe
    typedef boost::signals2::signal<void (entropy::uint16, int)> t_signalPartChanged;
    void addPartChangedListener(const t_signalPartChanged::slot_type &subscriber)
    {m_sigPartChanged.connect(subscriber);}

    // threadsafe
    typedef boost::signals2::signal<void (entropy::uint16, int, bool)> t_signalPartAddedOrRemoved;
    void addPartVoxelChangedListener(const t_signalPartAddedOrRemoved::slot_type &subscriber)
    {m_sigPartAddedOrRemoved.connect(subscriber);}

    VoxelMap3* voxelMap(entropy::uint16 lod);
    typedef boost::shared_ptr<Part3> t_part3Ptr;
    t_part3Ptr part(entropy::uint16 lod, int par);
private:
    void partChanged (entropy::uint16 map, int part);
    void partRemovedOrAdded (entropy::uint16 map, int part, bool remove);

    t_signalPartChanged m_sigPartChanged;
    t_signalPartAddedOrRemoved m_sigPartAddedOrRemoved;

    std::vector<VoxelMap3*> m_maps;
};

}

#endif // VOXELTERRAIN_H
