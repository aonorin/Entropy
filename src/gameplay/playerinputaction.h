#ifndef PLAYERINPUTACTION_H
#define PLAYERINPUTACTION_H

// System includes
#ifdef WIN32
#include <unordered_map>
#include <functional>
#else
#include <tr1/unordered_map>
#include <tr1/functional>
#endif

#include <string>
#include <vector>

// Boost includes
#include "boost/shared_ptr.hpp"

// Entropy includes
#include "src/Utility/ois2sdl.h"
#include "Physics/physicstypes.h"
#include "src/gameplay/CollisionAction.h"

// Bullet forward declarations
class btSoftBody;
class btRigidBody;
class btCollisionObject;
class btPoint2PointConstraint;
class btHingeConstraint;
class btGeneric6DofConstraint;
class btGeneric6DofSpringConstraint;

// Ogre forward declarations
namespace Ogre
{
    class ManualObject;
    class Vector3;
    class SceneNode;
}

namespace physics
{
    class PhysicsObject;
    class SoftPhysicsActor;
    class PhysicsActor;
}

namespace gameplay {

/////////////////////
// PlayerInputAction
/////////////////////

class PlayerInputAction
{
public:

    typedef boost::shared_ptr<PlayerInputAction> action_ptr;
    typedef std::vector<action_ptr> action_vector_t;

#ifdef WIN32
    typedef std::function<void ()> action_func_t;
    typedef std::unordered_map<utility::EntKeyCode, action_func_t> action_func_map_t;
    typedef std::unordered_map<std::string, action_ptr> action_map_t;
#else
    typedef std::tr1::function<void ()> action_func_t;
    typedef std::tr1::unordered_map<int, action_func_t> action_func_map_t;
    typedef std::tr1::unordered_map<std::string, action_ptr> action_map_t;
#endif

    virtual void update() {}
    bool isScheduledForUpdates();
    std::string getName();
    const action_func_map_t& getKeyDownMap() const;
    const action_func_map_t& getKeyUpMap() const;

    static void add(PlayerInputAction* action); // actionMap takes ownership. DON'T FREE IT YOURSELF!
    static void remove(std::string& name);
    static bool has(std::string& name);
    static void clear(); // clear the actionMap and funcMap;
    static void keyDown(utility::EntKeyCode keyCode);
    static void keyUp(utility::EntKeyCode keyCode);
    static void updateActions();

    virtual ~PlayerInputAction();

protected:

    /**
     * @brief PlayerInputAction abstract class for actions based off user input via keyboard and mouse
     * @param name The name of the action, this is important and guarantees only one instance.
     * @param scheduledForUpdates If the action will have it's update method called in the update loop.
     */
    PlayerInputAction(std::string& name, bool scheduledForUpdates); // Protected constructor.

    static action_func_map_t keyDownMap;
    static action_func_map_t keyUpMap;
    static action_map_t actionMap;
    static action_vector_t actionVector;

    action_func_map_t mKeyDownMap;
    action_func_map_t mKeyUpMap;
    std::string name;
    bool scheduledForUpdates;
};

/////////////////
// GrapplingHook
/////////////////

// RopeActor
class RopeActor : public physics::SoftPhysicsActor
{
public:

    RopeActor(std::string name, btSoftBody* object = 0, Ogre::ManualObject* ogreObject = 0,
              physics::collision_group_id_t collisionGroup = physics::MAIN_GROUP_ID,
              physics::CursorType cursorType = physics::NoCursor,
              bool addToActors = true);

    virtual void updateTransform();
    void setObject(btSoftBody* body);
    void setVisible(bool visible);

private:

    void createLink(btVector3 link);
    void addTriangleToMesh(int linkNum, int v0, int v1, int v2);

    std::vector<Ogre::Vector3> linkPoints; // Points to make a single link
    double scale;
    int numLinks;
    bool visible;
};

// GrapplingHook
class GrapplingHook : public PlayerInputAction
{
    // Private constructor. Use the static create method.
    GrapplingHook();

public:

    ~GrapplingHook();

    // GrapplingHookMode
    enum GrapplingHookMode
    {
        NoHook,
        DynamicLift,
        DynamicDrag,
        StaticPull
    };

    // GrapplingHookCollision
    class GrapplingHookCollision : public gameplay::CollisionAction
    {
    public:

        GrapplingHookCollision(GrapplingHook* grapplingHook, physics::collision_group_id_t group0,
                               physics::collision_group_id_t group1, physics::collision_group_id_t collisionCombinationInt);

        virtual void collide(physics::PhysicsObject* object0, physics::PhysicsObject* object1);
        virtual CollisionActionType type();

    private:

        GrapplingHook* grapplingHook;
    };

    friend class GrapplingHookCollision;

    virtual void update();

    static void create();
    static std::string name;

private:

    void fire();
    void retract();
    void detach();
    void attach();
    void updateRopeMesh();
    void updateHookViewBufferTransform();
    void addTriangleToMesh(Ogre::ManualObject* object, std::vector<Ogre::Vector3>& points, int v0, int v1, int v2);
    void grappleTarget(btRigidBody* target, btVector3 hitPosition, btVector3 hitNormal);
    void dynamicLift();
    void dynamicDrag(const btVector3& hookPosition);
    void staticPull(const btVector3& hookPosition, const btVector3& hookNormal);
    void createHook();
    void createRope(btRigidBody* origin, const btVector3& hookPosition);
    void addCollisionResponses();
    void addCollisionResponse(physics::collision_group_id_t group0, physics::collision_group_id_t group1);
    void removeCollisionResponses();
    void destroyBodies();
    void destroyHook();
    void destroyRope();
    void createHookConstraint();
    void destroyHookConstraint();

    bool fired;
    bool attached;
    btVector3 targetSize;
    btSoftBody* rope;
    btRigidBody* hook;
    btRigidBody* target;
    float targetLinearFactor;
    physics::RigidObject* internalLiftSphere;
    float internalSphereMinOffsetY;
    btRigidBody* hookViewBuffer; // Buffer so that the constraint doesn't squish the players view into the object that's grabbed, also makes the grappled object follow view direction.
    RopeActor* ropeActor; // Ogre/Bullet object
    physics::PhysicsActor* hookActor; // Ogre/Bullet object
    btPoint2PointConstraint* hookConstraint;
    btHingeConstraint* hookTargetConstraint;
    btGeneric6DofConstraint* hookBufferConstraint;
    std::vector<GrapplingHookCollision*> collisionActions;
    physics::collision_group_id_t ropeCollisionGroupID;
    physics::collision_group_id_t hookCollisionGroupID;
    physics::collision_group_id_t hookMainCollisionCombination;
    std::string hookCollisionGroupName;
    int updateCounter, newUpdateCount, ropeLinkCounter;
    Ogre::ManualObject *ropeModel, *hookModel;
    Ogre::SceneNode *ropeNode, *hookNode;
    GrapplingHookMode hookMode;
};

} // gameplay

#endif // PLAYERINPUTACTION_H
