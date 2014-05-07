#ifdef __APPLE__
#include <boost/thread/mutex.hpp>
#else
#include <mutex>
#endif


// Bullet includes
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletSoftBody/btSoftBody.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "btBulletCollisionCommon.h"
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

// Entropy includes
#include "physics.h"
#include "world.h"
#include "src/Physics/scene.h"
#include "src/Physics/waterobject.h"

namespace physics {


World world;
std::vector<GhostObject*> sensors; // Sensors used for updating (like water, lava, etc...)
#ifdef __APPLE__
boost::mutex sensorMutex;
#else
std::mutex sensorMutex;
#endif

namespace { // Anonymous namespace

    btCollisionObject* initializeCollisionObject(btCollisionShape* colShape, const Vec3& origin, btScalar mass, const Quaternion& rotation,
                                                 BodyType type, bool addToWorld, void* userPointer, short group, short mask)
    {
        // Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();
        startTransform.setRotation(rotation);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass,localInertia);

        startTransform.setOrigin(origin);

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

        if(type == Rigid)
        {
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
            btRigidBody* body = new btRigidBody(rbInfo);
            body->setUserPointer(userPointer);

            if(addToWorld)
                world.addRigidBody(body, group, mask);

            return body;
        }

        else
        {
            if(type == Ghost)
            {
                btGhostObject* object = new btGhostObject();
                object->setCollisionShape(colShape);
                object->setWorldTransform(startTransform);
                object->setCollisionFlags(object->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
                object->setUserPointer(userPointer);

                if(addToWorld)
                    world.addCollisionObject(object, group, mask);

                return object;
            }

            else if(type == Soft)
            {
                btSoftBodyWorldInfo info;
                btSoftBody* body = new btSoftBody(&info);
                body->setUserPointer(userPointer);

                if(addToWorld)
                    world.addCollisionObject(body, group, mask);

                return body;
            }

            else
                return 0;
        }
    }

    void initializeSoftBody(btSoftBody* body, btScalar mass, void* userPointer, short group, short mask)
    {
        // btTransform transform = body->getWorldTransform();
        // transform.setOrigin(origin);
        // transform.setRotation(rotation);
        // body->setWorldTransform(transform);
        body->setUserPointer(userPointer);
        body->setTotalMass(mass);
        // body->generateBendingConstraints(2);
        // body->m_cfg.piterations = 2;
        // body->m_cfg.collisions = btSoftBody::fCollision::CL_RS;
        // body->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS + btSoftBody::fCollision::VF_SS;
        // body->m_cfg.collisions = btSoftBody::fCollision::VF_SS;
        // body->generateClusters(64);

        // body->m_cfg.kDF = 0.5;
        // body->m_cfg.kMT = 0.05;
        // body->m_cfg.piterations	= 5;
        // btSoftBody::Material* pm = body->appendMaterial();
        // pm->m_kLST = 0.5;
        // pm->m_flags -= btSoftBody::fMaterial::DebugDraw;
        // body->generateBendingConstraints(2, pm);
        // body->m_cfg.piterations = 3;
        // body->m_cfg.kDF = 0.5;
        // body->scale(btVector3(1, 1, 1));
        // body->setTotalMass(100, true);
        // body->randomizeConstraints();
        // body->randomizeConstraints();
        // body->scale(btVector3(6,6,6));
        // body->setTotalMass(100, true);
        // body->setPose(false,true);

        // body->getCollisionShape()->setMargin(0.05);
        body->generateClusters(64);
        //enable cluster collision between soft body and rigid body
        // body->m_cfg.collisions += btSoftBody::fCollision::CL_RS;
        //enable cluster collision between soft body and soft body
        // body->m_cfg.collisions += btSoftBody::fCollision::CL_SS;
        body->m_cfg.collisions += btSoftBody::fCollision::VF_SS + btSoftBody::fCollision::CL_SELF;
        world.addSoftBody(body, group, mask);
    }

} // Anonymous namespace

std::vector<PhysicsActor*> loadMap(TiXmlDocument* document)
{
    std::vector<PhysicsActor*> actors;
    Scene scene("Test");
    importScene(&scene, document);

    const std::vector<SceneObject*> objects = scene.getObjects();

    for(int i = 0; i < objects.size(); ++i)
    {
        SceneObject* object = objects[i];

        if(object->getPhysicsEnabled())
        {
            Vec3 position = object->getPosition();
            Vec3 scale = object->getScale();
            double mass = object->getMass();
            Quaternion rotation = object->getRotation();
            BodyType bodyType = object->getBodyType();
            ShapeType shapeType = object->getShapeType();
            collision_group_id_t collisionGroup = object->getCollisionGroup();
            CursorType cursorType = object->getCursorType();
            bool hollow = object->getHollow();

            if(hollow)
            {
                createCollisionRoom(position, scale, mass, rotation, bodyType);
            }

            else
            {
                btCollisionObject* collisionObject;

                switch(shapeType)
                {
                case Box:
                    collisionObject = createCollisionBox(position, scale, mass, rotation, bodyType);
                    break;

                case Sphere:
                    collisionObject = createCollisionSphere(position, scale.x(), mass, rotation, bodyType);
                    break;
                }

                PhysicsActor* actor = new PhysicsActor(object->getName(), collisionObject, 0, collisionGroup, cursorType, false);
                actors.push_back(actor);
            }
        }
    }

    return actors;
}

btCollisionObject* createCollisionBox(const Vec3& origin, const Vec3& boxHalfExtents, btScalar mass, const Quaternion& rotation, BodyType type,
                                      bool addToWorld, void* userPointer, short group, short mask)
{
    btCollisionShape* colShape = new btBoxShape(boxHalfExtents);
    return initializeCollisionObject(colShape, origin, mass, rotation, type, addToWorld, userPointer, group, mask);
}

btCollisionObject* createCollisionSphere(const Vec3& origin, btScalar scale, btScalar mass, const Quaternion& rotation, BodyType type,
                                         bool addToWorld, void* userPointer, short group, short mask)
{
    btCollisionShape* colShape = new btSphereShape(scale);
    return initializeCollisionObject(colShape, origin, mass, rotation, type, addToWorld, userPointer, group, mask);
}

btCollisionObject* createCollisionMultiSphere(const Vec3& origin, const btVector3* positions, const btScalar* radi, int numSpheres,
                                              btScalar mass, const Quaternion& rotation, BodyType type, bool addToWorld, void* userPointer,
                                              short group, short mask)
{
    btCollisionShape* colShape = new btMultiSphereShape(positions, radi, numSpheres);
    return initializeCollisionObject(colShape, origin, mass, rotation, type, addToWorld, userPointer, group, mask);
}

btCollisionObject* createCollisionSphereStack(const Vec3& origin, const btScalar& height, const btScalar& radius, int numSpheres,
                                              btScalar mass, const Quaternion& rotation, BodyType type, bool addToWorld, void* userPointer,
                                              short group, short mask)
{
    btVector3* positions = new btVector3[numSpheres];
    btScalar* radi = new btScalar[numSpheres];
    btScalar spacing = height / numSpheres;
    btVector3 initialPosition = btVector3(0, ((height / 2.0) * -1) + (spacing / 2.0), 0);

    for(int i = 0; i < numSpheres; ++i)
    {
        positions[i] = initialPosition + btVector3(0, spacing * i, 0);
        radi[i] = radius;
    }

    return createCollisionMultiSphere(origin, positions, radi, numSpheres, mass, rotation, type, addToWorld, userPointer, group, mask);
}

btCollisionObject* createCollisionCapsule(const Vec3& origin, btScalar radius, btScalar height, btScalar mass,
                                          const Quaternion& rotation, BodyType type, bool addToWorld, void *userPointer,
                                          short group, short mask)
{
    btCollisionShape* colShape = new btCapsuleShape(radius, height);
    return initializeCollisionObject(colShape, origin, mass, rotation, type, addToWorld, userPointer, group, mask);
}

btCollisionObject* createCollisionCylinder(const Vec3& origin, const Vec3& halfExtents, btScalar mass, const Quaternion& rotation, BodyType type,
                                           bool addToWorld, void *userPointer, short group, short mask)
{
    btCollisionShape* colShape = new btCylinderShape(halfExtents);
    return initializeCollisionObject(colShape, origin, mass, rotation, type, addToWorld, userPointer, group, mask);
}

btCollisionObject* createCollisionCone(const Vec3& origin, btScalar radius, btScalar height, btScalar mass, const Quaternion& rotation,
                                       BodyType type, bool addToWorld, void *userPointer, short group, short mask)
{
    btCollisionShape* colShape = new btConeShape(radius, height);
    return initializeCollisionObject(colShape, origin, mass, rotation, type, addToWorld, userPointer, group, mask);
}

btCollisionObject* createProjectile(const Vec3& origin, const Vec3& direction, btScalar force, btScalar scale, btScalar mass,
                                    const Quaternion& rotation, void* userPointer, short group, short mask)
{
    btRigidBody* projectile = btRigidBody::upcast(createCollisionSphere(origin + (direction * 10), scale, mass, rotation,
                                                                        Rigid, true, userPointer, group, mask));
    projectile->setLinearVelocity(direction * force);

    return projectile;
}

std::vector<btCollisionObject*> createCollisionRoom(const Vec3& origin, const Vec3& roomHalfExtents, btScalar wallMass, const Quaternion& rotation,
                                                    BodyType type, bool addToWorld, void* userPointer, short group, short mask)
{
    std::vector<btCollisionObject*> room;

    int numberOfFaces = 6;
    for(int i = 0; i < numberOfFaces; ++i)
    {
        Vec3 faceOrigin, faceExtents;

        switch(i)
        {
        case North:
            faceOrigin = Vec3(origin.x(), origin.y(), origin.z() + roomHalfExtents.z());
            faceExtents = Vec3(roomHalfExtents.x(), roomHalfExtents.y(), 1.0);
            break;

        case South:
            faceOrigin = Vec3(origin.x(), origin.y(), origin.z() - roomHalfExtents.z());
            faceExtents = Vec3(roomHalfExtents.x(), roomHalfExtents.y(), 1.0);
            break;

        case East:
            faceOrigin = Vec3(origin.x() + roomHalfExtents.x(), origin.y(), origin.z());
            faceExtents = Vec3(1.0, roomHalfExtents.y(), roomHalfExtents.z());
            break;

        case West:
            faceOrigin = Vec3(origin.x() - roomHalfExtents.x(), origin.y(), origin.z());
            faceExtents = Vec3(1.0, roomHalfExtents.y(), roomHalfExtents.z());
            break;

        case Top:
            faceOrigin = Vec3(origin.x(), origin.y() + roomHalfExtents.y(), origin.z());
            faceExtents = Vec3(roomHalfExtents.x(), 1.0, roomHalfExtents.z());
            break;

        case Bottom:
            faceOrigin = Vec3(origin.x(), origin.y() - roomHalfExtents.y(), origin.z());
            faceExtents = Vec3(roomHalfExtents.x(), 1.0, roomHalfExtents.z());
            break;
        }

        btCollisionObject* face = createCollisionBox(faceOrigin, faceExtents, wallMass, rotation, type, addToWorld,
                                                     userPointer, group, mask);
        room.push_back(face);
    }

    return room;
}

btCollisionObject* createCollisionRope(const btVector3& from, const btVector3& to, int res, int fixeds,
                                       btScalar mass, void* userPointer, short group, short mask)
{
    btSoftBody* body = btSoftBodyHelpers::CreateRope(world.getDynamicsWorld()->getWorldInfo(), from, to, res, fixeds);
    body->setFriction(2.0);
    initializeSoftBody(body, mass, userPointer, group, mask);
    return body;
}

void spawnTestRope(float x0, float y0, float z0, float x1, float y1, float z1)
{
    float mass = 1.0;
    btVector3 from(x0, y0, z0);
    btVector3 to(x1, y1, z1);
    int res = (to - from).length();
    res = res >= 2 ? res : 2;
    int fixeds = 2;
    createCollisionRope(btVector3(x0, y0, z0), btVector3(x1, y1, z1), res, fixeds, mass);
}

btCollisionObject* createCollisionBlob(const btVector3 origin, btScalar radius, btScalar mass, void* userPointer,
                                       short group, short mask)
{
    btSoftBody* body = btSoftBodyHelpers::CreateEllipsoid(
                world.getDynamicsWorld()->getWorldInfo(),
                origin,
                btVector3(radius, radius, radius),
                128
    );

    body->getCollisionShape()->setMargin(1);
    initializeSoftBody(body, mass, userPointer, group, mask);
    return body;
}

void spawnBlob(float x, float y, float z, float radius)
{
    createCollisionBlob(btVector3(x, y, z), radius);
}

void removeCollisionObject(btCollisionObject* object)
{
    world.removeCollisionObject(object);
}

PhysicsObject* objectPick(const Vec3& rayFromWorld, const Vec3& rayToWorld)
{
    // World::world_read_t readLock(world.mutex);

    PhysicsObject* result = 0;

    btCollisionWorld::ClosestRayResultCallback callback = objectPickRay(rayFromWorld, rayToWorld);

    if(callback.hasHit())
    {
        if(callback.m_collisionObject->getUserPointer() != 0)
            result = (PhysicsObject*) callback.m_collisionObject->getUserPointer();
    }

    return result;
}

btCollisionWorld::ClosestRayResultCallback objectPickRay(const Vec3& rayFromWorld, const Vec3& rayToWorld)
{
    btCollisionWorld::ClosestRayResultCallback callback(rayFromWorld, rayToWorld);
    world.getDynamicsWorld()->rayTest(rayFromWorld, rayToWorld, callback);
    return callback;
}

ClosestNotMe objectPick(const Vec3& rayFromWorld, const Vec3& rayToWorld, btCollisionObject* excludedObject)
{
    ClosestNotMe rayCallback(excludedObject);
    rayCallback.m_closestHitFraction = 1.0;
    physics::world.getDynamicsWorld()->rayTest(rayFromWorld, rayToWorld, rayCallback);

    return rayCallback;
}

btPoint2PointConstraint* createPointConstraint(btRigidBody* body0, btRigidBody* body1, const Vec3& pivotInA, const Vec3& pivotInB)
{
    btPoint2PointConstraint* constraint = new btPoint2PointConstraint(
                *body0,
                *body1,
                pivotInA,
                pivotInB
    );

    world.addConstraint(constraint);
    return constraint;
}

btGeneric6DofConstraint* createGenericConstraint(btRigidBody* body0, btRigidBody* body1, const btTransform& frameInA,
                                                 const btTransform& frameInB, bool useLinearReferenceFrameB)
{
    btGeneric6DofConstraint* constraint = new btGeneric6DofConstraint(*body0, *body1, frameInA, frameInB, useLinearReferenceFrameB);
    world.addConstraint(constraint);
    return constraint;
}

btHingeConstraint* createHingeConstraint(btRigidBody* body0, btRigidBody* body1, const btTransform& rbAFrame,
                                         const btTransform& rbBFrame, bool useReferenceFrameA)
{
    btHingeConstraint* constraint = new btHingeConstraint(*body0, *body1, rbAFrame, rbBFrame, useReferenceFrameA);
    world.addConstraint(constraint);
    return constraint;
}

void removeConstraint(btTypedConstraint* constraint)
{
    world.removeConstraint(constraint);
    delete constraint;
    constraint = 0;
    //std::cout << "Remove Constraint!" << std::endl;
}

// Returns the unique group combination number
collision_group_id_t addCollisionResponse(collision_group_id_t group0, collision_group_id_t group1,
                         gameplay::CollisionAction* collisionAction)
{
    return world.getDispatcher()->addCollisionResponse(group0, group1, collisionAction);
}

// You can remove by either calling the unique combination or just enumerating the two groups
void removeCollisionResponse(collision_group_id_t groupCombination)
{
    world.getDispatcher()->removeCollisionResponse(groupCombination);
}

void removeCollisionResponse(collision_group_id_t group0, collision_group_id_t group1)
{
    world.getDispatcher()->removeCollisionResponse(group0, group1);
}

void addDefaultCollisionActions()
{
    // WaterObject::addDefaultCollisionActions();
}

void removeDefaultCollisionActions()
{
    // WaterObject::removeDefaultCollisionActions();
}

void addSensorUpdate(GhostObject* ghost)
{
    sensorMutex.lock();
    sensors.push_back(ghost);
    sensorMutex.unlock();
}

void removeSensorUpdate(GhostObject *ghost)
{
    sensorMutex.lock();

    for(int i = 0; i < sensors.size(); ++i)
    {
        if(ghost == sensors.at(i))
        {
            sensors.erase(sensors.begin() + i);
            break;
        }
    }

    sensorMutex.unlock();
}

void updateSensors()
{
    sensorMutex.lock();

    for(int i = 0; i < sensors.size(); ++i)
    {
        sensors.at(i)->update();
    }

    sensorMutex.unlock();
}

/////////////////
// Lua Interface
/////////////////

std::tr1::unordered_map<int, btCollisionObject*> objectMap;

void setGravity(double x, double y, double z)
{
    world.setGravity(btVector3(x, y, z));
}

void applyForce(int objectID, double x, double y, double z)
{
    std::tr1::unordered_map<int, btCollisionObject*>::iterator find = objectMap.find(objectID);
    if(find != objectMap.end())
    {
        btRigidBody* object = btRigidBody::upcast(objectMap[objectID]);
        object->applyCentralForce(btVector3(x, y, z));
    }
}

} // Physics namespace
