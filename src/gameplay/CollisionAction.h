#ifndef COLLISIONACTION_H
#define COLLISIONACTION_H

// System includes
#include <string>

// Entropy includes
#include "src/Physics/physicsobject.h"

namespace gameplay {

enum CollisionActionType
{
    AbstractCollisionAction,
    ForceCollisionAction,
    GrapplingHookCollisionAction,
    WaterObjectCollisionAction
};


///////////////////
// CollisionAction
///////////////////

class CollisionAction
{
public:
    CollisionAction(physics::collision_group_id_t group0, physics::collision_group_id_t group1,
                    physics::collision_group_id_t collisionCombinationInt);
    virtual ~CollisionAction();

    physics::collision_group_id_t getCollisionCombinationInt();
    physics::collision_group_id_t getGroup0();
    physics::collision_group_id_t getGroup1();
    virtual void collide(physics::PhysicsObject* object0, physics::PhysicsObject* object1); // Overload this!
    virtual CollisionActionType type(); // Overload this!

    static void newCollisionAction(std::string group0, std::string group1); // JUST AN EXAMPLE. DON'T USE THIS.

protected:

    static bool getCollisionIdentities(std::string group0, std::string group1,
                                       physics::collision_group_id_t& group0ID, physics::collision_group_id_t& group1ID,
                                       physics::collision_group_id_t& collisionCombinationInt);

    physics::collision_group_id_t group0, group1, collisionCombinationInt;
};


///////////////////
// ForceCollision
///////////////////

class ForceCollision : public CollisionAction
{
public:
    ForceCollision(physics::collision_group_id_t group0, physics::collision_group_id_t group1, double force,
                   physics::collision_group_id_t collisionCombinationInt);

    virtual void collide(physics::PhysicsObject* object0, physics::PhysicsObject* object1);
    virtual CollisionActionType type();

    static void newForceCollisionAction(std::string sourceGroup, std::string targetGroup, double force);

protected:

    double force;
};

} // gameplay namespace

#endif // COLLISIONACTION_H
