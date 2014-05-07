#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

// System includes
#ifdef __APPLE__
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

// Bullet includes
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"

// Boost includes
#include "boost/function.hpp"

// Entropy includes
#include "src/Physics/physicsobject.h"
#include "src/gameplay/CollisionAction.h"

// Global Bullet func, used for voxel collision actions
extern ContactProcessedCallback gContactProcessedCallback;

namespace physics {

// As a rule of thumb the lowered numbered group object will be first
typedef boost::function<void (PhysicsObject* object0, PhysicsObject* object1)> collision_response_t;

class EventDispatcher : public btCollisionDispatcher
{
public:

    EventDispatcher(btCollisionConfiguration* collisionConfiguration);
    ~EventDispatcher();

    // Returns the unique group combination number
    collision_group_id_t addCollisionResponse(collision_group_id_t group0, collision_group_id_t group1,
                             gameplay::CollisionAction* collisionAction);
    // You can remove by either calling the unique combination or just enumerating the two groups
    void removeCollisionResponse(collision_group_id_t groupCombination);
    void removeCollisionResponse(collision_group_id_t group0, collision_group_id_t group1);

#ifdef __APPLE__
    virtual bool needsResponse(btCollisionObject *body0, btCollisionObject *body1);
#else
    virtual bool needsResponse(const btCollisionObject *body0, const btCollisionObject *body1);
#endif

    // Custom voxel collision actions
    static bool voxelNeedsResponse(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1);
    void response(PhysicsObject* objectPointer0, PhysicsObject* objectPointer1);

#ifdef __APPLE__
    typedef std::tr1::unordered_map<collision_group_id_t, gameplay::CollisionAction*> response_map_t;
#else
    typedef std::unordered_map<collision_group_id_t, gameplay::CollisionAction*> response_map_t;
#endif

    static EventDispatcher* EVENT_DISPATCHER;
    response_map_t responseMap;
};

} // physics namespace

#endif // EVENTDISPATCHER_H
