#ifndef ENEMYACTOR_H
#define ENEMYACTOR_H

#include "Physics/physicsobject.h"
#include "Physics/physics.h"
#include "Core/Entropy.h"
#include "Physics/collisiongroupmanager.h"
#include "gameplay/actors.h"

namespace gameplay {
namespace actor {

static int enemyActorCount;

class EnemyActor : public physics::PhysicsActor
{
public:
    std::string generateName();
    EnemyActor(Ogre::Vector3 position,btCollisionObject *object, std::string collisionGroupName,
               entropy::WalkType walkability,bool diagonolMovement = true, entropy::uint32 tilesToUpdate=5,entropy::uint32 movesTilUpdate=120);

    virtual void move(int targetID);
    virtual void move(Ogre::Vector3 nextPosition);
    void setTarget(PhysicsActor* actor);
    void setTarget(int id);
    void setEntity(Ogre::Entity* entity);
    virtual void updateTransform();

protected:
    int generateCollisionGroup();
    Ogre::Entity* entity;
    Ogre::Vector3 position;

};

} }

#endif // ENEMYACTOR_H
