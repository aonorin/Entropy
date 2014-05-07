#ifndef PHYSICSTYPES_H
#define PHYSICSTYPES_H

#include <stdint.h>

// Bullet includes
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"

// Ogre includes
#include "OGRE/OgreVector3.h"

// Boost includes
#include "boost/function.hpp"

// Bullet forward declaration
class btBroadphaseProxy;
class btCollisionObject;

namespace physics {

enum BodyType
{
    Ghost, // NOT IMPLEMENTED CURRENTLY
    Rigid,
    Soft, // NOT IMPLEMENTED CURRENTLY
    Collision // Parent type
};

enum ShapeType
{
    Box,
    Sphere,
    Mesh, // NOT IMPLEMENTED CURRENTLY
    HeightMap // NOT IMPLEMENTED CURRENTLY
};

enum RoomFace
{
    North,
    South,
    East,
    West,
    Top,
    Bottom
};

enum PhysicsMode
{
    StandardPhysics,
    WaterPhysics
};

typedef uint64_t collision_group_id_t;
// This should probably be dynamically generated based on system arch.
const collision_group_id_t MAX_COLLISION_GROUPS = 64;

extern collision_group_id_t MAIN_GROUP_ID;
extern collision_group_id_t PLAYER_GROUP_ID;
extern collision_group_id_t EQUIPMENT_GROUP_ID;
extern collision_group_id_t VOXEL_GROUP_ID;
extern collision_group_id_t SENSOR_GROUP_ID;

extern const std::string MAIN_GROUP_NAME;
extern const std::string PLAYER_GROUP_NAME;
extern const std::string EQUIPMENT_GROUP_NAME;
extern const std::string VOXEL_GROUP_NAME;
extern const std::string SENSOR_GROUP_NAME;

// Used for filtering collisions directly in the Bullet physics.
// DO NOT USE THESE FOR COLLISION ACTIONS/RESPONSES. ONLY FOR FILTERING
#define BIT(x) (1<<(x))
enum BulletCollisionGroup
{
    COL_NOTHING = 0, // Collide with nothing
    COL_MAIN = BIT(0), // Collide with the main group
    COL_PLAYER = BIT(1), // Collide with the player
    COL_EQUIPMENT = BIT(2), // Collide with equipment
    COL_VOXEL = BIT(3), // Collide with Voxel
    COL_SENSOR = BIT(4) // Collide with Sensor
};

extern short nothingMask;
extern short mainMask;
extern short playerMask;
extern short equipmentMask;
extern short voxelMask;
extern short sensorMask;

enum CursorType
{
    NoCursor,
    PickUpCursor,
    HackCursor,
    LookCursor,
    ActivateCursor
};

// Helper class for changing  between Ogre::Vector3 and btVector3
class Vec3 : public btVector3
{
public:
    Vec3(const Ogre::Vector3& vec);
    Vec3(btScalar x = 0.0, btScalar y = 0.0, btScalar z = 0.0);
    Vec3(const btVector3& vec);

    Ogre::Vector3 toOgre();
};

class Quaternion : public btQuaternion
{
public:
    Quaternion(const Ogre::Quaternion& quaternion);
    Quaternion(btScalar x = 0.0, btScalar y = 1.0, btScalar z = 0.0, btScalar w = 0.0);
    Quaternion(const btQuaternion& quaternion);

    Ogre::Quaternion toOgre();
};

/////////////////////////////////////
// PlayerLiftCollisionFilterCallback
/////////////////////////////////////

struct PlayerLiftCollisionFilterCallback : public btOverlapFilterCallback
{
    PlayerLiftCollisionFilterCallback(btCollisionObject* playerObject, btCollisionObject* liftObject);

    virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const;

private:

    btCollisionObject *playerObject, *liftObject;
};

} // physics namespace

#endif // PHYSICSTYPES_H
