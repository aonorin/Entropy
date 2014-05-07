#include "playerinputaction.h"
#include "BulletSoftBody/btSoftBody.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h"
#include "src/Physics/physics.h"
#include "src/Core/collisioncamera.h"
#include "src/Physics/physicsobject.h"
#include "src/Core/Entropy.h"
#include "src/gameplay/actors.h"
#include "src/Physics/physicstypes.h"

// Ogre includes
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreVector3.h"
#include "OGRE/OgreSceneNode.h"

#ifdef __APPLE__
#include "tr1/functional"
#endif

using namespace std;
#ifndef WIN32
using namespace std::tr1;
#endif

namespace gameplay {

/////////////////////
// PlayerInputAction
/////////////////////

PlayerInputAction::action_func_map_t PlayerInputAction::keyDownMap;
PlayerInputAction::action_func_map_t PlayerInputAction::keyUpMap;
PlayerInputAction::action_map_t PlayerInputAction::actionMap;
PlayerInputAction::action_vector_t PlayerInputAction::actionVector;

PlayerInputAction::PlayerInputAction(std::string& name, bool scheduledForUpdates) :
    name(name),
    scheduledForUpdates(scheduledForUpdates)
{

}

PlayerInputAction::~PlayerInputAction()
{

}

bool PlayerInputAction::isScheduledForUpdates()
{
    return scheduledForUpdates;
}

std::string PlayerInputAction::getName()
{
    return name;
}

const PlayerInputAction::action_func_map_t& PlayerInputAction::getKeyDownMap() const
{
    return mKeyDownMap;
}

const PlayerInputAction::action_func_map_t& PlayerInputAction::getKeyUpMap() const
{
    return mKeyUpMap;
}

void PlayerInputAction::add(PlayerInputAction* action)
{
    action_ptr ptr(action);
    PlayerInputAction::actionMap[action->getName()] = ptr; // add to our map

    if(ptr->isScheduledForUpdates())
        PlayerInputAction::actionVector.push_back(ptr); // add to our update vector

    const action_func_map_t& keyDownFunctions = action->getKeyDownMap();
    const action_func_map_t& keyUpFunctions = action->getKeyUpMap();

    // Append the global function map with all of the functions in the local map
    PlayerInputAction::keyDownMap.insert(keyDownFunctions.begin(), keyDownFunctions.end());
    PlayerInputAction::keyUpMap.insert(keyUpFunctions.begin(), keyUpFunctions.end());
}

void PlayerInputAction::remove(std::string& name)
{
    action_ptr action = PlayerInputAction::actionMap[name];
    const action_func_map_t& keyDownFunctions = action->getKeyDownMap();
    const action_func_map_t& keyUpFunctions = action->getKeyUpMap();

    // Append the global function map with all of the functions in the local map
    PlayerInputAction::keyDownMap.erase(keyDownFunctions.begin(), keyDownFunctions.end());
    PlayerInputAction::keyUpMap.erase(keyUpFunctions.begin(), keyUpFunctions.end());

    PlayerInputAction::actionMap.erase(action->getName()); // Remove the action itself from the map

    if(action->isScheduledForUpdates())
    {
        // Remove the action from the update vector
        for(unsigned int i = 0; i < PlayerInputAction::actionVector.size(); ++i)
        {
            action_ptr ptr = PlayerInputAction::actionVector.at(i);
            if(ptr->getName().compare(name) == 0)
            {
                PlayerInputAction::actionVector.erase(PlayerInputAction::actionVector.begin() + i);
                break;
            }
        }
    }
}

bool PlayerInputAction::has(std::string& name)
{
    return PlayerInputAction::actionMap.find(name) != PlayerInputAction::actionMap.end();
}

void PlayerInputAction::clear()
{
    PlayerInputAction::keyDownMap.clear();
    PlayerInputAction::keyUpMap.clear();
    PlayerInputAction::actionMap.clear();
    PlayerInputAction::actionVector.clear();
}

void PlayerInputAction::keyDown(utility::EntKeyCode keyCode)
{
    if(PlayerInputAction::keyDownMap.find(keyCode) != PlayerInputAction::keyDownMap.end())
    {
        PlayerInputAction::keyDownMap[keyCode]();
    }
}

void PlayerInputAction::keyUp(utility::EntKeyCode keyCode)
{
    if(PlayerInputAction::keyUpMap.find(keyCode) != PlayerInputAction::keyUpMap.end())
    {
        PlayerInputAction::keyUpMap[keyCode]();
    }
}

void PlayerInputAction::updateActions()
{
    for(unsigned int i = 0; i < PlayerInputAction::actionVector.size(); ++i)
    {
        PlayerInputAction::actionVector.at(i)->update();
    }
}

/////////////////
// GrapplingHook
/////////////////

std::string GrapplingHook::name = "GrapplingHook";

// GrapplingHookCollision
GrapplingHook::GrapplingHookCollision::GrapplingHookCollision(GrapplingHook* grapplingHook,
                                                              physics::collision_group_id_t group0,
                                                              physics::collision_group_id_t group1,
                                                              physics::collision_group_id_t collisionCombinationInt) :
    gameplay::CollisionAction(group0, group1, collisionCombinationInt),
    grapplingHook(grapplingHook)
{

}

void GrapplingHook::GrapplingHookCollision::collide(physics::PhysicsObject* object0,
                                                    physics::PhysicsObject* object1)
{
    btRigidBody* body0 = btRigidBody::upcast(object0->getObject());
    btRigidBody* body1 = btRigidBody::upcast(object1->getObject());

    btVector3 nullVector(0, 0, 0);

    body0->setLinearVelocity(nullVector);
    body1->setLinearVelocity(nullVector);

    if(object0->getCollisionGroup() != grapplingHook->hookCollisionGroupID)
    {
        // grapplingHook->grappleTarget(body0);
    }

    else
    {
        // grapplingHook->grappleTarget(body1);
    }

    grapplingHook->removeCollisionResponses();
}

gameplay::CollisionActionType GrapplingHook::GrapplingHookCollision::type()
{
    return GrapplingHookCollisionAction;
}

// RopeActor
RopeActor::RopeActor(std::string name, btSoftBody* object, Ogre::ManualObject* ogreObject,
          physics::collision_group_id_t collisionGroup, physics::CursorType cursorType, bool addToActors) :
    physics::SoftPhysicsActor(name, object, ogreObject, collisionGroup, cursorType, addToActors),
    scale(0.25),
    numLinks(20),
    visible(false)
{
    // ogreObject->clear();
    // Create the points that make up an Icosahedron
    double t = (1.0 + std::sqrt(5.0)) / 2.0;

    linkPoints.push_back(Ogre::Vector3(-1,  t,  0) * scale);
    linkPoints.push_back(Ogre::Vector3( 1,  t,  0) * scale);
    linkPoints.push_back(Ogre::Vector3(-1, -t,  0) * scale);
    linkPoints.push_back(Ogre::Vector3( 1, -t,  0) * scale);
    linkPoints.push_back(Ogre::Vector3( 0, -1,  t) * scale);
    linkPoints.push_back(Ogre::Vector3( 0,  1,  t) * scale);
    linkPoints.push_back(Ogre::Vector3( 0, -1, -t) * scale);
    linkPoints.push_back(Ogre::Vector3( 0,  1, -t) * scale);
    linkPoints.push_back(Ogre::Vector3( t,  0, -1) * scale);
    linkPoints.push_back(Ogre::Vector3( t,  0,  1) * scale);
    linkPoints.push_back(Ogre::Vector3(-t,  0, -1) * scale);
    linkPoints.push_back(Ogre::Vector3(-t,  0,  1) * scale);

    // Create the triangle list for the icosphere
    ogreObject->begin("Material.001", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

    // Buffer points
    for(int l = 0; l < numLinks; ++l)
    {
        for(int i = 0; i < linkPoints.size(); ++i)
        {
            ogreObject->position(linkPoints.at(i));
        }
    }

    for(int l = 0; l < numLinks; ++l)
    {
        // buffer indexes

        // 5 faces around point 0
        addTriangleToMesh(l, 0, 11, 5);
        addTriangleToMesh(l, 0, 5, 1);
        addTriangleToMesh(l, 0, 1, 7);
        addTriangleToMesh(l, 0, 7, 10);
        addTriangleToMesh(l, 0, 10, 11);

        // 5 adjacent faces
        addTriangleToMesh(l, 1, 5, 9);
        addTriangleToMesh(l, 5, 11, 4);
        addTriangleToMesh(l, 11, 10, 2);
        addTriangleToMesh(l, 10, 7, 6);
        addTriangleToMesh(l, 7, 1, 8);

        // 5 faces around point 3
        addTriangleToMesh(l, 3, 9, 4);
        addTriangleToMesh(l, 3, 4, 2);
        addTriangleToMesh(l, 3, 2, 6);
        addTriangleToMesh(l, 3, 6, 8);
        addTriangleToMesh(l, 3, 8, 9);

        // 5 adjacent faces
        addTriangleToMesh(l, 4, 9, 5);
        addTriangleToMesh(l, 2, 4, 11);
        addTriangleToMesh(l, 6, 2, 10);
        addTriangleToMesh(l, 8, 6, 7);
        addTriangleToMesh(l, 9, 8, 1);
    }

    ogreObject->end();
}

void RopeActor::updateTransform()
{
    if(ogreObject)
    {
        if(this->visible)
        {
            ogreObject->beginUpdate(0);
            btSoftBody* body = upcastObject();

            for(int i = 0; i < body->m_nodes.size(); ++i)
            {
                createLink(body->m_nodes.at(i).m_x);
            }

            ogreObject->end();
        }
    }
}

void RopeActor::setObject(btSoftBody* body)
{
    this->object = body;
}

void RopeActor::setVisible(bool visible)
{
    if(this->visible != visible)
    {
        if(visible)
        {
            gameplay::actors::addActor(this);
        }

        else
        {
            gameplay::actors::removeActor(name, false);
        }
    }

    this->visible = visible;
    ogreObject->setVisible(visible);
}

void RopeActor::createLink(btVector3 link)
{

    for(int i = 0; i < linkPoints.size(); ++i)
    {
        ogreObject->position(physics::Vec3(link).toOgre() + linkPoints.at(i));
    }
}

void RopeActor::addTriangleToMesh(int linkNum, int v0, int v1, int v2)
{
    ogreObject->index((linkNum * 12) + v0);
    ogreObject->index((linkNum * 12) + v1);
    ogreObject->index((linkNum * 12) + v2);

    Ogre::Vector3 u = linkPoints.at(v1) - linkPoints.at(v0);
    Ogre::Vector3 v = linkPoints.at(v2) - linkPoints.at(v0);

    Ogre::Vector3 normal(
        (u.y * v.z) - (u.z * v.y),
        (u.z * v.x) - (u.x * v.z),
        (u.x * v.y) - (u.y * v.x)
    );

    ogreObject->normal(normal);
    ogreObject->normal(normal);
    ogreObject->normal(normal);
}

// GrapplingHook
GrapplingHook::GrapplingHook() :
    PlayerInputAction(GrapplingHook::name, true),
    fired(false),
    attached(false),
    // hook(0),
    rope(0),
    hookActor(0),
    ropeActor(0),
    hookConstraint(0),
    hookTargetConstraint(0),
    hookBufferConstraint(0),
    hookViewBuffer(0),
    updateCounter(0),
    newUpdateCount(0),
    ropeLinkCounter(0),
    hookCollisionGroupName("GrapplingHook"),
    ropeModel(0),
    hookModel(0),
    ropeNode(0),
    hookNode(0),
    target(0),
    internalLiftSphere(0),
    hookMode(NoHook)
{
#ifdef __APPLE__
    mKeyDownMap[utility::MB_Left] = std::tr1::bind(&GrapplingHook::fire, this);
    mKeyUpMap[utility::MB_Left] = std::tr1::bind(&GrapplingHook::retract, this);
#else
    mKeyDownMap[utility::MB_Left] = std::bind(&GrapplingHook::fire, this);
    mKeyUpMap[utility::MB_Left] = std::bind(&GrapplingHook::retract, this);
#endif
    hookCollisionGroupID = physics::CollisionGroupManager::addCollisionGroup(hookCollisionGroupName);
    ropeCollisionGroupID = physics::EQUIPMENT_GROUP_ID;
    hookMainCollisionCombination = hookCollisionGroupID | physics::MAIN_GROUP_ID;

    hookModel = new Ogre::ManualObject("hookModel");
    hookModel->setDynamic(false);


    // Create the points that make up an Icosahedron
    double t = (1.0 + std::sqrt(5.0)) / 2.0;
    std::vector<Ogre::Vector3> icoPoints;

    icoPoints.push_back(Ogre::Vector3(-1,  t,  0));
    icoPoints.push_back(Ogre::Vector3( 1,  t,  0));
    icoPoints.push_back(Ogre::Vector3(-1, -t,  0));
    icoPoints.push_back(Ogre::Vector3( 1, -t,  0));
    icoPoints.push_back(Ogre::Vector3( 0, -1,  t));
    icoPoints.push_back(Ogre::Vector3( 0,  1,  t));
    icoPoints.push_back(Ogre::Vector3( 0, -1, -t));
    icoPoints.push_back(Ogre::Vector3( 0,  1, -t));
    icoPoints.push_back(Ogre::Vector3( t,  0, -1));
    icoPoints.push_back(Ogre::Vector3( t,  0,  1));
    icoPoints.push_back(Ogre::Vector3(-t,  0, -1));
    icoPoints.push_back(Ogre::Vector3(-t,  0,  1));

    // Create the triangle list for the icosphere
    hookModel->begin("Material.001", Ogre::RenderOperation::OT_TRIANGLE_LIST);

    // Buffer points
    for(int i = 0; i < icoPoints.size(); ++i)
    {
        hookModel->position(icoPoints.at(i));
    }

    // buffer indexes

    // 5 faces around point 0
    addTriangleToMesh(hookModel, icoPoints, 0, 11, 5);
    addTriangleToMesh(hookModel, icoPoints, 0, 5, 1);
    addTriangleToMesh(hookModel, icoPoints, 0, 1, 7);
    addTriangleToMesh(hookModel, icoPoints, 0, 7, 10);
    addTriangleToMesh(hookModel, icoPoints, 0, 10, 11);

    // 5 adjacent faces
    addTriangleToMesh(hookModel, icoPoints, 1, 5, 9);
    addTriangleToMesh(hookModel, icoPoints, 5, 11, 4);
    addTriangleToMesh(hookModel, icoPoints, 11, 10, 2);
    addTriangleToMesh(hookModel, icoPoints, 10, 7, 6);
    addTriangleToMesh(hookModel, icoPoints, 7, 1, 8);

    // 5 faces around point 3

    addTriangleToMesh(hookModel, icoPoints, 3, 9, 4);
    addTriangleToMesh(hookModel, icoPoints, 3, 4, 2);
    addTriangleToMesh(hookModel, icoPoints, 3, 2, 6);
    addTriangleToMesh(hookModel, icoPoints, 3, 6, 8);
    addTriangleToMesh(hookModel, icoPoints, 3, 8, 9);

    // 5 adjacent faces
    addTriangleToMesh(hookModel, icoPoints, 4, 9, 5);
    addTriangleToMesh(hookModel, icoPoints, 2, 4, 11);
    addTriangleToMesh(hookModel, icoPoints, 6, 2, 10);
    addTriangleToMesh(hookModel, icoPoints, 8, 6, 7);
    addTriangleToMesh(hookModel, icoPoints, 9, 8, 1);

    hookModel->end();

    // Create the ogre objects
    hookNode = Entropy::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode("hook");
    hookNode->attachObject(hookModel);
    hookNode->setVisible(false);

    // Create rope model
    ropeModel = new Ogre::ManualObject("ropeModel");
    ropeModel->setDynamic(true);

    /*
    ropeModel->begin("Material.001", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

    for(int i = 0; i < 20; ++i)
    {
        ropeModel->position(0, 0, 0);
        ropeModel->index(i);
    }

    ropeModel->end();*/
    // ropeModel->setCastShadows(true);
    ropeModel->setVisible(false);

    ropeActor = new RopeActor("GrapplingHookRope", rope, ropeModel, ropeCollisionGroupID, physics::NoCursor, true);
    Entropy::getSingletonPtr()->mSceneMgr->getRootSceneNode()->attachObject(ropeModel);
}

GrapplingHook::~GrapplingHook()
{
    hookNode->detachAllObjects();
    removeCollisionResponses();
    destroyBodies();
    destroyHookConstraint();

    Entropy::getSingletonPtr()->mSceneMgr->destroySceneNode(hookNode);
    delete hookModel;
    hookModel = 0;

    if(ropeActor)
    {
        delete ropeActor;
        ropeActor = 0;
    }

    Entropy::getSingletonPtr()->mSceneMgr->getRootSceneNode()->detachObject(ropeModel);
    delete ropeModel;
    ropeModel = 0;
}

void GrapplingHook::update()
{
    if(hookMode == DynamicLift)
    {
        if(target && fired)
        {
            if(targetLinearFactor < 1.0)
            {
                targetLinearFactor *= 2;
                target->setLinearFactor(btVector3(targetLinearFactor, targetLinearFactor, targetLinearFactor));
            }

            if(targetLinearFactor > 1.0)
            {
                targetLinearFactor = 1.0;
                target->setLinearFactor(btVector3(targetLinearFactor, targetLinearFactor, targetLinearFactor));
            }

            if(!target->isStaticObject())
                target->activate(true);
        }

        if(internalLiftSphere)
        {
            CollisionCamera* player = CollisionCamera::getSingletonPtr();
            btVector3 playerPosition = player->getPosition();
            btVector3 spherePosition = playerPosition + (physics::Vec3(player->getCamera()->getDirection()) * targetSize * 1.1);
            spherePosition.setY(std::max(playerPosition.y() + internalSphereMinOffsetY, spherePosition.y()));
            internalLiftSphere->setPosition(spherePosition);
        }
    }

    else if(hookMode == StaticPull)
    {
        btRigidBody* player = CollisionCamera::getSingletonPtr()->upcastObject();
        float linearFactor = player->getLinearFactor().y();

        if(linearFactor < 1)
            linearFactor *= 1.3;
        else if(linearFactor > 1)
            linearFactor = 1;

        player->setLinearFactor(btVector3(linearFactor, linearFactor, linearFactor));

        /*
        if(++updateCounter >= newUpdateCount)
        {
            updateCounter = 0;

            if(ropeLinkCounter >= 0)
            {
                // int begin = ropeLinkCounter;
                btSoftBody::Node* endNode = &rope->m_nodes.at(rope->m_nodes.size() - 1);
                btSoftBody::Node* node = &rope->m_nodes.at(ropeLinkCounter);
                // rope->appendAnchor(ropeLinkCounter, target);
                node->m_x = endNode->m_x;


                for(int i = begin - 1; i >= 0; --i)
                {
                    btSoftBody::Node* node = &rope->m_nodes.at(i);
                    node->m_x = endNode->m_x;
                    // rope->appendAnchor(ropeLinkCounter, btRigidBody::upcast(player->getObject()));
                }

                ropeLinkCounter--;
            }
        }*/
    }

    /*
    CollisionCamera* player = CollisionCamera::getSingletonPtr();


    if(rope)
    {
        if(++updateCounter >= newUpdateCount)
        {
            updateCounter = 0;

            if(staticTarget)
            {
                if(ropeLinkCounter >= 0)
                {
                    int begin = ropeLinkCounter;
                    btSoftBody::Node* endNode = &rope->m_nodes.at(begin);

                    for(int i = begin - 1; i >= 0; --i)
                    {
                        btSoftBody::Node* node = &rope->m_nodes.at(i);
                        node->m_x = endNode->m_x;
                        // rope->appendAnchor(ropeLinkCounter, btRigidBody::upcast(player->getObject()));
                    }

                    ropeLinkCounter--;
                }
            }

            else
            {
                if(1 / target->getInvMass() < 10)
                {
                    if(ropeLinkCounter < rope->m_nodes.size())
                    {
                        btVector3 playerPosition = player->getPosition();

                        for(int i = 0; i < ropeLinkCounter; ++i)
                        {
                            btSoftBody::Node* node =&rope->m_nodes.at(i);
                            node->m_x = playerPosition;
                            // rope->appendAnchor(ropeLinkCounter, btRigidBody::upcast(player->getObject()));
                        }

                        ropeLinkCounter++;
                    }
                }
            }
        }
    }*/

    /*

    if(hookViewBuffer)
    {
        updateHookViewBufferTransform();
        // btTransform transform = hookViewBuffer->getWorldTransform();
        // transform.setOrigin(player->getPosition() + physics::Vec3(player->getCamera()->getDirection() * 8));
        // hookViewBuffer->setWorldTransform(transform);
    }*/
}

void GrapplingHook::fire()
{
    if(!fired)
    {
        updateCounter = 0;
        ropeLinkCounter = 0;

        /*
        createHook();
        addCollisionResponses();

        btVector3 force = physics::Vec3(
                    CollisionCamera::getSingleton().getCamera()->getDirection().normalisedCopy() * 200
        );

        hook->applyCentralImpulse(force);*/

        CollisionCamera* player = CollisionCamera::getSingletonPtr();
        physics::Vec3 from(player->getPosition() + btVector3(0, player->cameraHeight, 0));
        physics::Vec3 to(from + physics::Vec3(player->getCamera()->getDirection() * 1000));
        // std::cout << "GrapplingHook::fire() -> from(" << from.x() << ", " << from.y() << ", " << from.z() << ")" << std::endl;
        // std::cout << "GrapplingHook::fire() -> to(" << to.x() << ", " << to.y() << ", " << to.z() << ")" << std::endl;
        btCollisionWorld::ClosestRayResultCallback callback = physics::objectPickRay(from, to);

        if(callback.m_collisionObject)
        {
            physics::PhysicsObject* object = (physics::PhysicsObject*) callback.m_collisionObject->getUserPointer();

            if(object)
            {
                if(object->getObject() && object->type() == physics::Rigid)
                {
                    target = btRigidBody::upcast(object->getObject());
                }
            }
        }

        if(target)
        {
            grappleTarget(target, callback.m_hitPointWorld, callback.m_hitNormalWorld);
            // std::cout << "GRAPPLE TARGET!!!!! -> .m_hitPointWorld(" << callback.m_hitPointWorld.x() << ", " << callback.m_hitPointWorld.y() << ", "
            //          << callback.m_hitPointWorld.z() << ")" << std::endl;
            fired = true;
        }

        else
        {
            fired = false;
        }
    }
}

void GrapplingHook::retract()
{
    fired = false;
    destroyBodies();

    if(hookMode == StaticPull)
    {
        btRigidBody* player = CollisionCamera::getSingletonPtr()->upcastObject();
        player->setLinearFactor(btVector3(1.0, 1.0, 1.0));
        player->setLinearVelocity(target->getLinearVelocity() * 0.01);
    }

    else if(target && hookMode == DynamicLift)
    {
        target->setLinearFactor(btVector3(1.0, 1.0, 1.0));
        target->setLinearVelocity(target->getLinearVelocity() * 0.1);
    }

    target = 0;
    hookMode = NoHook;
    // detach();
}

void GrapplingHook::attach()
{
    createHookConstraint();
    attached = true;
}

void GrapplingHook::detach()
{
    destroyHookConstraint();
    attached = false;
}

void GrapplingHook::create()
{
    if(!PlayerInputAction::has(GrapplingHook::name))
        PlayerInputAction::add(new GrapplingHook());
}

void GrapplingHook::updateRopeMesh()
{
    ropeModel->beginUpdate(0);

    for(int i = 0; i < rope->m_nodes.size(); ++i)
    {
        ropeModel->position(physics::Vec3(rope->m_nodes.at(i).m_x).toOgre());
    }

    ropeModel->end();
}

void GrapplingHook::updateHookViewBufferTransform()
{
    CollisionCamera* player = CollisionCamera::getSingletonPtr();
    btTransform liftTransform;
    liftTransform.setIdentity();

    geometry::Euler objectRotation = player->getEuler();
    objectRotation.setRoll(Ogre::Radian(0));
    liftTransform.setRotation(physics::Quaternion(objectRotation.toQuaternion()));
    // physics::Vec3 offsetLift((player->getCamera()->getDirection() * 4) + Ogre::Vector3(0, player->getCamera()->getPosition().y, 0));
    physics::Vec3 offsetLift(physics::Vec3(CollisionCamera::getSingletonPtr()->getCamera()->getPosition()) + physics::Vec3(player->getCamera()->getDirection()) * targetSize);
    liftTransform.setOrigin(player->getWorldTransform().getOrigin() + offsetLift);

    hookViewBuffer->setWorldTransform(liftTransform);
}

void GrapplingHook::addTriangleToMesh(Ogre::ManualObject* object, std::vector<Ogre::Vector3>& points,
                                      int v0, int v1, int v2)
{
    object->index(v0);
    object->index(v1);
    object->index(v2);

    Ogre::Vector3 u = points.at(v1) - points.at(v0);
    Ogre::Vector3 v = points.at(v2) - points.at(v0);

    Ogre::Vector3 normal(
        (u.y * v.z) - (u.z * v.y),
        (u.z * v.x) - (u.x * v.z),
        (u.x * v.y) - (u.y * v.x)
    );

    object->normal(normal);
    object->normal(normal);
    object->normal(normal);
}

void GrapplingHook::createHook()
{
    /*
    CollisionCamera* player = CollisionCamera::getSingletonPtr();
    btVector3 playerPosition = player->getPosition();

    hook = btRigidBody::upcast(
        physics::createCollisionSphere(
            playerPosition,
            1.0,
            0.1,
            physics::Quaternion(0, 1, 0, 0),
            physics::Rigid,
            true,
            0,
            physics::COL_EQUIPMENT,
            physics::equipmentMask
        )
    );

    hook->setAngularFactor(btVector3(0, 0, 0));

    hookNode->setVisible(true);
    hookActor = new physics::PhysicsActor("GrapplingHook", hook, hookNode, hookCollisionGroupID,
                                          physics::NoCursor, true, false);*/
}

void GrapplingHook::grappleTarget(btRigidBody* target, btVector3 hitPosition, btVector3 hitNormal)
{
    this->target = target;
    // btVector3 hookPosition = hook->getWorldTransform().getOrigin();

    if(target->isStaticObject())
    {
        hookMode = StaticPull;
    }

    else
    {
        if(1 / target->getInvMass() < 10)
            hookMode = DynamicLift;
        else
            hookMode = DynamicDrag;
    }

    switch(hookMode)
    {
    case DynamicLift:
        dynamicLift();
        destroyHook();
        break;

    case DynamicDrag:
        dynamicDrag(hitPosition);
        destroyHook();
        break;

    case StaticPull:
        staticPull(hitPosition, hitNormal);
        break;
    }
}

void GrapplingHook::dynamicLift()
{
    CollisionCamera* player = CollisionCamera::getSingletonPtr();
    btRigidBody* playerBody = player->upcastObject();
    btVector3 playerPosition = player->getPosition();

    btVector3 targetMin, targetMax;
    target->getAabb(targetMin, targetMax);
    targetSize = targetMax - targetMin;

    if(targetSize.x() > targetSize.z())
    {
        targetSize.setZ(targetSize.x());
    }

    else
    {
        targetSize.setZ(targetSize.x());
    }

    targetSize.setY(targetSize.y() / 2);

    btVector3 spherePosition = playerPosition + (physics::Vec3(player->getCamera()->getDirection()) * targetSize * 1.1);
    btVector3 playerMin, playerMax;
    playerBody->getAabb(playerMin, playerMax);
    float playerHeight = (playerMax - playerMin).getY();
    internalSphereMinOffsetY = targetSize.y() - (playerHeight / 2) + 3;

    spherePosition.setY(std::max(playerPosition.y() + internalSphereMinOffsetY, spherePosition.y()));

    btRigidBody* rigidSphere = btRigidBody::upcast(
                physics::createCollisionSphere(
                    spherePosition,
                    1,
                    0,
                    physics::Quaternion(0, 1, 0, 0),
                    physics::Rigid,
                    true,
                    0,
                    physics::COL_EQUIPMENT,
                    physics::equipmentMask
                    )
                );

    internalLiftSphere = new physics::RigidObject(rigidSphere);
    internalLiftSphere->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

    // btTransform identity = btTransform::getIdentity();
    /*hookConstraint = physics::createPointConstraint(
                //upcastObject(),
                rigidSphere,
                target,
                //Vec3(p),
                physics::Vec3(0, 0, 0),
                physics::Vec3(0, 0, 0)
                );*/

    // hookConstraint->m_setting.m_damping = 2.0;
    // hookConstraint->m_setting.m_impulseClamp = 30.f;
    // hookConstraint->m_setting.m_tau = 0.001f;


    btTransform localA, localB;
    localA.setIdentity();
    localB.setIdentity();
    localA.getBasis().setEulerZYX(0, M_PI_2, 0);
    localB.getBasis().setEulerZYX(0, M_PI_2, 0);

    hookTargetConstraint = physics::createHingeConstraint(
                rigidSphere,
                target,
                localA,
                localB
                );

    hookTargetConstraint->setLimit(0, 0);

    physics::world.addCollisionFilter(new physics::PlayerLiftCollisionFilterCallback(playerBody, target));
    physics::world.getDynamicsWorld()->getPairCache()->removeOverlappingPair(
                playerBody->getBroadphaseHandle(),
                target->getBroadphaseHandle(),
                physics::world.getDispatcher()
    );

    targetLinearFactor = 0.0001;
    target->setLinearFactor(btVector3(targetLinearFactor, targetLinearFactor, targetLinearFactor));
}

void GrapplingHook::dynamicDrag(const btVector3& hookPosition)
{
    ropeLinkCounter = 0;
    createRope(CollisionCamera::getSingletonPtr()->upcastObject(), hookPosition);
}

void GrapplingHook::staticPull(const btVector3& hookPosition, const btVector3 &hookNormal)
{
    btRigidBody* player = CollisionCamera::getSingletonPtr()->upcastObject();
    hook = btRigidBody::upcast(physics::createCollisionSphere(hookPosition - physics::Vec3(CollisionCamera::getSingletonPtr()->getCamera()->getDirection() * 25), 2.0, 0.0));
    // btTransform newHookTransform = hook->getWorldTransform();
    // newHookTransform.setOrigin(hookPosition - (invDirection * 3));

    btTransform localA, localB;
    localA.setIdentity();
    localB.setIdentity();
    localA.getBasis().setEulerZYX(0, M_PI_2, 0);
    localB.getBasis().setEulerZYX(0, M_PI_2, 0);
    // hook->setMassProps(0.0, btVector3(0, 0, 0));

    hookTargetConstraint = physics::createHingeConstraint(
                player,
                hook,
                localA,
                localB
                );

    hookTargetConstraint->setLimit(0, 0);
    // createRope(player, hookPosition);
    // ropeLinkCounter = rope->m_nodes.size() - 1;
    player->setLinearFactor(btVector3(0.01, 0.01, 0.01));
}

/*
    Config constants for softbodies.

    btSoftBody::Config


    btSoftBody::Config::kVCF; // Velocities correction factor (Baumgarte)
    define the amount of correction per time step for drift solver (sometimes referred as ERP in rigid bodies solvers).

    btSoftBody::Config::kDP; // Damping coefficient [0,1]
    damping, zero = no damping, one= full damping.

    btSoftBody::Config::kDG; // Drag coefficient [0,+inf]
    [aerodynamic] kDG=0 mean no drag.

    btSoftBody::Config::kLF; // Lift coefficient [0,+inf]
    [aerodynamic]=> is a factor of the lift force kLF=0 mean no lift

    btSoftBody::Config::kPR; // Pressure coefficient [-inf,+inf]
    [aerodynamic]=> is a factor of pressure.

    btSoftBody::Config::kVC; // Volume conversation coefficient [0,+inf]
    when 'setPose(true,...)' as been called, define the magnitude of the force used to conserve volume.

    btSoftBody::Config::kDF; // Dynamic friction coefficient [0,1]
    friction, kDF=0 mean sliding, kDF=1 mean sticking.

    btSoftBody::Config::kMT; // Pose matching coefficient [0,1]
    when 'setPose(...,true)' as been called, define the factor used for pose matching.

    btSoftBody::Config::kCHR; // Rigid contacts hardness [0,1]
    define how 'soft' contact with rigid bodies are, kCHR=0 mean no penetration correction, 1 mean full correction.

    btSoftBody::Config::kKHR; // Kinetic contacts hardness [0,1]
    define how 'soft' contact with kinetic/static bodies are, kKHR=0 mean no penetration correction, 1 mean full correction.

    btSoftBody::Config::kSHR; // Soft contacts hardness [0,1]
    define how 'soft' contact with other soft bodies are, kSHR=0 mean no penetration correction, 1 mean full correction.

    btSoftBody::Config::kAHR; // Anchors hardness [0,1]
    define how 'soft' anchor constraint (joint) are, kAHR=0 mean no drift correction, 1 mean full correction.

    btSoftBody::Config::maxvolume; // Maximum volume ratio for pose

    --unused--
    btSoftBody::Config::timescale; // Time scale
    factor of time step, can be used to speed up, or slow down simulation, default=1.
*/

void GrapplingHook::createRope(btRigidBody* origin, const btVector3& hookPosition)
{
    int numLinks = 20;
    rope = btSoftBody::upcast(
        physics::createCollisionRope(
            origin->getWorldTransform().getOrigin(),
            hookPosition,
            numLinks,
            0,
            20,
            0,
            physics::COL_EQUIPMENT,
            physics::equipmentMask
        )
    );

    // rope->m_cfg.kDP = 0.001; // Damping
    rope->m_cfg.kDP = 0.0; // Damping
    rope->m_cfg.kAHR = 0.5; // Anchor hardness
    rope->m_cfg.kDF = 0.0; // Dynamic friction coefficient
    rope->m_cfg.kCHR = 1.0; // Rigid contacts hardness
    rope->appendAnchor(0, origin);
    rope->appendAnchor(rope->m_nodes.size() - 1, target);
    ropeModel->setVisible(true);
    ropeActor->setObject(rope);
    ropeActor->setVisible(true);

    /*
    if(hookMode == StaticPull)
    {
        btVector3 targetPosition = rope->m_nodes.at(rope->m_nodes.size() - 1).m_x;
        for(int i = 0; i < rope->m_nodes.size(); ++i)
        {
            rope->m_nodes.at(i).m_x = targetPosition;
        }
    }*/

    /*
    btTransform identity = btTransform::getIdentity();

    btVector3 targetMin, targetMax;
    target->getAabb(targetMin, targetMax);
    targetSize = targetMax - targetMin;

    if(targetSize.x() > targetSize.z())
    {
        targetSize.setZ(targetSize.x());
    }

    else
    {
        targetSize.setZ(targetSize.x());
    }

    targetSize.setY(targetSize.y() / 2);

    if(hook)
    {
        if(target->isStaticObject())
        {
            hookViewBuffer = btRigidBody::upcast(
                        physics::createCollisionSphere(
                            physics::Vec3(CollisionCamera::getSingletonPtr()->getCamera()->getPosition()) + physics::Vec3(player->getCamera()->getDirection()) * targetSize,
                            1.0
                        )
            );

            hookTargetConstraint = physics::createGenericConstraint(hook, target, identity, identity);
            // hook->setMassProps(0, btVector3(0, 0, 0));
            hookViewBuffer->setAngularFactor(btVector3(0, 0, 0));
            hookBufferConstraint = physics::createGenericConstraint(hookViewBuffer, hook, identity, identity);
            hookConstraint = physics::createGenericConstraint(player->upcastObject(), hookViewBuffer, identity, identity);
        }

        else
        {
            hookViewBuffer = btRigidBody::upcast(
                        physics::createCollisionSphere(
                            physics::Vec3(CollisionCamera::getSingletonPtr()->getCamera()->getPosition()) + physics::Vec3(player->getCamera()->getDirection()) * targetSize,
                            1.0
                        )
            );

            hookTargetConstraint = physics::createGenericConstraint(hook, target, identity, identity);
            hook->setLinearVelocity(btVector3(0, 0, 0));
            hookBufferConstraint = physics::createGenericConstraint(hookViewBuffer, hook, identity, identity);
        }


        // hookViewBuffer = btRigidBody::upcast(physics::createCollisionSphere(physics::Vec3(CollisionCamera::getSingletonPtr()->getCamera()->getPosition() + (player->getCamera()->getDirection() * 4)), 1.0, 0.0));
        // hookTargetConstraint = physics::createGenericConstraint(hook, target, identity, identity);
        // // hook->setMassProps(0, btVector3(0, 0, 0));
        // hookViewBuffer->setAngularFactor(btVector3(0, 0, 0));
        // hookBufferConstraint = physics::createGenericConstraint(hookViewBuffer, hook, identity, identity);
        // hookConstraint = physics::createGenericConstraint(player->upcastObject(), hookViewBuffer, identity, identity);
        // updateHookViewBufferTransform();
    }

    else
    {
        // hookConstraint = physics::createGenericConstraint(player->upcastObject(), target, identity, identity);
    }*/
}

void GrapplingHook::addCollisionResponses()
{
    addCollisionResponse(hookCollisionGroupID, physics::VOXEL_GROUP_ID);
    addCollisionResponse(hookCollisionGroupID, physics::MAIN_GROUP_ID);
}

void GrapplingHook::addCollisionResponse(physics::collision_group_id_t group0,
                                         physics::collision_group_id_t group1)
{
    GrapplingHookCollision* collisionAction = new GrapplingHookCollision(
                this,
                group0,
                group1,
                group0 | group1
    );

    physics::world.addCollisionResponse(
        group0,
        group1,
        collisionAction
    );

    collisionActions.push_back(collisionAction);
}

void GrapplingHook::removeCollisionResponses()
{
    for(unsigned int i = 0; i < collisionActions.size(); ++i)
    {
        GrapplingHookCollision* collisionAction = collisionActions.at(i);
        physics::world.removeCollisionResponse(collisionAction->getCollisionCombinationInt());
        collisionAction = 0;
    }

    collisionActions.clear();
}

void GrapplingHook::destroyBodies()
{
    destroyHook();
    destroyRope();
    destroyHookConstraint();
}

void GrapplingHook::destroyHook()
{
    if(hook)
    {
        hookNode->setVisible(false);
        delete hookActor;
        hook = 0;
        hookActor = 0;
    }

    if(hookViewBuffer)
    {
        physics::world.removeCollisionObject(hookViewBuffer);
        hookViewBuffer = 0;
    }
}

void GrapplingHook::destroyRope()
{
    if(rope)
    {
        physics::world.removeCollisionObject(rope);
        rope = 0;
        ropeModel->setVisible(false);
        ropeActor->setVisible(false);
        ropeActor->setObject(0);
    }

    if(internalLiftSphere)
    {
        physics::world.removeCollisionFilter();
        delete internalLiftSphere;
        internalLiftSphere = 0;
    }
}

void GrapplingHook::createHookConstraint()
{
    // hookConstraint = physics::createGenericConstraint(CollisionCamera::getSingletonPtr()->upcastObject(), )
}

void GrapplingHook::destroyHookConstraint()
{
    if(hookConstraint)
    {
        physics::world.removeConstraint(hookConstraint);
        hookConstraint = 0;
    }

    if(hookTargetConstraint)
    {
        physics::world.removeConstraint(hookTargetConstraint);
        hookTargetConstraint = 0;
    }

    if(hookBufferConstraint)
    {
        physics::world.removeConstraint(hookBufferConstraint);
        hookBufferConstraint = 0;
    }
}

} // gameplay namespace
