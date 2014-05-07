// Ogre includes
#include "OGRE/OgreNode.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreManualObject.h"

// Bullet includes
#include "btBulletCollisionCommon.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

// Entropy includes
#include "physicsobject.h"
#include "Physics/physics.h"
#include "gameplay/actors.h"
#include "src/Core/Entropy.h"

namespace physics {

/////////////////
// PhysicsObject
/////////////////

PhysicsObject::PhysicsObject(btCollisionObject *object, collision_group_id_t collisionGroup,
                             CursorType cursorType, PhysicsMode physicsMode) :
    collisionGroup(collisionGroup),
    cursorType(cursorType),
    objectType("default"),
    mass(0),
    isStatic(false),
    physicsMode(physicsMode)
{
    this->object = object;

    if(object)
        object->setUserPointer(this);
}

void PhysicsObject::setPhysicsMode(PhysicsMode physicsMode)
{
    this->physicsMode = physicsMode;

    if(object)
        updatePhysicsMode(); // pure virtual call, defined in child
}

PhysicsMode PhysicsObject::getPhysicsMode()
{
    return physicsMode;
}

PhysicsObject::~PhysicsObject()
{
    if(object)
        physics::removeCollisionObject(object);
}

void PhysicsObject::setObject(btCollisionObject* object)
{
    this->object = object;
}


void PhysicsObject::setStatic()
{
    if(type() == Rigid)
    {
        mass = getMass();
        // World::world_upgrade_write_t lock(world.mutex);
        // World::world_write_t writeLock(lock);
        btRigidBody::upcast(object)->setMassProps(0, btVector3(0, 0, 0));
        btRigidBody::upcast(object)->updateInertiaTensor();
    }

    isStatic = true;
}

void PhysicsObject::setDynamic()
{
    if(type() == Rigid)
    {
        // World::world_upgrade_write_t lock(world.mutex);
        // World::world_write_t writeLock(lock);
        btRigidBody::upcast(object)->setMassProps(mass, btVector3(0, 0, 0));
        btRigidBody::upcast(object)->updateInertiaTensor();
    }

    isStatic = false;
}
void PhysicsObject::setCenterOfMassTransform(const btTransform &transform)
{
    if(type() == Rigid)
    {
        // World::world_upgrade_write_t lock(world.mutex);
        // World::world_write_t writeLock(lock);
        btRigidBody::upcast(object)->setCenterOfMassTransform(transform);
    }
}

void PhysicsObject::transform(const btTransform& transform) const
{
    // World::world_upgrade_write_t lock(world.mutex);
    // World::world_write_t writeLock(lock);
    this->object->setWorldTransform(transform);
}

void PhysicsObject::setWorldTransform(const btTransform& transform) const
{
    // World::world_upgrade_write_t lock(world.mutex);
    // World::world_write_t writeLock(lock);
    this->object->setWorldTransform(transform);
}

const btTransform& PhysicsObject::getWorldTransform() const
{
    // World::world_read_t readLock(world.mutex);
    return object->getWorldTransform();
}

void PhysicsObject::setCollisionFlags(int flags)
{
    // World::world_upgrade_write_t lock(world.mutex);
    // World::world_write_t writeLock(lock);
    object->setCollisionFlags(flags);
}

void PhysicsObject::setInterpolationLinearVelocity(const btVector3& linearVelocity) const
{
    // World::world_upgrade_write_t lock(world.mutex);
    // World::world_write_t writeLock(lock);
    this->object->setInterpolationLinearVelocity(linearVelocity);
}

void PhysicsObject::applyCentralImpulse(const btVector3& impulse)
{
    if(type() == Rigid)
    {
        // World::world_upgrade_write_t lock(world.mutex);
        // World::world_write_t writeLock(lock);
        btRigidBody::upcast(object)->applyCentralImpulse(impulse);
    }
}

void PhysicsObject::activate(bool forceActive)
{
    // World::world_upgrade_write_t lock(world.mutex);
    // World::world_write_t writeLock(lock);
    object->activate(forceActive);
}

BodyType PhysicsObject::type()
{
    // World::world_read_t readLock(world.mutex);

    switch(object->getInternalType())
    {
    case btCollisionObject::CO_RIGID_BODY:
        return Rigid;
        break;

    case btCollisionObject::CO_SOFT_BODY:
        return Soft;
        break;

    case btCollisionObject::CO_GHOST_OBJECT:
        return Ghost;
        break;

    default:
        return Collision;
        break;
    }

}

collision_group_id_t PhysicsObject::getCollisionGroup()
{
    return collisionGroup;
}

void PhysicsObject::setCollisionGroup(collision_group_id_t collisionGroup)
{
    this->collisionGroup = collisionGroup;
}

CursorType PhysicsObject::getCursorType()
{
    return cursorType;
}

void PhysicsObject::setCursorType(CursorType cursorType)
{
    this->cursorType = cursorType;
}

btCollisionObject* PhysicsObject::getObject()
{
    // World::world_read_t readLock(world.mutex);
    return object;
}

const btVector3& PhysicsObject::getPosition() const
{
    // World::world_read_t readLock(world.mutex);
    return object->getWorldTransform().getOrigin();
}

void PhysicsObject::setPosition(const btVector3& position, bool clearVelocity)
{
    // World::world_upgrade_write_t lock(world.mutex);
    // World::world_write_t writeLock(lock);
    btTransform transform = object->getWorldTransform();
    transform.setOrigin(position);
    object->setWorldTransform(transform);

    if(clearVelocity)
    {
        object->setInterpolationAngularVelocity(btVector3(0, 0, 0));
        object->setInterpolationLinearVelocity(btVector3(0, 0, 0));
    }
}

btQuaternion PhysicsObject::getRotation() const
{
    // World::world_read_t readLock(world.mutex);
    return object->getWorldTransform().getRotation();
}

float PhysicsObject::getMass()
{
    // World::world_read_t readLock(world.mutex);

    float mass;

    if(this->type() == Rigid)
        mass = 1.0 / btRigidBody::upcast(object)->getInvMass();
    else
        mass = 0;

    return mass;
}

std::string PhysicsObject::getObjectType()
{
    return objectType;
}

void PhysicsObject::updatePhysicsMode()
{
    btScalar physicsMultiplier;

    switch(physicsMode)
    {
    case StandardPhysics:
        physicsMultiplier = 1;
        break;

    case WaterPhysics:
        physicsMultiplier = 0.025;
        break;
    }

    switch(type())
    {
    case Rigid:
        btRigidBody::upcast(object)->setGravity(physics::world.getDynamicsWorld()->getGravity() * physicsMultiplier);
        break;

    case Soft:
        btSoftBody* softBody = btSoftBody::upcast(object);
        softBody->m_cfg.kDP = std::abs(physicsMultiplier - 1);
        break;
    }
}

////////////////
// GhostObject
////////////////

GhostObject::GhostObject(btGhostObject *object, collision_group_id_t collisionGroup, CursorType cursorType, bool addToSensorUpdates):
    PhysicsObject(object, collisionGroup, cursorType),
    addToSensorUpdates(addToSensorUpdates)
{
    if(addToSensorUpdates)
        physics::addSensorUpdate(this);
}

GhostObject::~GhostObject()
{
    if(addToSensorUpdates)
        physics::removeSensorUpdate(this);

    if(object)
        physics::removeCollisionObject(object);
}

void GhostObject::update()
{
    btAlignedObjectArray < btCollisionObject* >* overlappingObjects = &upcast()->getOverlappingPairs();
    collision_set_t collisionSetCopy = collisionSet;

    // Iterate over all the colliding objects
    for(int i = 0; i < overlappingObjects->size(); ++i)
    {
        btCollisionObject* collidingObject = overlappingObjects->at(i);

        // If we don't already have a pointer to an object, add to our original collisionSet, and finally call objectCollisionStarted()
        if(collisionSetCopy.find(collidingObject) == collisionSetCopy.end())
        {
            collisionSet.insert(collidingObject);
            objectCollisionStarted(collidingObject);
        }

        else // Otherwise remove it from our collisionSetCopy (this way we only end up with items left that were in the set before, but are no longer there: the collision has ended)
        {
            collisionSetCopy.erase(collidingObject);
        }
    }

    collision_set_iter_t iter = collisionSetCopy.begin();

    // Iterate over each object thats left in collisionSetCopy, call objectCollisionEnded(), and finally remove from the original collisionSet
    while(iter != collisionSetCopy.end())
    {
        btCollisionObject* collidingObject = *iter;
        objectCollisionEnded(collidingObject);
        collisionSet.erase(collidingObject);
        ++iter;
    }
}

btGhostObject* GhostObject::upcast()
{
    return btGhostObject::upcast(object);
}

void GhostObject::objectCollisionStarted(btCollisionObject* object)
{

}

void GhostObject::objectCollisionEnded(btCollisionObject* object)
{

}

///////////////
// RigidObject
///////////////

RigidObject::RigidObject(btRigidBody* object, collision_group_id_t collisionGroup, CursorType cursorType) :
    PhysicsObject(object, collisionGroup, cursorType)
{

}

RigidObject::~RigidObject()
{
    if(object)
        physics::removeCollisionObject(object);
}

void RigidObject::update()
{

}

void RigidObject::applyForce(const btVector3& forceVector, const btVector3& relPos) const
{
    // World::world_upgrade_write_t lock(world.mutex);
    // World::world_write_t writeLock(lock);
    btRigidBody::upcast(this->object)->applyForce(forceVector, relPos);
}

const btVector3& RigidObject::getLinearVelocity() const
{
    // World::world_read_t readLock(world.mutex);
    return btRigidBody::upcast(object)->getLinearVelocity();
}

void RigidObject::setLinearVelocity(const btVector3& linearVelocity) const
{
    // World::world_upgrade_write_t lock(world.mutex);
    // World::world_write_t writeLock(lock);
    btRigidBody::upcast(this->object)->setLinearVelocity(linearVelocity);
}

void RigidObject::getMotionStateWorldTransform(btTransform& transform) const
{
    // World::world_read_t readLock(world.mutex);
    btRigidBody::upcast(this->object)->getMotionState()->getWorldTransform(transform);
}

const btTransform& RigidObject::getWorldTransform() const
{
    // World::world_read_t readLock(world.mutex);

    btDefaultMotionState* motionState = (btDefaultMotionState*) btRigidBody::upcast(object)->getMotionState();
    return  motionState->m_graphicsWorldTrans;
}

const btVector3& RigidObject::getPosition() const
{
    // World::world_read_t readLock(world.mutex);

    btDefaultMotionState* motionState = (btDefaultMotionState*) btRigidBody::upcast(object)->getMotionState();
    return  motionState->m_graphicsWorldTrans.getOrigin();
}

void RigidObject::setPosition(const btVector3 &position, bool clearVelocity)
{
    // World::world_upgrade_write_t lock(world.mutex);
    // World::world_write_t writeLock(lock);

    btRigidBody* body = btRigidBody::upcast(object);
    btDefaultMotionState* motionState = (btDefaultMotionState*) body->getMotionState();
    motionState->m_graphicsWorldTrans.setOrigin(position);
    btTransform transform = body->getWorldTransform();
    transform.setOrigin(position);
    body->setWorldTransform(transform);

    if(clearVelocity)
    {
        body->setLinearVelocity(btVector3(0, 0, 0));
        body->setAngularVelocity(btVector3(0, 0, 0));
    }
}

btQuaternion RigidObject::getRotation() const
{
    // World::world_read_t readLock(world.mutex);

    btDefaultMotionState* motionState = (btDefaultMotionState*) btRigidBody::upcast(object)->getMotionState();
    return motionState->m_graphicsWorldTrans.getRotation();
}

BodyType RigidObject::type()
{
    return Rigid;
}

void RigidObject::updatePhysicsMode()
{
    PhysicsObject::updatePhysicsMode();
}

btRigidBody* RigidObject::upcastObject()
{
    btRigidBody* tempBody = btRigidBody::upcast(this->object);
    return tempBody;
}


/////////////////
// PhysicsActor
/////////////////

PhysicsActor::PhysicsActor(std::string name, btCollisionObject *object, Ogre::SceneNode* ogreNode,
                           collision_group_id_t collisionGroup, CursorType cursorType, bool addToActors,
                           bool autoFreeOgreNode) :
    PhysicsObject(object, collisionGroup, cursorType),
    name(name),
    ogreNode(ogreNode),
    autoFreeOgreNode(autoFreeOgreNode)
{
    if(addToActors)
    {
        gameplay::actors::addActor(this);
    }
}

PhysicsActor::~PhysicsActor()
{
    gameplay::actors::removeActor(name, false);

    if(ogreNode && autoFreeOgreNode)
        Entropy::getSingletonPtr()->mSceneMgr->destroySceneNode(ogreNode);

    if(object)
        physics::removeCollisionObject(object);
}

std::string PhysicsActor::getName()
{
    return name;
}

void PhysicsActor::update()
{
//    std::cout << "PhysicsActor::update(): " << updateActions.size() << std::endl;
    for(unsigned int i = 0; i < updateActions.size(); ++i)
    {
        updateActions[i]->update();
    }

    //updateTransform();
}

void PhysicsActor::addUpdateAction(gameplay::ActorUpdateAction::ptr updateAction)
{
    int position;
    if(findUpdateTypePosition(updateAction->type(), position) == false)
    {
//        std::cout << "UPDATE ACTION ADDED" << std::endl;
        updateAction->setActor(this);
        updateActions.push_back(updateAction);
    }

}

void PhysicsActor::removeUpdateAction(gameplay::UpdateActionType type)
{
    int position;
    if(findUpdateTypePosition(type, position) == false)
    {
        updateActions.erase(updateActions.begin() + position);
    }
}

void PhysicsActor::updateTransform()
{
    try
    {
        if(object && ogreNode)
        {
            btQuaternion rotation;
            btVector3 position;

            if(this->type() == physics::Rigid)
            {
                btDefaultMotionState* motionState = (btDefaultMotionState*) btRigidBody::upcast(object)->getMotionState();
                rotation = motionState->m_graphicsWorldTrans.getRotation();
                position = motionState->m_graphicsWorldTrans.getOrigin();
            }

            else
            {
                btTransform transform = object->getWorldTransform();
                rotation = transform.getRotation();
                position = transform.getOrigin();
            }

            ogreNode->setOrientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());
            ogreNode->setPosition(position.x(), position.y(), position.z());
        }
    }

    catch(...)
    {
        std::cerr << "ERROR in PhysicsActor::updateTransform()" << std::endl;
    }
}

void PhysicsActor::bindOgreNode(Ogre::SceneManager *sceneManager)
{
    if(sceneManager->hasSceneNode(name))
    {
        ogreNode = sceneManager->getSceneNode(name);
    }
}

Ogre::SceneNode *PhysicsActor::getOgreNode()
{
    return ogreNode;
}

bool PhysicsActor::findUpdateTypePosition(gameplay::UpdateActionType type, int& position)
{
    for(unsigned int i = 0; i < updateActions.size(); ++i)
    {
        if(updateActions[i]->type() == type)
        {
            position = i;
            return true;
        }
    }

    return false;
}

void PhysicsActor::setPathID(int pathID)
{
    this->pathID = pathID;
}

int PhysicsActor::getPathID() const
{
    return pathID;
}

void PhysicsActor::move(int targetID)
{
    std::cout << "PhysicsActor::move(int targetID)" << std::endl;
}

void PhysicsActor::updatePhysicsMode()
{
    PhysicsObject::updatePhysicsMode();
}

/////////////////////
// SoftPhysicsActor
/////////////////////

SoftPhysicsActor::SoftPhysicsActor(std::string name, btSoftBody* object, Ogre::ManualObject* ogreObject,
                                   collision_group_id_t collisionGroup, CursorType cursorType,
                                   bool addToActors) :
    PhysicsActor(name, object, 0, collisionGroup, cursorType, addToActors, false),
    ogreObject(ogreObject)
{

}

void SoftPhysicsActor::updateTransform()
{
    if(object && ogreObject)
    {
        if(ogreObject->getDynamic())
        {
            ogreObject->beginUpdate(0);
            btSoftBody* body = upcastObject();

            for(int i = 0; i < body->m_nodes.size(); ++i)
            {
                ogreObject->position(physics::Vec3(body->m_nodes.at(i).m_x).toOgre());
            }

            ogreObject->end();
        }
    }
}

btSoftBody* SoftPhysicsActor::upcastObject()
{
    return btSoftBody::upcast(object);
}

void SoftPhysicsActor::updatePhysicsMode()
{
    PhysicsObject::updatePhysicsMode();
}

} // physics namespace
