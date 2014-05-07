#ifndef VOXELPHYSICS_H
#define VOXELPHYSICS_H

#include "voxelterrain.h"
#include "Mesh.h"
#include "rigidentity.h"
#include "Physics/world.h"
#include "boost/shared_ptr.hpp"
namespace voxel {

class VoxelPhysics : public Ogre::Singleton<VoxelPhysics>
{
public:
    typedef boost::shared_ptr<VoxelPhysics> ptr;
    VoxelPhysics(VoxelTerrain *terrain);
    ~VoxelPhysics();

    // voxelMapThread
    void voxelMapPartChanged(entropy::uint16 lod, int id);
    // physicThread
    void voxelMapPartChangedPhysic(int id, Mesh::ptr shape, Ogre::Vector3 position);

    struct Rigid
    {
        RigidEntity::ptr entity;
        Mesh::ptr shape;
    };
    typedef boost::unordered::unordered_map<int, Rigid> t_PartRigidMap;
    t_PartRigidMap m_partRigids;

    void removePhysicsObject(entropy::uint16 lod, int id);

    VoxelTerrain* m_ground;

    static VoxelPhysics& getSingleton(void);
    static VoxelPhysics* getSingletonPtr(void);
//    World* m_physics;
};

}

#endif // VOXELPHYSICS_H
