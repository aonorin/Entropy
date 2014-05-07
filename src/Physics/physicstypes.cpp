// Bullet includes
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"

// Entropy includes
#include "physicstypes.h"

namespace physics {

// When adding new default groups be sure to add them to CollisionGroupManager::createDefaultGroups()
collision_group_id_t MAIN_GROUP_ID = 0;
collision_group_id_t PLAYER_GROUP_ID = 0;
collision_group_id_t EQUIPMENT_GROUP_ID = 0;
collision_group_id_t VOXEL_GROUP_ID = 0;
collision_group_id_t WATER_GROUP_ID = 0;
collision_group_id_t SENSOR_GROUP_ID = 0;

const std::string MAIN_GROUP_NAME = "Main";
const std::string PLAYER_GROUP_NAME = "Player";
const std::string EQUIPMENT_GROUP_NAME = "Equipment";
const std::string VOXEL_GROUP_NAME = "Voxel";
const std::string WATER_GROUP_NAME = "Water";
const std::string SENSOR_GROUP_NAME = "Sensor";

// Bullet collision filtering masks
short nothingMask = COL_NOTHING;
short mainMask = COL_MAIN | COL_PLAYER | COL_EQUIPMENT | COL_VOXEL | COL_SENSOR;
short playerMask = COL_MAIN | COL_VOXEL | COL_SENSOR;
short equipmentMask = COL_MAIN | COL_VOXEL;
short voxelMask = COL_MAIN | COL_PLAYER | COL_EQUIPMENT;
short sensorMask = COL_MAIN | COL_PLAYER;

/////////
// Vec3
/////////

Vec3::Vec3(const Ogre::Vector3& vec) :
    btVector3(vec.x, vec.y, vec.z)
{

}

Vec3::Vec3(btScalar x, btScalar y, btScalar z) :
    btVector3(x, y, z)
{

}

Vec3::Vec3(const btVector3& vec) :
    btVector3(vec)
{

}

Ogre::Vector3 Vec3::toOgre()
{
    return Ogre::Vector3(x(), y(), z());
}

//////////////
// Quaterion
//////////////

Quaternion::Quaternion(const Ogre::Quaternion& quaternion) :
    btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w)
{

}

Quaternion::Quaternion(btScalar x, btScalar y, btScalar z, btScalar w) :
    btQuaternion(x, y, z, w)
{

}

Quaternion::Quaternion(const btQuaternion& quaternion) :
    btQuaternion(quaternion)
{

}

Ogre::Quaternion Quaternion::toOgre()
{
    return Ogre::Quaternion(w(), x(), y(), z());
}

/////////////////////////////////////////////
// PlayerLiftCollisionFilterCallback Filters
/////////////////////////////////////////////

PlayerLiftCollisionFilterCallback::PlayerLiftCollisionFilterCallback(btCollisionObject* playerObject, btCollisionObject* liftObject) :
    playerObject(playerObject),
    liftObject(liftObject)
{

}

// Return true when pairs need collision
bool PlayerLiftCollisionFilterCallback::needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
{
    bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
    collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

    // add some additional logic here that modified 'collides'
    if((proxy0->m_clientObject == playerObject && proxy1->m_clientObject == liftObject)
            || (proxy0->m_clientObject == liftObject && proxy1->m_clientObject == playerObject))
    {
            collides = false;
    }

    return collides;
}

} // physics namespace
