#ifndef COLLISIONCAMERA_H
#define COLLISIONCAMERA_H

// Standard includes
#include <vector>

// Ogre includes
#include "OGRE/OgreSingleton.h"
#ifndef __APPLE__
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISEvents.h>
#include <OIS/OISMouse.h>
#include <OGRE/SdkCameraMan.h>
#endif
#include <OGRE/OgreSceneNode.h>

#ifndef WIN32
// SDL includes
//#include "SDL2/SDL_events.h"
#endif
#include "Utility/ois2sdl.h"

// Bullet includes
#include "bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "bullet/BulletSoftBody/btSoftBody.h"

// Boost includes
#include "boost/thread/shared_mutex.hpp"
#include "boost/chrono.hpp"
#include "boost/chrono/time_point.hpp"

// Entropy includes
#include "src/Physics/physicsobject.h"
#include "src/Utility/OgreEuler.h"

// bullet forward declaration
class btPoint2PointConstraint;
class btGeneric6DofConstraint;
class btHingeConstraint;

// Ogre forward declaration

class CollisionCamera : public physics::RigidObject, public Ogre::Singleton<CollisionCamera>
{
public:

    /**
    * CollisionCamera a class that joins a physics object to a camera man, fusing the user input to an object in the physics world
    * Ogre::Camera* We need a camera to control. This inherits from SdkCameraMan which allows for user input, but is not a camera itself.
    */
    CollisionCamera(Ogre::SceneManager* manager);
    ~CollisionCamera();

    PhysicsObject* getPickedObject();
    void update(); // General update

    void initCollisionObject(); // Initialize the internal physics object. This MUST be called during setup.
    void setCameraPosition(Ogre::Vector3 position);
    void setCameraPosition(int x, int y, int z);
    geometry::Euler getEuler();
    btRigidBody* getInternalLiftSphere();
    Ogre::Camera* getCamera();
    std::vector<Ogre::Light*>& getFlashlights();

//#ifndef WIN32
    void injectKeyDown(utility::EntKeyCode key);
    void injectKeyUp(utility::EntKeyCode key);

    // void injectMouseMove(SDL_Event event);
    void injectMouseDown(utility::EntKeyCode button);
    void injectMouseUp(utility::EntKeyCode button);
    void mouseMove(double x, double y);
//#endif

    void jump();

    enum KeyCombo // Uses bit shifting to differentiate combinations of keys pressed
    {
        NONE, // 0
        D, // 1
        S, // 2
        SD, // 3
        A, // 4
        AD, // 5
        AS, // 6
        ASD, // 7
        W, // 8
        WD, // 9
        WS, // 10
        WSD, // 11
        WA, // 12
        WAD, // 13
        WAS, // 14
        WASD // 15
    };

    static const int numDirectionKeys = 4; // WASD count
    static float scale;
    static float pickingEyeDistance;
    static float pickingHandDistance;
    static float crouchingSpeed;
    static float walkingSpeed;
    static float runningSpeed;
    static float maxCrouchingSpeed;
    static float maxWalkingSpeed;
    static float maxRunningSpeed;
    static float groundDamping;
    static float airDamping;
    static float groundFriction;
    static float airFriction;
    static boost::chrono::milliseconds crouchingInterval;
    static boost::chrono::milliseconds walkingInterval;
    static boost::chrono::milliseconds runningInterval;
    static const float maxLiftingWeight;
    static const int NUM_ATTENUATION_ARGS = 4;
    static const int NUM_FLASHLIGHTS = 1;
    static float FLASHLIGHT_SPACING_SCALE;
    static float FLASHLIGHT_FOCUS_DISTANCE;

    float flashlightAttenuation[NUM_ATTENUATION_ARGS];
    float flashlightPowerScale;
    float cameraHeight, crouchHeight, standingHeight;

    static void setFlashlightSpacingScale(int flashlightSpacingScale);
    static void setFlashlightFocusDistance(int flashlightFocusDistance);

    static void setFlashLightAttenuation(float range, float constant, float linear, float quadratic)
    {
        //CollisionCamera::getSingletonPtr()->flashlightAttenuation = new float[4];
        CollisionCamera::getSingletonPtr()->flashlightAttenuation[0] = range;
        CollisionCamera::getSingletonPtr()->flashlightAttenuation[1] = constant;
        CollisionCamera::getSingletonPtr()->flashlightAttenuation[2] = linear;
        CollisionCamera::getSingletonPtr()->flashlightAttenuation[3] = quadratic;
    }

    static void setFlashlightPowerscale(float powerscale)
    {
        CollisionCamera::getSingletonPtr()->flashlightPowerScale = powerscale;
    }

    static CollisionCamera& getSingleton(void);
    static CollisionCamera* getSingletonPtr(void);

    int getPathID();

    void resetFlashlightNodes();
    void toggleFlashLights();

private:

    void fpsMove(); // Moves the physics object according to the keyCombo, First Person Shooter style
    void applyFPSMoveForce(physics::Vec3 force); // Applies physics force using a timer based system
    void flyMove(); // Moves the physics object according to the keyCombo, but free look style
    void updateObject(); // Update the internal physics object
    void bitShiftWASD(); // sets the keyCombo variable by collecting the bits in the wasdBitArray
    bool onGround();
    void liftObject();
    void dropObject();
    void throwObject();
    void setCrouching(bool crouching);
    void updateFlashlights();
    void createFlashlights();
    Ogre::Light* createFlashlight(int lightNumber);
    void createFlashlightNodes();
    void destroyFlashlights();
    virtual void updatePhysicsMode();

    boost::shared_mutex mutex; // MultiThread synchronization
    bool flyMode, wDown, aDown, sDown, dDown, spaceDown, shiftDown; // Key booleans
    std::vector<int> wasdBitArray; // Collection of bits used to differentiate combinations of keys pressed
    int keyCombo; // The current combinations of keys pressed
    PhysicsObject* pickedObject;
    Ogre::Camera* mCamera;
    Ogre::SceneNode* actorNode;
    std::vector<Ogre::SceneNode*> flashlightNodes;
    double pitchAngle, pitchAngleSign;
    geometry::Euler eulerRotation;
    btVector3 linearVelocity, currentPosition, raySource, rayTarget, down;
    Ogre::Vector3 viewDirection;
    btScalar rayLambda, halfHeight;
    btTransform transform;
    bool liftingObject, crouching;
    btPoint2PointConstraint* liftConstraint;
    //btGeneric6DofConstraint* liftConstraint;
    btHingeConstraint* liftHinge;
    //btHingeConstraint* liftConstraint1;
    btScalar liftDistance;
    //btRigidBody* internalLiftSphere;
    physics::RigidObject* internalLiftSphere;
    btVector3 pickedObjectAngularFactor;
    btTransform pickedTransform, liftTransform;
    btScalar pickedObjectWidth, pickedObjectHeight;
    btCollisionShape *crouchingShape, *standingShape;
    std::vector<Ogre::Light*> flashlights;
    Ogre::SceneManager* mSceneMgr;
    bool flashlightOn;
    float maxSpeed;
    btVector3 defaultGravity;
    float gravityModifier;
    int pathID;
    boost::chrono::steady_clock::time_point lastMovementTime;
    boost::chrono::milliseconds movementInterval;
};

#endif // COLLISIONCAMERA_H
