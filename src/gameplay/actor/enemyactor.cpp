#include "enemyactor.h"
#include "Core/Entropy.h"
#include "OGRE/OgreEntity.h"
using namespace gameplay;
using namespace actor;

std::string EnemyActor::generateName()
{
    enemyActorCount++;
    std::string generatedName = "entAct";
    generatedName.append(Ogre::StringConverter::toString(enemyActorCount));
//    this->name = generatedName;
    return generatedName;
}

EnemyActor::EnemyActor(Ogre::Vector3 position, btCollisionObject *object,
                       std::string collisionGroupName, entropy::WalkType walkability, bool diagonolMovement, entropy::uint32 tilesToUpdate, entropy::uint32 movesTilUpdate) :
    PhysicsActor(
        generateName(),
        object,
        Entropy::getSingletonPtr()->mSceneMgr->createSceneNode(generateName()),
        physics::CollisionGroupManager::addCollisionGroup(collisionGroupName),
        physics::NoCursor
        ),
    position(position),
    entity(0)
{
    Entropy::getSingletonPtr()->mSceneMgr->getRootSceneNode()->addChild(ogreNode);
    pathID = gameplay::astar.registerMover(ogreNode->getPosition(),astar.getWalkMap(walkability),diagonolMovement,tilesToUpdate,movesTilUpdate);
    gameplay::actors::addActor(this);
}

void EnemyActor::setEntity(Ogre::Entity* entity)
{
    if(this->entity!=0)
    {
        ogreNode->detachObject(this->entity->getName());
        Entropy::getSingletonPtr()->mSceneMgr->destroyEntity(this->entity);
        delete this->entity;
    }
    this->entity = entity;

    ogreNode->attachObject(entity);
    ogreNode->setPosition(position);
}

void EnemyActor::move(int targetID)
{
    move(gameplay::astar.move(pathID,targetID,ogreNode->getPosition()));
}

void EnemyActor::move(Ogre::Vector3 nextPosition)
{
//    std::cout << "enemyactor move: " << nextPosition << std::endl;
}

void EnemyActor::setTarget(int id)
{
    ActorUpdateAction::ptr updateAction(new gameplay::PathFindUpdate(this,id));
    addUpdateAction(updateAction);
}

void EnemyActor::setTarget(PhysicsActor *actor)
{
    ActorUpdateAction::ptr updateAction(new gameplay::PathFindUpdate(this,actor));
    addUpdateAction(updateAction);
}

void EnemyActor::updateTransform()
{
    PhysicsActor::updateTransform();
    this->position = ogreNode->getPosition();
}


