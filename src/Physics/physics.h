#ifndef PHYSICS_H
#define PHYSICS_H

// System includes
#include <vector>

#ifndef WIN32
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif
// Ogre includes
#include "OGRE/OgreVector3.h"

// Entropy includes
#include "src/Physics/physicstypes.h"
#include "src/Physics/world.h"
#include "src/Physics/physicsobject.h"
#include "src/Physics/collisiongroupmanager.h"

// bullet includes
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

class btRigidBody;
class btCollisionShape;
class btCollisionObject;
class TiXmlDocument;
class btPoint2PointConstraint;
class btGeneric6DofConstraint;
class btHingeConstraint;
class btDefaultMotionState;

namespace physics {

    extern World world;
    std::vector<PhysicsActor*> loadMap(TiXmlDocument* document);

    // Ray Casting, used for standing/jumping, grappling hook, and other kinds of distance based effects
    class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
    {
    public:
        ClosestNotMe(btCollisionObject* me) :
            btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
        {
            this->me = me;
        }

        virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
        {
            if(rayResult.m_collisionObject == me)
                return 1.0;

            return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
        }

    protected:

        btCollisionObject* me;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // If addToWorld = true, World takes ownership, True for all collision objects
    ////////////////////////////////////////////////////////////////////////////////

    btCollisionObject* createCollisionBox(const Vec3 &origin, const Vec3& boxHalfExtents, btScalar mass = 1.,
                                          const Quaternion& rotation = Quaternion(0, 1, 0, 0), BodyType type = Rigid,
                                          bool addToWorld = true, void* userPointer = 0, short group = COL_MAIN, short mask = mainMask);

    btCollisionObject* createCollisionSphere(const Vec3& origin, btScalar scale, btScalar mass = 1.,
                                             const Quaternion& rotation = Quaternion(0, 1, 0, 0), BodyType type = Rigid,
                                             bool addToWorld = true, void* userPointer = 0, short group = COL_MAIN, short mask = mainMask);

    btCollisionObject* createCollisionMultiSphere(const Vec3& origin, const btVector3* positions, const btScalar* radi, int numSpheres,
                                                  btScalar mass = 1., const Quaternion& rotation = Quaternion(0, 1, 0, 0),
                                                  BodyType type = Rigid, bool addToWorld = true, void* userPointer = 0,
                                                  short group = COL_MAIN, short mask = mainMask);

    // radius is the radius of each sphere, height is the TOTAL height of the stack. Origin will be in the center of the stack.
    btCollisionObject* createCollisionSphereStack(const Vec3& origin, const btScalar& height, const btScalar& radius, int numSpheres,
                                                  btScalar mass = 1., const Quaternion& rotation = Quaternion(0, 1, 0, 0),
                                                  BodyType type = Rigid, bool addToWorld = true, void* userPointer = 0,
                                                  short group = COL_MAIN, short mask = mainMask);

    btCollisionObject* createCollisionCapsule(const Vec3& origin, btScalar radius, btScalar height, btScalar mass = 1.,
                                             const Quaternion& rotation = Quaternion(0, 1, 0, 0), BodyType type = Rigid,
                                             bool addToWorld = true, void* userPointer = 0, short group = COL_MAIN, short mask = mainMask);

    btCollisionObject* createCollisionCylinder(const Vec3& origin,const Vec3& halfExtents, btScalar mass = 1.,
                                               const Quaternion& rotation = Quaternion(0, 1, 0, 0), BodyType type = Rigid,
                                               bool addToWorld = true, void* userPointer = 0);

    btCollisionObject* createCollisionCone(const Vec3& origin, btScalar radius, btScalar height, btScalar mass = 1.,
                                               const Quaternion& rotation = Quaternion(0, 1, 0, 0), BodyType type = Rigid,
                                               bool addToWorld = true, void* userPointer = 0);

    btCollisionObject* createProjectile(const Vec3& origin, const Vec3& direction, btScalar force = 1.0, btScalar scale = 0.25,
                                        btScalar mass = 1., const Quaternion& rotation = Quaternion(0, 1, 0, 0),
                                        void* userPointer = 0, short group = COL_MAIN, short mask = mainMask);

    std::vector<btCollisionObject*> createCollisionRoom(const Vec3& origin, const Vec3& roomHalfExtents, btScalar wallMass = 0.0,
                                                        const Quaternion& rotation = Quaternion(0, 1, 0, 0), BodyType type = Rigid,
                                                        bool addToWorld = true, void* userPointer = 0, short group = COL_MAIN,
                                                        short mask = mainMask);

    // res = number of segments, fixeds = fixed ends. 0: none 1: 1st end is fixed 2: 2nd end fixed 3: both fixed
    btCollisionObject* createCollisionRope(const btVector3& from, const btVector3& to, int res, int fixeds,
                                           btScalar mass = 1., void* userPointer = 0, short group = COL_MAIN, short mask = mainMask);

    void spawnTestRope(float x0, float y0, float z0, float x1, float y1, float z1);

    btCollisionObject* createCollisionBlob(const btVector3 origin, btScalar radius, btScalar mass = 1., void* userPointer = 0,
                                           short group = COL_MAIN, short mask = mainMask);
    void spawnBlob(float x, float y, float z, float radius);

    void removeCollisionObject(btCollisionObject* object);

    btPoint2PointConstraint* createPointConstraint(btRigidBody* body0, btRigidBody* body1, const Vec3& pivotInA, const Vec3& pivotInB);
    btGeneric6DofConstraint* createGenericConstraint(btRigidBody* body0, btRigidBody* body1, const btTransform& frameInA,
                                                     const btTransform& frameInB, bool useLinearReferenceFrameB = true);
    btHingeConstraint* createHingeConstraint(btRigidBody* body0, btRigidBody* body1, const btTransform& rbAFrame,
                                             const btTransform& rbBFrame, bool useReferenceFrameA = false);

    void removeConstraint(btTypedConstraint* constraint); // DELETES OBJECT

    // Returns the physics object pointed at by the ray, if 0 then no object found. If exludedObject is non-zero, it will be
    // excluded from the object picking test.
    PhysicsObject* objectPick(const Vec3& rayFromWorld, const Vec3& rayToWorld);
    btCollisionWorld::ClosestRayResultCallback objectPickRay(const Vec3& rayFromWorld, const Vec3& rayToWorld);
    ClosestNotMe objectPick(const Vec3& rayFromWorld, const Vec3& rayToWorld, btCollisionObject* excludedObject);

    // Returns the unique group combination number
    collision_group_id_t addCollisionResponse(collision_group_id_t group0, collision_group_id_t group1,
                             gameplay::CollisionAction* collisionAction);
    // You can remove by either calling the unique combination or just enumerating the two groups
    void removeCollisionResponse(collision_group_id_t groupCombination);
    void removeCollisionResponse(collision_group_id_t group0, collision_group_id_t group1);

    void addDefaultCollisionActions();
    void removeDefaultCollisionActions();

    void addSensorUpdate(GhostObject* ghost);
    void removeSensorUpdate(GhostObject* ghost);
    void updateSensors();

    //////////////////
    // Lua Interface
    //////////////////
    extern std::tr1::unordered_map<int, btCollisionObject*> objectMap; // Used for mapping Lua ObjectID to btCollisionObject pointers

    void setGravity(double x, double y, double z);
    void applyForce(int objectID, double x, double y, double z);
}

#endif // PHYSICS_H
