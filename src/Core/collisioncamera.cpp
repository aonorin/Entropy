// Ogre includes
#include <OGRE/OgreCamera.h>
//#include <OgreEntity.h>
//#include <OgreLogManager.h>
//#include <OgreRoot.h>
//#include <OgreViewport.h>
//#include <OgreSceneManager.h>
//#include <OgreRenderWindow.h>
//#include <OgreConfigFile.h>

// Bullet includes
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"

// Boost includes
#include "boost/thread/locks.hpp"

// Entropy includes
#include "Entropy.h"
#include "VoxelTerrain/terraingenerator.h"
#include "VoxelTerrain/fresnelwater.h"
#include "collisioncamera.h"
#include "src/Physics/physics.h"
#include "src/Utility/Math.h"
#include "src/Menu/entropycursor.h"
#include "src/Physics/collisiongroupmanager.h"
#include "gameplay/actorupdateactions.h"
#include "Utility/Math.h"
#include "OGRE/OgreBitwise.h"
#include "OGRE/OgreSceneManager.h"
#include "src/gameplay/playerinputaction.h"
#include "src/Physics/collisiongroupmanager.h"

using namespace Ogre;
//using namespace OgreBites;
using namespace physics;
using namespace utility;
using namespace math;

// Static declaration
float CollisionCamera::scale = 0.25;
float CollisionCamera::pickingEyeDistance = 1000 * 0.2 * CollisionCamera::scale;
float CollisionCamera::pickingHandDistance = 25 * 0.2 * CollisionCamera::scale;
const float CollisionCamera::maxLiftingWeight = 20.0;
float CollisionCamera::FLASHLIGHT_SPACING_SCALE = 8;
float CollisionCamera::FLASHLIGHT_FOCUS_DISTANCE = 20;
float CollisionCamera::crouchingSpeed = 150;
float CollisionCamera::walkingSpeed = 300;
float CollisionCamera::runningSpeed = 1200;
float CollisionCamera::maxCrouchingSpeed = 1.25;
float CollisionCamera::maxWalkingSpeed = 2.5;
float CollisionCamera::maxRunningSpeed = 5;
float CollisionCamera::groundDamping = 0.9999;
float CollisionCamera::airDamping = 0.00;
float CollisionCamera::groundFriction = 0.1;
float CollisionCamera::airFriction = 0.0;

// interval time (in milliseconds) between application of force
boost::chrono::milliseconds CollisionCamera::crouchingInterval = boost::chrono::milliseconds(800);
boost::chrono::milliseconds CollisionCamera::walkingInterval = boost::chrono::milliseconds(400);
boost::chrono::milliseconds CollisionCamera::runningInterval = boost::chrono::milliseconds(200);

#ifdef WIN32
static const double M_PI_2 = 6.28318530718;
#endif

CollisionCamera::CollisionCamera(SceneManager *manager) :
    RigidObject(0, physics::PLAYER_GROUP_ID),
    flyMode(false),
    wDown(false),
    aDown(false),
    sDown(false),
    dDown(false),
    spaceDown(false),
    shiftDown(false),
    keyCombo(0),
    pickedObject(0),
    eulerRotation(Ogre::Radian(math::PI)),
    linearVelocity(0,0,0),
    raySource(0, 0 ,0),
    rayTarget(0, 0, 0),
    halfHeight(0),
    rayLambda(0),
    down(0, 0, 0),
    liftingObject(false),
    liftConstraint(0),
    liftHinge(0),
    internalLiftSphere(0),
    crouchingShape(0),
    standingShape(0),
    crouching(false),
    cameraHeight(7*CollisionCamera::scale),
    crouchHeight(3*CollisionCamera::scale),
    standingHeight(7*CollisionCamera::scale),
    mSceneMgr(manager),
    flashlightOn(false),
    pickedObjectHeight(0),
    pickedObjectWidth(0),
    gravityModifier(1.0)
{
    pickedTransform.setIdentity();

    for(int i = 0; i < numDirectionKeys; ++i)
    {
        wasdBitArray.push_back(0); // initialize bit array to 0
    }

    mCamera = manager->createCamera("Player Camera");
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(10000);
    actorNode = manager->createSceneNode();
    actorNode->setVisible(true);
    createFlashlightNodes();

    //= { 5000, 1.0, 0.0009, 0.00032 };

    // createFlashlights();
    flashlightAttenuation[0] = 5000;
    flashlightAttenuation[1] = 1.0;
    flashlightAttenuation[2] = 0.000;
    flashlightAttenuation[3] = 0.00000003;
    flashlightPowerScale = 1.0;

    lastMovementTime = boost::chrono::steady_clock::time_point(boost::chrono::milliseconds(0));
    movementInterval = CollisionCamera::walkingInterval;
}

CollisionCamera::~CollisionCamera()
{
    //destroyFlashlights();

    if(internalLiftSphere)
        delete internalLiftSphere;

    if(crouching)
        delete standingShape;
    else
        delete crouchingShape;

    //physics::removeCollisionObject(object);
}

PhysicsObject *CollisionCamera::getPickedObject()
{
    boost::shared_lock<boost::shared_mutex> readLock(mutex);
    return pickedObject;
}

void CollisionCamera::update()
{
    //boost::upgrade_lock<boost::shared_mutex> lock(mutex);
    //boost::upgrade_to_unique_lock<boost::shared_mutex> writeLock(lock);
    updateObject();
}

void CollisionCamera::initCollisionObject()
{
    //boost::upgrade_lock<boost::shared_mutex> lock(mutex);
    //boost::upgrade_to_unique_lock<boost::shared_mutex> writeLock(lock);

    collisionGroup = physics::PLAYER_GROUP_ID;

    //object = physics::createCollisionBox(Vec3(0, 0, 0), Vec3(4, 8, 4), 3.0); // Create the actual physics object
//    object = createCollisionCapsule(Vec3(0, 0, 0), 6.0*CollisionCamera::scale, 8.0*CollisionCamera::scale);
    object = createCollisionCapsule(Vec3(0, 0, 0), 2.0 * CollisionCamera::scale, 8.0 * CollisionCamera::scale, 91.0,
                                    physics::Quaternion(0, 1, 0, 0), Rigid, true, (PhysicsObject*) this, COL_PLAYER,
                                    playerMask);
    // object = createCollisionCone(Vec3(0, 0, 0), 3 * CollisionCamera::scale, 8.0 * CollisionCamera::scale, 1.0);
    // object = physics::createCollisionCylinder(Vec3(0, 0, 0), Vec3(4, 8, 4) * CollisionCamera::scale, CollisionCamera::scale);
    // object = physics::createCollisionSphere(Vec3(0, 0, 0), CollisionCamera::scale * 8, CollisionCamera::scale);
    // object = physics::createCollisionSphereStack(Vec3(0, 0, 0), CollisionCamera::scale * 8, CollisionCamera::scale * 3, 2, CollisionCamera::scale);

    setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT); // Set the flag

    // btTransform transform = object->getWorldTransform();
    // btQuaternion upsideDown = transform.getRotation().inverse();
    // transform.setRotation(upsideDown);
    // object->setWorldTransform(transform);

    standingShape = object->getCollisionShape();
    // crouchingShape = new btConeShape(3.0 * CollisionCamera::scale, 4.0 * CollisionCamera::scale);
    crouchingShape = new btCapsuleShape(2.0 * CollisionCamera::scale, 4.0 * CollisionCamera::scale);
    // crouchingShape = new btCylinderShape(Vec3(4, 4, 4) * CollisionCamera::scale);
    // crouchingShape = new btSphereShape(CollisionCamera::scale * 4);

    //World::world_upgrade_write_t physicsLock(world.mutex);
    //World::world_write_t physicsWriteLock(physicsLock); // Lockdown physics::world to set a few things
    halfHeight = btScalar(8.0*CollisionCamera::scale);
    object->setFriction(CollisionCamera::airFriction);
    btRigidBody* rigidObject = upcastObject();
    rigidObject->setAngularFactor(0.0);
    rigidObject->setSleepingThresholds(0.0, 0.0);
    rigidObject->setDamping(CollisionCamera::airDamping, 1.0);

    // defaultGravity = rigidObject->getGravity();
    defaultGravity = btVector3(0, -180, 0) * gravityModifier;

    object->setUserPointer((PhysicsObject*) this);
    setCameraPosition(Ogre::Vector3(0,0,160));
    updateObject();
    pathID = gameplay::astar.registerMover(mCamera->getPosition(),gameplay::astar.getWalkMap(entropy::Free));
}

void CollisionCamera::setCameraPosition(Vector3 position)
{
    //boost::upgrade_lock<boost::shared_mutex> lock(mutex);
    //boost::upgrade_to_unique_lock<boost::shared_mutex> writeLock(lock);
    //    setPosition(btVector3(position.x

    mCamera->setPosition(position);

    if(object)
        setPosition(btVector3(position.x, position.y, position.z));
//    gameplay::astar.setPos(pathID,position);
    //    updateObject();
    //    actorNode->setPosition(position);
    //    actorNode->_update(true, false);
    //    gameplay::astar.setPos(pathID,position);
}

void CollisionCamera::setCameraPosition(int x, int y, int z)
{
    //boost::upgrade_lock<boost::shared_mutex> lock(mutex);
    //boost::upgrade_to_unique_lock<boost::shared_mutex> writeLock(lock);
    setCameraPosition(Vector3(x,y,z));
    // mCamera->setPosition(x,y,z);
    std::cout << "setCameraPosition: "
              << mCamera->getPosition().x<<","
              << mCamera->getPosition().y<<","
              << mCamera->getPosition().z
              << std::endl;
    //    actorNode->setPosition(x,y,z);
    //    actorNode->_update(true, false);
    //    updateObject();
    //    gameplay::astar.setPos(pathID,mCamera->getPosition());
}

geometry::Euler CollisionCamera::getEuler()
{
    return eulerRotation;
}

btRigidBody* CollisionCamera::getInternalLiftSphere()
{
    return internalLiftSphere->upcastObject();
}

Ogre::Camera* CollisionCamera::getCamera()
{
    //boost::shared_lock<boost::shared_mutex> readLock(mutex);
    return mCamera;
}

std::vector<Ogre::Light*>& CollisionCamera::getFlashlights()
{
    return flashlights;
}

void CollisionCamera::injectKeyDown(EntKeyCode key)
{
    switch(key)
    {
    case utility::KC_W:
        wDown = true;
        wasdBitArray[3] = 1;
        break;

    case utility::KC_A:
        aDown = true;
        wasdBitArray[2] = 1;
        break;

    case utility::KC_S:
        sDown = true;
        wasdBitArray[1] = 1;
        break;

    case utility::KC_D:
        dDown = true;
        wasdBitArray[0] = 1;
        break;

    case utility::KC_F:
        toggleFlashLights();
        break;

    case utility::KC_SPACE:
        spaceDown = true;
        jump();
        break;

    case utility::KC_LSHIFT:
        shiftDown = true;
        break;

    case utility::KC_UP:
        flyMode = true;
        object->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
        upcastObject()->setLinearVelocity(btVector3(0, 0, 0));

        for(int i = 0; i < wasdBitArray.size(); ++i)
        {
            wasdBitArray[i] = 0;
        }

        break;

    case utility::KC_DOWN:
        flyMode = false;
        object->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

        for(int i = 0; i < wasdBitArray.size(); ++i)
        {
            wasdBitArray[i] = 0;
        }

        break;

    case utility::KC_LCONTROL:
        setCrouching(true);
        break;
    }

    // We want to move immediately so we offset the time to our longest possible interval
    lastMovementTime = entropy::clock.now() - CollisionCamera::crouchingInterval;
    bitShiftWASD(); // Updates the keyCombo variable
}

void CollisionCamera::injectKeyUp(EntKeyCode key)
{
    switch(key)
    {
    case utility::KC_W:
        wDown = false;
        wasdBitArray[3] = 0;
        break;

    case utility::KC_A:
        aDown = false;
        wasdBitArray[2] = 0;
        break;

    case utility::KC_S:
        sDown = false;
        wasdBitArray[1] = 0;
        break;

    case utility::KC_D:
        dDown = false;
        wasdBitArray[0] = 0;
        break;

    case utility::KC_SPACE:
        spaceDown = false;
        break;

    case utility::KC_LSHIFT:
        shiftDown = false;
        break;

    case utility::KC_LCONTROL:
        setCrouching(false);
        break;
    }

    bitShiftWASD(); // Updates the keyCombo variable
}

void CollisionCamera::mouseMove(double x, double y)
{
    //boost::shared_lock<boost::shared_mutex> lock(mutex); // Lock down for synchronization
    //boost::upgrade_lock<boost::shared_mutex> lock(mutex);
    //boost::upgrade_to_unique_lock<boost::shared_mutex> writeLock(lock);

    try
    {
        eulerRotation.yaw(Ogre::Degree(x * 0.15));
        eulerRotation.pitch(Ogre::Degree(y * 0.15));

        if(eulerRotation.getPitch().valueRadians() > math::HALF_PI)
        {
            eulerRotation.setPitch(Ogre::Radian(math::HALF_PI));
        }

        else if(eulerRotation.getPitch().valueRadians() < -math::HALF_PI)
        {
            eulerRotation.setPitch(Ogre::Radian(-math::HALF_PI));
        }

        mCamera->setOrientation(eulerRotation.toQuaternion());
        viewDirection = mCamera->getDirection();
        actorNode->setOrientation(mCamera->getOrientation());
        actorNode->_update(true, false);
        // updateFlashlights();
    }

    catch(...)
    {
        std::cout << "ERROR in CollisionCamera::mouseMove()" << std::endl;
    }
}

void CollisionCamera::injectMouseDown(EntKeyCode button)
{
    //boost::upgrade_lock<boost::shared_mutex> lock(mutex);
    //boost::upgrade_to_unique_lock<boost::shared_mutex> writeLock(lock);

    if(button == utility::MB_Left)
    {
        //throwObject();
        if(!liftingObject)
        {
            if(pickedObject)
                liftObject();

            if(!liftingObject)
                gameplay::PlayerInputAction::keyDown(button);
        }

        else
        {
            dropObject();
        }
    }

    else if(button == utility::MB_Right)
    {
        if(pickedObject)
            liftObject();

        if(!liftingObject)
            gameplay::PlayerInputAction::keyDown(button);
    }
}

void CollisionCamera::injectMouseUp(EntKeyCode button)
{
    //boost::upgrade_lock<boost::shared_mutex> lock(mutex);
    //boost::upgrade_to_unique_lock<boost::shared_mutex> writeLock(lock);

    if(button == utility::MB_Left)
    {
        // std::cout << "Left Mouse Button Up." << std::endl;
        //throwObject();
        if(liftingObject)
            dropObject();
        else
            gameplay::PlayerInputAction::keyUp(button);
    }

    if(button == utility::MB_Right)
    {
        if(liftingObject)
            dropObject();
        else
            gameplay::PlayerInputAction::keyUp(button);
    }

    if(liftingObject)
        dropObject();
    else
        gameplay::PlayerInputAction::keyUp(button);
}

bool CollisionCamera::onGround()
{
    //boost::shared_lock<boost::shared_mutex> readLock(mutex);
    return rayLambda < btScalar(1.0);
}

void CollisionCamera::liftObject()
{
    if(pickedObject)
    {
        float mass = pickedObject->getMass();
        if(mass > 0 && mass < maxLiftingWeight && pickedObject->type() == Rigid)
        {
            /*
            if(object->checkCollideWith(pickedObject->getObject())) // If we're touching the object move it back slightly
            {
                btVector3 pickedObjectPosition = pickedObject->getPosition();
                pickedObject->setPosition(pickedObjectPosition + btVector3(viewDirection.x * 2, 0, viewDirection.z * 2));
                pickedObject->activate();
            }*/

            physics::world.addCollisionFilter(new PlayerLiftCollisionFilterCallback(object, pickedObject->getObject()));
            physics::world.getDynamicsWorld()->getPairCache()->removeOverlappingPair(
                        object->getBroadphaseHandle(),
                        pickedObject->getObject()->getBroadphaseHandle(),
                        physics::world.getDispatcher()
                        );

            btRigidBody* rigidSphere = btRigidBody::upcast(
                        physics::createCollisionSphere(
                            pickedObject->getPosition(),
                            1,
                            0,
                            physics::Quaternion(0, 1, 0, 0),
                            Rigid
                            )
                        );

            internalLiftSphere = new physics::RigidObject(rigidSphere);
            internalLiftSphere->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

            btTransform pickedObjectTransform = pickedObject->getWorldTransform();
            btVector3 min, max;
            pickedObject->getObject()->getCollisionShape()->getAabb(pickedObjectTransform, min, max);
            pickedObjectWidth = max.x() - min.x();
            pickedObjectHeight = max.y() - min.y();

            //pickedObject->setStatic();
            pickedObject->activate(true);
            btRigidBody::upcast(pickedObject->getObject())->setActivationState(DISABLE_DEACTIVATION);
            const btQuaternion& btRotation = pickedObject->getRotation();
            Ogre::Quaternion ogRotation(btRotation.w(), btRotation.x(), btRotation.y(), btRotation.z());
            //btVector3 btP(getPosition() - pickedObject->getPosition());

            btVector3 btP(
                        getPosition()
                        + Vec3((viewDirection * ((pickedObjectWidth) + halfHeight)) + Ogre::Vector3(0, cameraHeight, 0))
                        - pickedObject->getPosition()
                        );


            //offsetLift.setY(std::max(pickedObjectHalfHeight * 1.1, offsetLift.y()));
            //liftTransform.setOrigin(getWorldTransform().getOrigin() + offsetLift);

            Ogre::Vector3 p(btP.x(), btP.y(), btP.z());
            p = ogRotation.Inverse() * p;


            liftConstraint = physics::createPointConstraint(
                        //upcastObject(),
                        btRigidBody::upcast(internalLiftSphere->getObject()),
                        btRigidBody::upcast(pickedObject->getObject()),
                        //Vec3(p),
                        Vec3(0, 0, 0),
                        Vec3(0, 0, 0)
                        );

            liftConstraint->m_setting.m_damping = 0.99;
            liftConstraint->m_setting.m_impulseClamp = 30.f;
            liftConstraint->m_setting.m_tau = 0.001f;

            //pickedObjectAngularFactor = btRigidBody::upcast(pickedObject->getObject())->getAngularFactor();
            //btRigidBody::upcast(pickedObject->getObject())->setAngularFactor(btVector3(0, 0, 0));

            /*
            btTransform identity = btTransform::getIdentity();

            liftConstraint = physics::createGenericConstraint(
                        internalLiftSphere,
                        btRigidBody::upcast(pickedObject->getObject()),
                        identity,
                        identity
            );


            liftConstraint->setLinearLowerLimit(btVector3(-10, -10, -10));
            liftConstraint->setLinearUpperLimit(btVector3(10, 10, 10));
            liftConstraint->setAngularLowerLimit(btVector3(0, 0, 0));
            liftConstraint->setAngularUpperLimit(btVector3(0, 0, 0));
            */


            btTransform localA, localB;
            localA.setIdentity();
            localB.setIdentity();
            localA.getBasis().setEulerZYX(0, M_PI_2, 0);
            //localA.setOrigin(btVector3(3, 1, 3));
            //localA.setOrigin(btVector3(0.0, 1.0, 3.05));
            localB.getBasis().setEulerZYX(0, M_PI_2, 0);
            //localB.setOrigin(btVector3(3, 1, 3));
            //localB.setOrigin(btVector3(0.0, -1.5, -0.05));

            liftHinge = physics::createHingeConstraint(
                        btRigidBody::upcast(internalLiftSphere->getObject()),
                        btRigidBody::upcast(pickedObject->getObject()),
                        localA,
                        localB
                        );

            liftHinge->setLimit(0, 0);
            //liftConstraint->setBreakingImpulseThreshold(6000);
            //liftHinge->setBreakingImpulseThreshold(6000);


            /*
            btTransform localA2, localB2;
            localA2.setIdentity();
            localB2.setIdentity();
            localA2.getBasis().setEulerZYX(M_PI_2, 0, 0);
            //localA2.setOrigin(btVector3(3, 1, 3));
            //localA.setOrigin(btVector3(0.0, 1.0, 3.05));
            localB2.getBasis().setEulerZYX(M_PI_2, 0, 0);
            //localB2.setOrigin(btVector3(3, 1, 3));

            liftConstraint1 = physics::createHingeConstraint(
                        internalLiftSphere,
                        btRigidBody::upcast(pickedObject->getObject()),
                        localA2,
                        localB2
            );*/

            liftDistance = getPosition().distance(pickedObject->getPosition());
            liftingObject = true;
        }
    }
}

void CollisionCamera::dropObject()
{
    if(liftConstraint)
        //if(liftConstraint0 && liftConstraint1)
    {
        physics::world.removeCollisionFilter();
        physics::removeConstraint(liftConstraint);
        liftConstraint = 0;
        physics::removeConstraint(liftHinge);
        liftHinge = 0;
        if(pickedObject)
            btRigidBody::upcast(pickedObject->getObject())->forceActivationState(ACTIVE_TAG);
        //pickedObject->setDynamic();
        //btRigidBody::upcast(pickedObject->getObject())->setAngularFactor(pickedObjectAngularFactor);
        //physics::removeConstraint(liftConstraint0);
        //physics::removeConstraint(liftConstraint1);
        //liftConstraint0 = liftConstraint1 = 0;

        delete internalLiftSphere;
        internalLiftSphere = 0;
    }

    liftingObject = false;
}

void CollisionCamera::throwObject()
{
    if(liftingObject && pickedObject)
    {
        pickedObject->applyCentralImpulse(physics::Vec3(mCamera->getDirection()) * 10);
        dropObject();
        liftingObject = false;
    }
}

void CollisionCamera::setCrouching(bool crouching)
{
    this->crouching = crouching;

    if(crouching)
    {
        if(liftingObject)
            dropObject(); // Can't crouch and lift at the same time

        object->setCollisionShape(crouchingShape);
        //cameraHeight = 3;
    }

    else
    {
        if(upcastObject()->getLinearVelocity().length() > 1)
        {
            btVector3 pos = getPosition();
            pos.setY(pos.y() + 2);
            setPosition(pos);
            //object->setCollisionShape(standingShape);
            //cameraHeight = 2;
        }

        object->setCollisionShape(standingShape);
    }
}

void CollisionCamera::toggleFlashLights()
{
    flashlightOn = !flashlightOn;

    if(flashlightOn)
    {
        createFlashlights();
        resetFlashlightNodes();
        //updateFlashlights();
    }

    else
    {
        destroyFlashlights();
    }
}

void CollisionCamera::updateFlashlights()
{
    if(flashlightOn)
    {
        for(int i = 0; i < flashlights.size(); ++i)
        {
            flashlights.at(i)->setDirection(Ogre::Vector3(0, 0, -1));
            flashlightNodes.at(i)->setPosition(Ogre::Vector3(0, 0, 0));
        }
    }
}

void CollisionCamera::createFlashlights()
{
    std::cout << "void CollisionCamera::createFlashlights()" << std::endl;
    if(flashlights.size() > 0)
        destroyFlashlights();

    for(int i = 0; i < NUM_FLASHLIGHTS; ++i)
    {
        flashlights.push_back(createFlashlight(i));
    }
}

Ogre::Light *CollisionCamera::createFlashlight(int lightNumber)
{
    std::stringstream ss;
    ss << "PlayerFlashlight" << lightNumber;
    Ogre::Light* flashlight = mSceneMgr->createLight(ss.str().c_str());
    flashlight->setCastShadows(true);
    flashlight->setType(Ogre::Light::LT_SPOTLIGHT);
    // flashlight->setType(Ogre::Light::LT_DIRECTIONAL);
    flashlight->setDiffuseColour(0.85, 0.85, 0.85);
    flashlight->setSpecularColour(0.85, 0.85, 0.85);
    flashlight->setAttenuation(
                CollisionCamera::flashlightAttenuation[0],
            CollisionCamera::flashlightAttenuation[1],
            CollisionCamera::flashlightAttenuation[2],
            CollisionCamera::flashlightAttenuation[3]
            );
    flashlight->setSpotlightRange(Ogre::Degree(0), Ogre::Degree(80));
    flashlight->setShadowNearClipDistance(mCamera->getNearClipDistance());
    flashlight->setShadowFarClipDistance(mCamera->getFarClipDistance());
    flashlight->setVisible(true);
    flashlight->setPowerScale(flashlightPowerScale);
    flashlightNodes.at(lightNumber)->attachObject(flashlight);
    flashlight->setPosition(Ogre::Vector3(0, 0, 0));
    flashlight->setDirection(Ogre::Vector3(0, 0, -1));
    return flashlight;
}

void CollisionCamera::createFlashlightNodes()
{
    for(int i = 0; i < NUM_FLASHLIGHTS; ++i)
    {
        std::stringstream ss;
        ss << "PlayerFlashlightNode" << i;
        Ogre::SceneNode* flashlightNode = actorNode->createChildSceneNode(ss.str().c_str());
        flashlightNode->setVisible(true);
        flashlightNodes.push_back(flashlightNode);
    }

    resetFlashlightNodes();
}

void CollisionCamera::resetFlashlightNodes()
{
    for(int i = 0; i < flashlightNodes.size(); ++i)
    {
        Ogre::Vector3 relativePosition(0, 0, 0);

        switch(i)
        {
        case 0:
            relativePosition = Ogre::Vector3(0.25, utility::math::SQUARE_ROOT_ZERO_POINT_SEVEN_FIVE - 0.6, 0)
                    * FLASHLIGHT_SPACING_SCALE;

            break;

        case 1:
            relativePosition = Ogre::Vector3(1, -0.75, 0) * FLASHLIGHT_SPACING_SCALE;
            break;

        case 2:
            relativePosition = Ogre::Vector3(-1,-0.75, 0) * FLASHLIGHT_SPACING_SCALE;
            break;
            /*
        case 0:
            relativePosition = Ogre::Vector3(-1, 1, 0) * FLASHLIGHT_SPACING_SCALE;
            break;

        case 1:
            relativePosition = Ogre::Vector3(1, 1, 0) * FLASHLIGHT_SPACING_SCALE;
            break;

        case 2:
            relativePosition = Ogre::Vector3(1, -1, 0) * FLASHLIGHT_SPACING_SCALE;
            break;

        case 3:
            relativePosition = Ogre::Vector3(-1, -1, 0) * FLASHLIGHT_SPACING_SCALE;
            break;
        */
        }

        Ogre::SceneNode* flashlightNode = flashlightNodes.at(i);
        flashlightNode->setPosition(relativePosition);
        flashlightNode->lookAt(Ogre::Vector3(0, 0.5, -FLASHLIGHT_FOCUS_DISTANCE), Ogre::Node::TS_PARENT);

        flashlightNode->_update(true, false);
    }
}

void CollisionCamera::destroyFlashlights()
{
    std::cout << "void CollisionCamera::destroyFlashlights()" << std::endl;
    for(int i = 0; i < flashlights.size(); ++i)
    {
        if(flashlights.at(i))
        {
            mSceneMgr->destroyLight(flashlights.at(i)->getName());
            flashlights[i] = 0;
        }
    }

    flashlights.clear();
}

void CollisionCamera::setFlashlightSpacingScale(int flashlightSpacingScale)
{
    CollisionCamera::FLASHLIGHT_SPACING_SCALE = flashlightSpacingScale;
    CollisionCamera::getSingletonPtr()->resetFlashlightNodes();
}

void CollisionCamera::setFlashlightFocusDistance(int flashlightFocusDistance)
{
    CollisionCamera::FLASHLIGHT_FOCUS_DISTANCE = flashlightFocusDistance;
    CollisionCamera::getSingletonPtr()->resetFlashlightNodes();
}

void CollisionCamera::jump()
{
    if(onGround())
    {
        // btVector3 jumpVector = upcastObject()->getLinearVelocity() * 0.1;
        btVector3 jumpVector(0, 0, 0);

        if(shiftDown)
            jumpVector.setY(24000*CollisionCamera::scale);
        else
            jumpVector.setY(12000*CollisionCamera::scale);

        applyCentralImpulse(jumpVector);
        // applyForce(jumpVector, object->getWorldTransform().getOrigin());
    }
}

void CollisionCamera::fpsMove()
{
    try
    {
        btRigidBody* rigidBody = upcastObject();
        if(onGround())
        {
            rigidBody->setDamping(CollisionCamera::groundDamping, 0.0);
            rigidBody->setFriction(CollisionCamera::groundFriction);
            rigidBody->setGravity(btVector3(0, 0, 0));

            Vec3 directionVector = mCamera->getDirection();
            directionVector.setY(0);

            activate(true);

            if(crouching)
            {
                maxSpeed = CollisionCamera::maxCrouchingSpeed;
                movementInterval = CollisionCamera::crouchingInterval;
                directionVector *= crouchingSpeed;
            }

            else
            {
                if(shiftDown)
                {
                    maxSpeed = CollisionCamera::maxRunningSpeed;
                    movementInterval = CollisionCamera::runningInterval;
                    directionVector *= runningSpeed;
                }

                else
                {
                    maxSpeed = CollisionCamera::maxWalkingSpeed;
                    movementInterval = CollisionCamera::walkingInterval;
                    directionVector *= walkingSpeed;
                }
            }


            btScalar rotation = 0.0;
            bool pushDown = true;

            switch(keyCombo)
            {
            case D: // Right
                rotation = -HALF_PI;
                break;

            case SD: // Diagonal, Right/Back
                rotation = -THREE_FOURTHS_PI;
                break;

            case S: // Back
                rotation = PI;
                break;

            case AS: // Diagonal, Left/Back
                rotation = THREE_FOURTHS_PI;
                break;

            case ASD: // Same as S, just move backwards
                rotation = PI;
                break;

            case A: // Left
                rotation = HALF_PI;
                break;

            case WA:  // Diagonal, Left/Forward
                rotation = ONE_FOURTH_PI;
                break;

            case W: // Forward
                break;

            case WD: // Diagonal, Right/Forward
                rotation = -ONE_FOURTH_PI;
                break;

            case WAD: // Same as W, just move forward
                break;

            default:
                directionVector.setX(0);
                directionVector.setZ(0);
                pushDown = false;
                break;
            }

            directionVector = directionVector.rotate(btVector3(0, 1, 0), rotation); // rotate the camera direction based on key combo

            if(pushDown)
                directionVector += (defaultGravity * 0.05);

            // linearVelocity = getLinearVelocity();
            // directionVector.setY(linearVelocity.y()); // However, we want to retain our Y velocity

            //upcastObject()->setLinearVelocity(directionVector); // Update our velocity
            // setLinearVelocity(directionVector);

            // rigidBody->applyForce(directionVector, rigidBody->getWorldTransform().getOrigin());
            rigidBody->applyCentralImpulse(directionVector);

            // rigidBody->setGravity(defaultGravity + directionVector);
            //applyFPSMoveForce(directionVector);


            /*
            if(rigidBody->getLinearVelocity().length() < maxSpeed)
            {
                // rigidBody->applyCentralImpulse(directionVector);
                rigidBody->applyForce(directionVector, rigidBody->getWorldTransform().getOrigin());
            }*/
        }

        else
        {
            rigidBody->setDamping(CollisionCamera::airDamping, 0.0);
            rigidBody->setFriction(CollisionCamera::airFriction);
            rigidBody->setGravity(defaultGravity * gravityModifier);
        }

        btVector3 pos = getPosition(); // Obtain our current position
        mCamera->setPosition(pos.x(), pos.y() + cameraHeight, pos.z());
        //        gameplay::astar.setPos(pathID,mCamera->getPosition());
    }

    catch(...)
    {
        std::cout << "ERROR in CollisionCamera::fpsMove()" << std::endl;
    }
}

void CollisionCamera::applyFPSMoveForce(Vec3 force)
{
    boost::chrono::steady_clock::time_point time = entropy::clock.now();

    if(time - lastMovementTime > movementInterval)
    {
        btRigidBody* body = upcastObject();
        body->applyForce(force, body->getWorldTransform().getOrigin());
        // body->applyCentralImpulse(force);
        lastMovementTime = time;
    }

    // rigidBody->applyForce(directionVector, rigidBody->getWorldTransform().getOrigin());
    // boost::chrono::time_point

    /*
    if(force < maxSpeed)
    { = boost::chrono::steady_clock::time_point(boost::chrono::milliseconds(0)); = boost::chrono::steady_clock::time_point(boost::chrono::milliseconds(0));
        // rigidBody->applyCentralImpulse(directionVector);
    }*/
}

void CollisionCamera::flyMove()
{
    try
    {
        Vec3 directionVector = this->mCamera->getDirection();

        activate(true);

        if(shiftDown)
            directionVector *= 21*CollisionCamera::scale*1;
        else
            directionVector *= 6*CollisionCamera::scale*1;

        btScalar rotation;

        switch(keyCombo)
        {
        case D: // Right
            rotation = -HALF_PI;
            break;

        case SD: // Diagonal, Right/Back
            rotation = -THREE_FOURTHS_PI;
            break;

        case S: // Back
            rotation = PI;
            break;

        case AS: // Diagonal, Left/Back
            rotation = THREE_FOURTHS_PI;
            break;

        case ASD: // Same as S, just move backwards
            rotation = PI;
            break;

        case A: // Left
            rotation = HALF_PI;
            break;

        case WA:  // Diagonal, Left/Forward
            rotation = ONE_FOURTH_PI;
            break;

        case W: // Forward
            break;

        case WD: // Diagonal, Right/Forward
            rotation = -ONE_FOURTH_PI;
            break;

        case WAD: // Same as W, just move forward
            break;

        default:
            directionVector.setZero();
            break;
        }

        directionVector = directionVector.rotate(physics::Vec3(mCamera->getUp()), rotation);
        transform.setOrigin(currentPosition + (directionVector * 0.2));
        upcastObject()->getMotionState()->setWorldTransform(transform);
        setWorldTransform(transform);


        btVector3 pos = transform.getOrigin(); // Obtain our current position

        mCamera->setPosition(pos.x(), pos.y() + cameraHeight, pos.z());
    }

    catch(...)
    {
        std::cout << "ERROR in CollisionCamera::flyMove()" << std::endl;
    }
}

void CollisionCamera::updateObject()
{
    //boost::shared_lock<boost::shared_mutex> lock(mutex);
    //boost::upgrade_lock<boost::shared_mutex> lock(mutex);
    //boost::upgrade_to_unique_lock<boost::shared_mutex> writeLock(lock);

    if(object != 0)
    {
        if(crouching)
        {
            if(cameraHeight > crouchHeight)
                cameraHeight -= 0.125*CollisionCamera::scale;
        }

        else if(cameraHeight < standingHeight)
        {
            cameraHeight += 0.125*CollisionCamera::scale;
        }

        transform = getWorldTransform();

        liftTransform.setIdentity();
        geometry::Euler objectRotation = eulerRotation;
        objectRotation.setRoll(Ogre::Radian(0));
        //objectRotation.setPitch(Ogre::Radian(0));
        //objectRotation.yaw(Ogre::Radian(-HALF_PI));
        liftTransform.setRotation(physics::Quaternion(objectRotation.toQuaternion()));
        Vec3 offsetLift((viewDirection * ((pickedObjectWidth) + halfHeight)) + Ogre::Vector3(0, cameraHeight, 0));

        if(offsetLift.y() < (pickedObjectHeight / 8.0))
            offsetLift.setY(pickedObjectHeight / 8.0);

        //offsetLift.setY(std::max(pickedObjectHalfHeight * 1.1, offsetLift.y()));
        liftTransform.setOrigin(getWorldTransform().getOrigin() + offsetLift);

        if(internalLiftSphere)
            internalLiftSphere->setWorldTransform(liftTransform);

        currentPosition = transform.getOrigin();

        if(flyMode)
        {
            flyMove();
        }

        else
        {
            fpsMove();
        }

        // Ray casting for testing whether we're on top of something
        btTransform xForm;
        getMotionStateWorldTransform(xForm);
        down = -xForm.getBasis()[1];
        down.normalize();

        raySource = xForm.getOrigin();
        rayTarget = raySource + (down * halfHeight * btScalar(1.1));

        ClosestNotMe rayCallback(object);
        rayCallback.m_closestHitFraction = 1.0;
        physics::world.getDynamicsWorld()->rayTest(raySource, rayTarget, rayCallback);

        if(rayCallback.hasHit())
        {
            rayLambda = rayCallback.m_closestHitFraction;
        }

        else
        {
            rayLambda = 1.0;
        }

        /*
        if(pickedObject)
        {
            if(rayCallback.m_collisionObject == pickedObject->getObject())
            {
                dropObject();
            }
        }

        // 3D Picking
        physics::Vec3 direction(mCamera->getDirection() * CollisionCamera::pickingHandDistance);

        if(!liftingObject)
        {
            pickedObject = physics::objectPick(
                        transform.getOrigin() + btVector3(0, cameraHeight, 0),
                        transform.getOrigin() + btVector3(0, cameraHeight, 0) + direction
                        );

            // btVector3 from(transform.getOrigin() + btVector3(0, cameraHeight, 0));
            // btVector3 to(transform.getOrigin() + btVector3(0, cameraHeight, 0) + direction);
            // std::cout << "CollisionCamera::updateObject() -> from(" << from.x() << ", " << from.y() << ", " << from.z() << ")" << std::endl;
            // std::cout << "CollisionCamera::updateObject() -> to(" << to.x() << ", " << to.y() << ", " << to.z() << ")" << std::endl;
        }

        if(pickedObject) // If we found an object, change our cursor
        {
            EntropyCursor::getSingletonPtr()->setCursor(pickedObject->getCursorType());
        } else {
            EntropyCursor::getSingletonPtr()->setCursor(physics::NoCursor);
        }

        if(liftingObject)
        {
            if(liftConstraint->getAppliedImpulse() > 6000 || liftHinge->getAppliedImpulse() > 6000)
            {
                dropObject();
            }
        }*/

        actorNode->setPosition(mCamera->getPosition());
        actorNode->setOrientation(mCamera->getOrientation());
        actorNode->_update(true, false);
    }

    gameplay::astar.setPos(pathID,mCamera->getPosition());
}

void CollisionCamera::bitShiftWASD() // convert to a keyCombo number by bitshifting the bits in the wasdBitArray
{
    keyCombo = 0;

    for(int i = 0; i < numDirectionKeys; ++i)
    {
        keyCombo += wasdBitArray[i] << i;
    }

}

int CollisionCamera::getPathID()
{
    return pathID;
}

void CollisionCamera::updatePhysicsMode()
{
    switch(physicsMode)
    {
    case StandardPhysics:
        gravityModifier = 1.0;
        if(Entropy::getSingletonPtr()->terrainGenerator.use_count()>0)
        {
            Entropy::getSingletonPtr()->terrainGenerator->getWater()->setEnabled(false);
        }
        break;

    case WaterPhysics:
        gravityModifier = 0.025;
        if(Entropy::getSingletonPtr()->terrainGenerator.use_count()>0)
        {
            Entropy::getSingletonPtr()->terrainGenerator->getWater()->setEnabled(true);
        }
        break;
    }
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> CollisionCamera* Ogre::Singleton<CollisionCamera>::ms_Singleton = 0;
CollisionCamera* CollisionCamera::getSingletonPtr(void)
{
    return ms_Singleton;
}
CollisionCamera& CollisionCamera::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> CollisionCamera* Ogre::Singleton<CollisionCamera>::msSingleton = 0;
CollisionCamera* CollisionCamera::getSingletonPtr(void)
{
    return msSingleton;
}
CollisionCamera& CollisionCamera::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
