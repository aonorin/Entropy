#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

// System includes
#include <string>
#include <vector>
#ifdef __APPLE__
#include <tr1/unordered_set>
#else
#include <unordered_set>
#endif

// Bullet includes
#include "LinearMath/btTransform.h"
#include "boost/thread/shared_mutex.hpp"

// Entropy includes
#include "src/Physics/physicstypes.h"
#include "src/gameplay/actorupdateactions.h"


// Forward declarations
class btRigidBody;
class btGhostObject;
class btCollisionShape;
class btCollisionObject;
class btSoftBody;

namespace Ogre {
    class SceneNode;
    class ManualObject;
    class SceneManager;
} // Ogre namespace

namespace physics {

class PhysicsObject
{
public:

    PhysicsObject(btCollisionObject* object = 0, collision_group_id_t collisionGroup = MAIN_GROUP_ID,
                  CursorType cursorType = NoCursor, PhysicsMode physicsMode = StandardPhysics);
    virtual ~PhysicsObject();

    virtual void update() = 0; // OVERRIDE ME!

    void setPhysicsMode(PhysicsMode physicsMode);
    PhysicsMode getPhysicsMode();
    void setObject(btCollisionObject* object);
    void setStatic();
    void setDynamic();
    void transform(const btTransform& transform) const;
    void setCenterOfMassTransform(const btTransform& transform);
    void setWorldTransform(const btTransform& transform) const;
    void setCollisionFlags(int flags);
    virtual const btTransform& getWorldTransform() const;
    void setInterpolationLinearVelocity(const btVector3& linearVelocity) const;
    void applyCentralImpulse(const btVector3& impulse);
    void activate(bool forceActive = false);
    btCollisionObject* getObject();
    virtual const btVector3& getPosition() const;
    virtual void setPosition(const btVector3& position, bool clearVelocity = true);
    virtual btQuaternion getRotation() const;
    float getMass();

    virtual BodyType type();
    collision_group_id_t getCollisionGroup();
    void setCollisionGroup(collision_group_id_t collisionGroup);
    CursorType getCursorType();
    void setCursorType(CursorType cursorType);
    std::string getObjectType();

protected:

    virtual void updatePhysicsMode(); // OVERRIDE ME!

    std::string objectType;
    btCollisionObject* object;
    collision_group_id_t collisionGroup;
    CursorType cursorType;
    boost::shared_mutex mutex;
    float mass;
    bool isStatic;
    PhysicsMode physicsMode;
};

// Useful for objects that don't want physics interactions, but do want to know when objects beging to and end colliding with it.
class GhostObject : public PhysicsObject
{
public:

#ifdef __APPLE__
    typedef std::tr1::unordered_set<btCollisionObject*> collision_set_t;
#else
    typedef std::tr1::unordered_set<btCollisionObject*> collision_set_t;
#endif
    typedef collision_set_t::iterator collision_set_iter_t;

    GhostObject(btGhostObject* object = 0, collision_group_id_t collisionGroup = MAIN_GROUP_ID, CursorType cursorType = NoCursor, bool addToSensorUpdates = true);
    ~GhostObject();

    virtual void update(); // overridden
    btGhostObject* upcast();

protected:

    virtual void objectCollisionStarted(btCollisionObject* object); // begin collision callback
    virtual void objectCollisionEnded(btCollisionObject* object); // end collision callback

    collision_set_t collisionSet; // Set of currently colliding objects
    bool addToSensorUpdates; // Updated by physics in the update loop
};

class RigidObject : public PhysicsObject
{
public:

    RigidObject(btRigidBody* object = 0, collision_group_id_t collisionGroup = MAIN_GROUP_ID,
                CursorType cursorType = NoCursor);

    ~RigidObject();

    void update(); // overridden
    void applyForce(const btVector3& forceVector, const btVector3& relPos) const;
    const btVector3& getLinearVelocity() const;
    void setLinearVelocity(const btVector3& linearVelocity) const;
    void getMotionStateWorldTransform(btTransform& transform) const;

    virtual const btTransform& getWorldTransform() const;
    virtual const btVector3& getPosition() const;
    virtual void setPosition(const btVector3& position, bool clearVelocity = true);
    virtual btQuaternion getRotation() const;
    virtual BodyType type();

    btRigidBody* upcastObject();

protected:

    virtual void updatePhysicsMode();
};

class PhysicsActor : public PhysicsObject
{
public:

    PhysicsActor(std::string name, btCollisionObject* object = 0, Ogre::SceneNode* ogreNode = 0,
                 collision_group_id_t collisionGroup = MAIN_GROUP_ID, CursorType cursorType = NoCursor,
                 bool addToActors = true, bool autoFreeOgreNode = true);
    ~PhysicsActor();

    std::string getName();
    virtual void update(); // overridden
    virtual void updateTransform();
    void addUpdateAction(gameplay::ActorUpdateAction::ptr updateAction); // No duplicate types! Duplicates are discarded!
    void removeUpdateAction(gameplay::UpdateActionType type); // Since there are no duplicate types you only reference the type
    Ogre::SceneNode* getOgreNode();
    void bindOgreNode(Ogre::SceneManager* sceneManager); // Provide the scene manager so the actor can bind to the Ogre::Node
    void setPathID(int pathID);
    int getPathID() const;
    virtual void move(int targetID);

    bool autoFreeOgreNode;

protected:

    virtual void updatePhysicsMode();

    int pathID;
    Ogre::SceneNode* ogreNode; // pointer to the ogre entity bound to this actor
    std::string name;

private:

    bool findUpdateTypePosition(gameplay::UpdateActionType type, int& position); // returns true if the type is found, sets position
    std::vector<gameplay::ActorUpdateAction::ptr> updateActions; // Collection of update actions for this actor

};

class SoftPhysicsActor : public PhysicsActor
{
public:

    SoftPhysicsActor(std::string name, btSoftBody* object = 0, Ogre::ManualObject* ogreObject = 0,
                     collision_group_id_t collisionGroup = MAIN_GROUP_ID, CursorType cursorType = NoCursor,
                     bool addToActors = true);

    virtual void updateTransform();
    btSoftBody* upcastObject();

protected:

    virtual void updatePhysicsMode();

    Ogre::ManualObject* ogreObject;
};

} // physics namespace


#endif // PHYSICSOBJECT_H
