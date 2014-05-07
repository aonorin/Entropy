#ifndef WORLD_H
#define WORLD_H

// STL
#ifdef __LINUX__
#include <mutex>
#else
#include <boost/thread/locks.hpp>
#endif
#include <deque>

// Boost includes
#include "boost/thread.hpp"
#include "boost/thread/shared_mutex.hpp"
#include "boost/thread/locks.hpp"
#include "boost/interprocess/sync/interprocess_semaphore.hpp"

// Bullet includes
#include "LinearMath/btAlignedObjectArray.h"
#include "LinearMath/btIDebugDraw.h"

// Entropy includes
#include "src/Physics/eventdispatcher.h"
#include "src/gameplay/CollisionAction.h"
#include "VoxelTerrain/Mesh.h"

// Bullet forward declarations
class btDynamicsWorld;
class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class btDiscreteDynamicsWorld;
class btSoftRigidDynamicsWorld;
class btCollisionObject;
class btRigidBody;
class btSoftBody;
class btTypedConstraint;
class btGhostPairCallback;

namespace physics {

typedef void (*btInternalTickCallback) (btDynamicsWorld* world, btScalar timeStep);

class World
{
public:

    World();
    ~World();

    void lockForRead();
    void unlockForRead();
    void lockForWrite();
    void unlockForWrite();
    void initPhysics();
    void exitPhysics();
    void setGravity(const btVector3 &gravity);
    void update(double timeSinceLastFrame);
    void drawDebug();
    void resetScene();
    void addCollisionObject(btCollisionObject* object, short group = COL_MAIN, short mask = mainMask);
    void eagerRemoveCollisionObject(btCollisionObject* object);
    void removeCollisionObject(btCollisionObject* object);
    void eagerAddRigidBody(btRigidBody* body, short group = COL_MAIN, short mask = mainMask);
    void addRigidBody(btRigidBody* body, short group = COL_MAIN, short mask = mainMask);
    void addSoftBody(btSoftBody* body, short group = COL_MAIN, short mask = mainMask);
    void addConstraint(btTypedConstraint* constraint, bool disableCollisionBetweenLinkedObjects = false);
    void removeConstraint(btTypedConstraint* constraint);
    void setDebugDrawer(btIDebugDraw* debugDraw);
    btSoftRigidDynamicsWorld* getDynamicsWorld();
    EventDispatcher* getDispatcher();

    // Returns the unique group combination number
    collision_group_id_t addCollisionResponse(collision_group_id_t group0,
                                              collision_group_id_t group1,
                                              gameplay::CollisionAction* collisionAction);
    // You can remove by either calling the unique combination or just enumerating the two groups
    void removeCollisionResponse(collision_group_id_t groupCombination);
    void removeCollisionResponse(collision_group_id_t group0, collision_group_id_t group1);
    void addCollisionFilter(btOverlapFilterCallback* callback);
    void removeCollisionFilter(); // deletes the callback

    static btScalar timeStep;

    bool running;

    typedef boost::upgrade_to_unique_lock<boost::shared_mutex> world_write_t;
    typedef boost::upgrade_lock<boost::shared_mutex> world_upgrade_write_t;
    typedef boost::shared_lock<boost::shared_mutex> world_read_t;
    typedef boost::function<void (int,voxel::Mesh::ptr,Ogre::Vector3) > VoxelPhysicsFunction_t;

    struct VoxelPhysicsFuncStruct
    {
        VoxelPhysicsFunction_t func;
        int id;
        voxel::Mesh::ptr shape;
        Ogre::Vector3 position;
    };

    void addVoxelFunction(VoxelPhysicsFunction_t func, int id, voxel::Mesh::ptr shape, Ogre::Vector3 position);

    void setPaused(bool paused);
    bool getPaused();

private:

    btAlignedObjectArray<btCollisionShape*>	collisionShapes;
    btBroadphaseInterface*	broadphase;
    //btCollisionDispatcher*	dispatcher;
    EventDispatcher* dispatcher;
    btConstraintSolver*	solver;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btSoftRigidDynamicsWorld* dynamicsWorld;
    btOverlapFilterCallback* overlapFilterCallback;

    std::deque<VoxelPhysicsFuncStruct> voxelPhysicsFunctions;

    // structs for deferred add/remove of objects and constraints
    struct addConstraintArgs {
        btTypedConstraint* constraint;
        bool disableCollisionBetweenLinkedObjects;

        addConstraintArgs(btTypedConstraint* constraint, bool disableCollisionBetweenLinkedObjects) :
            constraint(constraint), disableCollisionBetweenLinkedObjects(disableCollisionBetweenLinkedObjects) {}
    };

    struct addRigidBodyArgs {
        btRigidBody* body;
        short group;
        short mask;

        addRigidBodyArgs(btRigidBody *body, short group, short mask) : body(body), group(group), mask(mask) {}
    };

    struct addSoftBodyArgs {
        btSoftBody* body;
        short group;
        short mask;

        addSoftBodyArgs(btSoftBody *body, short group, short mask) : body(body), group(group), mask(mask) {}
    };

    struct addCollisionObjectArgs {
        btCollisionObject* body;
        short group;
        short mask;

        addCollisionObjectArgs(btCollisionObject *body, short group, short mask) : body(body), group(group), mask(mask) {}
    };

    // Mutexes and deques for deferred add/remove of objects and constraints from the physics::world
#ifdef __LINUX__
    std::mutex addConstraintMutex, removeConstraintMutex, addRigidBodyMutex, addSoftBodyMutex, addCollisionObjectMutex, removeCollisionObjectMutex,voxelMutex;
#else
    boost::mutex addConstraintMutex, removeConstraintMutex, addRigidBodyMutex, addSoftBodyMutex, addCollisionObjectMutex, removeCollisionObjectMutex,voxelMutex;
#endif
    std::deque<addRigidBodyArgs> addRigidBodyDeque;
    std::deque<addSoftBodyArgs> addSoftBodyDeque;
    std::deque<addCollisionObjectArgs> addCollisionObjectDeque;
    std::deque<btCollisionObject*> removeCollisionObjectDeque;
    std::deque<btRigidBody*> removeRigidBodyDeque;
    std::deque<btSoftBody*> removeSoftBodyDeque;
    std::deque<addConstraintArgs> addConstraintDeque;
    std::deque<btTypedConstraint*> removeConstraintDeque;
    btGhostPairCallback* ghostPairCallback;

    bool paused;
};

}

#endif // WORLD_H
