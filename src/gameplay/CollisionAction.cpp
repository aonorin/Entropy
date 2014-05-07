// Bullet includes
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "btBulletDynamicsCommon.h"

// Entropy includes
#include "CollisionAction.h"
#include "src/Physics/collisiongroupmanager.h"
#include "src/Script/script.h"
#include "src/Physics/physics.h"

using namespace physics;

namespace gameplay {

CollisionAction::CollisionAction(collision_group_id_t group0, collision_group_id_t group1,
                                 collision_group_id_t collisionCombinationInt) :
    group0(group0),
    group1(group1),
    collisionCombinationInt(collisionCombinationInt)
{

}

CollisionAction::~CollisionAction()
{

}

collision_group_id_t CollisionAction::getCollisionCombinationInt()
{
    return collisionCombinationInt;
}

collision_group_id_t CollisionAction::getGroup0()
{
    return group0;
}

collision_group_id_t CollisionAction::getGroup1()
{
    return group1;
}

void CollisionAction::collide(PhysicsObject* object0, PhysicsObject* object1)
{
    // This should never be reached. This should be subclassed with the action defined there
    std::cout << "Abstract CollisionAction. This should not be called directly. Group0: " << group0
              << " Group1: " << group1 << std::endl;
}

CollisionActionType CollisionAction::type()
{
    return AbstractCollisionAction;
}

void CollisionAction::newCollisionAction(std::string group0, std::string group1)
{
    // Create a CollisionAction object and add it to the eventdispatcher
    collision_group_id_t group0ID = 0;
    collision_group_id_t group1ID = 0;
    collision_group_id_t collisionCombinationInt = 0;

    if(getCollisionIdentities(group0, group1, group0ID, group1ID, collisionCombinationInt))
    {
        world.addCollisionResponse(group0ID, group1ID, new CollisionAction(group0ID, group1ID, collisionCombinationInt));
    }
}

bool CollisionAction::getCollisionIdentities(std::string group0, std::string group1,
                                             collision_group_id_t& group0ID,
                                             collision_group_id_t& group1ID,
                                             collision_group_id_t& collisionCombinationInt)
{
    bool foundGroup0 = CollisionGroupManager::getCollisionGroupInt(group0, group0ID);
    bool foundGroup1 = CollisionGroupManager::getCollisionGroupInt(group1, group1ID);

    if(foundGroup0 && foundGroup1)
    {
        collisionCombinationInt = group0ID | group1ID;
        return true;
    }

    else
    {
        if(!foundGroup0)
        {
            std::string errorReport("print('Collision group not found: ");
            errorReport.append(group0);
            errorReport.append("')");
            script::interpret(errorReport.c_str());
            std::cerr << "Collision group not found: " << group0 << std::endl;
        }

        if(!foundGroup1)
        {
            std::string errorReport("print('Collision group not found: ");
            errorReport.append(group1);
            errorReport.append("')");
            script::interpret(errorReport.c_str());
            std::cerr << "Collision group not found: " << group1 << std::endl;
        }

        return false;
    }
}

//////////////////
// ForceCollision
//////////////////

ForceCollision::ForceCollision(collision_group_id_t group0, collision_group_id_t group1, double force,
                               collision_group_id_t collisionCombinationInt) :
    CollisionAction(group0, group1, collisionCombinationInt),
    force(force)
{

}

void ForceCollision::collide(PhysicsObject* object0, PhysicsObject* object1)
{
    btRigidBody* forcedObject;
    btVector3 forceVector;
    btVector3 sourceVector, targetVector;

    if(object0->getCollisionGroup() == group1) // if object0 is the target group
    {
        forcedObject = btRigidBody::upcast(object0->getObject());
        sourceVector = object1->getObject()->getWorldTransform().getOrigin();
        targetVector = object0->getObject()->getWorldTransform().getOrigin();
    }

    else
    {
        forcedObject = btRigidBody::upcast(object1->getObject());
        sourceVector = object0->getObject()->getWorldTransform().getOrigin();
        targetVector = object1->getObject()->getWorldTransform().getOrigin();
    }

    forceVector = targetVector.cross(sourceVector);
    forcedObject->applyImpulse(forceVector.safeNormalize() * force, targetVector - sourceVector);
}

CollisionActionType ForceCollision::type()
{
    return ForceCollisionAction;
}

void ForceCollision::newForceCollisionAction(std::string sourceGroup, std::string targetGroup, double force)
{
    // Create a CollisionAction object and add it to the eventdispatcher
    collision_group_id_t group0ID = 0;
    collision_group_id_t group1ID = 0;
    collision_group_id_t collisionCombinationInt = 0;

    if(getCollisionIdentities(sourceGroup, targetGroup, group0ID, group1ID, collisionCombinationInt))
    {
        world.addCollisionResponse(
            group0ID,
            group1ID,
            new ForceCollision(group0ID, group1ID, force, collisionCombinationInt)
        );
    }
}

} // gameplay namespace
