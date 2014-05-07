// System includes
#include <iostream>

// Bullet includes
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"

// Entropy includes
#include "eventdispatcher.h"
#include "physicstypes.h"
#include "src/Physics/collisiongroupmanager.h"

namespace physics {

EventDispatcher* EventDispatcher::EVENT_DISPATCHER = 0;

EventDispatcher::EventDispatcher(btCollisionConfiguration *collisionConfiguration) :
    btCollisionDispatcher(collisionConfiguration)
{
    EventDispatcher::EVENT_DISPATCHER = this;
    gContactProcessedCallback = (ContactProcessedCallback) voxelNeedsResponse;
}

EventDispatcher::~EventDispatcher()
{
    response_map_t::iterator iter = responseMap.begin();

    while(iter != responseMap.end())
    {
        delete iter->second;
        ++iter;
    }

    responseMap.clear();
}

// Called internally in bullet to filter collisions
#ifdef __APPLE__
bool EventDispatcher::needsResponse(btCollisionObject *body0, btCollisionObject *body1)
#else
bool EventDispatcher::needsResponse(const btCollisionObject *body0, const btCollisionObject *body1)
#endif
{
    bool respond = btCollisionDispatcher::needsResponse(body0, body1); // get the parent response

    if(respond)
    {
        PhysicsObject* objectPointer0 = (PhysicsObject*) body0->getUserPointer(); // Get the userPointer
        PhysicsObject* objectPointer1 = (PhysicsObject*) body1->getUserPointer(); // Get the userPointer

        if(objectPointer0 && objectPointer1)
        {
            if(objectPointer0->getCollisionGroup() != physics::VOXEL_GROUP_ID && objectPointer1->getCollisionGroup() != physics::VOXEL_GROUP_ID)
                response(objectPointer0, objectPointer1);
        }
    }

    return respond;
}

collision_group_id_t EventDispatcher::addCollisionResponse(collision_group_id_t group0,
                                                           collision_group_id_t group1,
                                                           gameplay::CollisionAction* collisionAction)
{
    collision_group_id_t groupCombination = group0 | group1;

    response_map_t::iterator find = responseMap.find(groupCombination);
    if(find != responseMap.end())
        removeCollisionResponse(groupCombination);

    responseMap[groupCombination] = collisionAction;
    return groupCombination;
}

void EventDispatcher::removeCollisionResponse(collision_group_id_t groupCombination)
{
    delete responseMap[groupCombination];
    responseMap.erase(groupCombination);
}

void EventDispatcher::removeCollisionResponse(collision_group_id_t group0, collision_group_id_t group1)
{
    collision_group_id_t groupCombination = group0 | group1;
    removeCollisionResponse(groupCombination);
}

bool EventDispatcher::voxelNeedsResponse(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1)
{
    PhysicsObject* objectPointer0 = (PhysicsObject*) body0->getUserPointer(); // Get the userPointer
    PhysicsObject* objectPointer1 = (PhysicsObject*) body1->getUserPointer(); // Get the userPointer

    if(objectPointer0 && objectPointer1)
    {
        if(objectPointer0->getCollisionGroup() == physics::VOXEL_GROUP_ID || objectPointer1->getCollisionGroup() == physics::VOXEL_GROUP_ID)
            EventDispatcher::EVENT_DISPATCHER->response(objectPointer0, objectPointer1);
    }

    return true;
}

void EventDispatcher::response(PhysicsObject* objectPointer0, PhysicsObject* objectPointer1)
{
    // Check if the userPointers are not 0, if not then we check for a collision_response
    collision_group_id_t group0 = objectPointer0->getCollisionGroup();
    collision_group_id_t group1 = objectPointer1->getCollisionGroup();

    // CollisionGroup is organized as powers of 2 which allows simple addition to define unique combinations
    collision_group_id_t groupCombination = group0 | group1;

    response_map_t::iterator find = responseMap.find(groupCombination);
    if(find != responseMap.end()) // check if the group combination has a response in our map
    {
        // call the response, supplying the PhysicsObject pointers. The lowered numbered group is supplied first
        if(group0 <= group1)
            responseMap[groupCombination]->collide(objectPointer0, objectPointer1);
        else
            responseMap[groupCombination]->collide(objectPointer1, objectPointer0);
    }
}

} // physics namespace
