#include "voxelphysics.h"
#include "Physics/physics.h"
#include "Physics/physicstypes.h"

//#include "voxelTerrain.hpp"
//#include "Mesh.hpp"
//#include "RigidEntity.hpp"

using namespace voxel;

VoxelPhysics::VoxelPhysics(VoxelTerrain *terrain)
    : m_ground(terrain)
    //    , m_physics(physics)
{
    m_ground->addPartChangedListener(boost::bind(&VoxelPhysics::voxelMapPartChanged, this, _1, _2));
}

VoxelPhysics::~VoxelPhysics()
{
    std::cout << "VoxelPhysics::~VoxelPhysics()" << std::endl;
}

void VoxelPhysics::voxelMapPartChanged(entropy::uint16 lod, int id)
{
//    std::cout << "VoxelPhysics::voxelMapPartChanged("<< lod<<","<<id<<")"<< std::endl;

    Part3::ptr part = m_ground->part(lod, id);

    if (part != Part3::ptr())
    {
        Triangle::vectorPtr triangles = part->hullTriangles(0);
        if(triangles != Triangle::vectorPtr())
        {
            if (triangles->size() > 0)
            {
                Ogre::Vector3Ptr vertices = part->hullVertices();
                Mesh::ptr shape(new Mesh(vertices, triangles, part->aabb()));
                //            m_physics->addToDo(boost::bind(&voxelTerrain::voxelMapPartChangedPhysic, this, id, shape, part->aabb().minimum()));
                physics::world.addVoxelFunction(
                            boost::bind(&VoxelPhysics::voxelMapPartChangedPhysic, this, id, shape, part->aabb().getMinimum()*GRAPHICS_SCALE),
                            id,
                            shape,
                            part->aabb().getMinimum()*GRAPHICS_SCALE
                            );
            }
            else
            {
                //            m_physics->addToDo(boost::bind(&voxelTerrain::voxelMapPartChangedPhysic, this, id, static_cast<blub::physic::shape::mesh*>(0), part->aabb().minimum()));
            }
        }
    }
}

void VoxelPhysics::voxelMapPartChangedPhysic(int id, Mesh::ptr shape, Ogre::Vector3 position)
{
//    std::cout << "VoxelPhysics::voxelMapPartChangedPhysic("<<id<<","<<shape<<","<< position<<")" << std::endl;
    // delete old one
    t_PartRigidMap::iterator it = m_partRigids.find(id);
    if (it != m_partRigids.end())
    {
        //ADD REMOVING OF RIGID ENTITY FROM WORL HERE
        physics::world.eagerRemoveCollisionObject(m_partRigids[id].entity->getRigidBody());
        //        delete m_partRigids[id].entity;
        //        delete m_partRigids[id].shape;
        m_partRigids.erase(id);
    }
    if (shape != Mesh::ptr())
    {
        // get new one
        // add/replace old one
        Rigid rig;
        rig.entity = RigidEntity::ptr(new RigidEntity(shape, 0));
        rig.entity->position(position);
        rig.shape = shape;
        m_partRigids[id] = rig;
//        physics::world.addCollisionObject( m_partRigids[id].entity->getRigidBody().get());
        std::cout << "add RigidEntity id: " << id << std::endl;
        physics::world.eagerAddRigidBody(rig.entity->getRigidBody(), physics::COL_VOXEL, physics::voxelMask);
//        shape.reset();
//        rig.entity.reset();
    }
}

void VoxelPhysics::removePhysicsObject(entropy::uint16 lod, int id)
{
    Part3::ptr part = m_ground->part(lod, id);

    t_PartRigidMap::iterator it = m_partRigids.find(id);
    if (it != m_partRigids.end())
    {
        std::cout << "void VoxelPhysics::removePhysicsObject(entropy::uint16 lod: " <<lod <<", int id: " << id << ")" << std::endl;
        //ADD REMOVING OF RIGID ENTITY FROM WORL HERE
//        std::cout << "physics::world.eagerRemoveCollisionObject(m_partRigids[id].entity->getRigidBody());" << std::endl;
//        physics::world.eagerRemoveCollisionObject(m_partRigids[id].entity->getRigidBody());
        //        delete m_partRigids[id].entity;
        //        delete m_partRigids[id].shape;
//        m_partRigids.erase(id);
    }
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> VoxelPhysics* Ogre::Singleton<VoxelPhysics>::ms_Singleton = 0;
VoxelPhysics* Entropy::getSingletonPtr(void)
{
    return ms_Singleton;
}
VoxelPhysics& VoxelPhysics::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> VoxelPhysics* Ogre::Singleton<VoxelPhysics>::msSingleton = 0;
VoxelPhysics* VoxelPhysics::getSingletonPtr(void)
{
    return msSingleton;
}
VoxelPhysics& VoxelPhysics::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
