// System includes
#include <iostream>

// Ogre includes
#include "OGRE/OgreRenderSystem.h"

// Bullet includes
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
//#include "BulletSoftBody/btSoftBodySolvers.h"
//#include "BulletSoftBody/btDefaultSoftBodySolver.h"
#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

// Entropy includes
#include "world.h"
#include "src/Graphics/OgreDebugDraw/DebugDrawer.h"
#include "src/Physics/physics.h"
#include "src/Physics/scene.h"
#include "src/Core/collisioncamera.h"
#include "src/Graphics/graphics.h"

#define MAX_PLAYER_SPEED 500.0

namespace physics {

void testResponse(PhysicsObject* object0, PhysicsObject* object1)
{
    std::cout << "Collision Reponse. Group0: " << object0->getCollisionGroup() << " Group1: " << object1->getCollisionGroup()
              << std::endl;
}

// Prevent the player from going too fast.
void tickCallback(btDynamicsWorld* world, btScalar timeStep)
{
    btRigidBody* player = btRigidBody::upcast(CollisionCamera::getSingletonPtr()->getObject());
    btVector3 velocity = player->getLinearVelocity();
    btScalar speed = velocity.length();

    if(speed > MAX_PLAYER_SPEED)
    {
        velocity *= MAX_PLAYER_SPEED / speed;
        player->setLinearVelocity(velocity);
    }
}

// btScalar World::timeStep = 16.0 / 1000.0;
btScalar World::timeStep = 4.0 / 1000.0;

World::World() :
    running(false),
    overlapFilterCallback(0)
{
    initPhysics();
}

World::~World()
{
    exitPhysics();
}

void World::initPhysics()
{
    collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

    //use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    //dispatcher = new btCollisionDispatcher(collisionConfiguration);
    dispatcher = new physics::EventDispatcher(collisionConfiguration);

    broadphase = new bt32BitAxisSweep3(btVector3(-10000, -10000, -10000), btVector3(10000, 10000, 10000));

    //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver();

    //dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -180, 0));
    ghostPairCallback = new btGhostPairCallback();
    dynamicsWorld->getPairCache()->setInternalGhostPairCallback(ghostPairCallback);
}

void World::exitPhysics()
{
    //cleanup in the reverse order of creation/initialization
    //remove the rigidbodies from the dynamics world and delete them
    for(int i = dynamicsWorld->getNumCollisionObjects() - 1;  i >= 0; --i)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);

        if(body && body->getMotionState())
        {
            delete body->getMotionState();
        }

        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    collisionShapes.clear();

    if(overlapFilterCallback)
        delete overlapFilterCallback;

    delete dynamicsWorld;
    delete solver;
    delete broadphase;
    delete dispatcher;
    delete collisionConfiguration;
    delete ghostPairCallback;
}

void World::setGravity(const btVector3 &gravity)
{
    dynamicsWorld->setGravity(gravity);
}

void World::update(double timeSinceLastFrame)
{
    if(!paused)
    {
        // Add Constraints
        addConstraintMutex.lock();

        while(addConstraintDeque.size() > 0)
        {
            addConstraintArgs args = addConstraintDeque.front();
            dynamicsWorld->addConstraint(args.constraint, args.disableCollisionBetweenLinkedObjects);
            addConstraintDeque.pop_front();
        }

        addConstraintMutex.unlock();

        // Remove Constraints
        removeConstraintMutex.lock();

        while(removeConstraintDeque.size() > 0)
        {
            btTypedConstraint* constraint = removeConstraintDeque.front();
            dynamicsWorld->removeConstraint(constraint);
            removeConstraintDeque.pop_front();
        }

        removeConstraintMutex.unlock();

        // Remove Objects / RigidBodies / SoftBodies
        removeCollisionObjectMutex.lock();

        while(removeCollisionObjectDeque.size() > 0)
        {
            btCollisionObject* object = removeCollisionObjectDeque.front();
            dynamicsWorld->removeCollisionObject(object);
            removeCollisionObjectDeque.pop_front();
        }

        while(removeRigidBodyDeque.size() > 0)
        {
            btRigidBody* body = removeRigidBodyDeque.front();
            dynamicsWorld->removeRigidBody(body);
            removeRigidBodyDeque.pop_front();
        }

        while(removeSoftBodyDeque.size() > 0)
        {
            btSoftBody* body = removeSoftBodyDeque.front();
            dynamicsWorld->removeSoftBody(body);
            removeSoftBodyDeque.pop_front();
        }

        removeCollisionObjectMutex.unlock();

        // Add Objects
        addCollisionObjectMutex.lock();

        while(addCollisionObjectDeque.size() > 0)
        {
            addCollisionObjectArgs args = addCollisionObjectDeque.front();
            dynamicsWorld->addCollisionObject(args.body, args.group, args.mask);
            addCollisionObjectDeque.pop_front();
        }

        addCollisionObjectMutex.unlock();

        // RigidBodies
        addRigidBodyMutex.lock();

        while(addRigidBodyDeque.size() > 0)
        {
            addRigidBodyArgs args = addRigidBodyDeque.front();
            dynamicsWorld->addRigidBody(args.body, args.group, args.mask);
            addRigidBodyDeque.pop_front();
        }

        addRigidBodyMutex.unlock();

        // SoftBodies
        addSoftBodyMutex.lock();

        while(addSoftBodyDeque.size() > 0)
        {
            addSoftBodyArgs args = addSoftBodyDeque.front();
            dynamicsWorld->addSoftBody(args.body, args.group, args.mask);
            addSoftBodyDeque.pop_front();
        }

        addSoftBodyMutex.unlock();

        voxelMutex.lock();
        for(int i=0;i<1;++i)
        {
            if(!voxelPhysicsFunctions.empty())
            {
                if(!voxelPhysicsFunctions.front().func.empty())
                {
                    try
                    {
                        voxelPhysicsFunctions.front().func(voxelPhysicsFunctions.front().id,voxelPhysicsFunctions.front().shape,voxelPhysicsFunctions.front().position);
                        voxelPhysicsFunctions.pop_front();
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "Exception executing voxel physics function: " << e.what() << std::endl;
                    }
                }
            }
            else
            {
                break;
            }
        }
        voxelMutex.unlock();

        if(dynamicsWorld)
        {
            // Step the simulations
            try
            {
                dynamicsWorld->stepSimulation(timeSinceLastFrame, 3, 1.0 / 240.0);
            }

            catch(std::exception &e)
            {
                std::cout << "Physics world simulation exception :" << std::endl;
            }
        }
    }
}

void World::drawDebug()
{
    dynamicsWorld->debugDrawWorld();
    const btSoftBodyArray& array = dynamicsWorld->getSoftBodyArray();

    // Softbody debug drawing
    for(int i = 0; i < array.size(); ++i)
    {
        btSoftBody*	psb = (btSoftBody*) array.at(i);
        btSoftBodyHelpers::DrawFrame(psb, &graphics::detail::bulletDebugDraw);
        btSoftBodyHelpers::Draw(psb, &graphics::detail::bulletDebugDraw, dynamicsWorld->getDrawFlags());
        btSoftBodyHelpers::DrawNodeTree(psb, &graphics::detail::bulletDebugDraw);
    }
}

void World::resetScene()
{
    exitPhysics();
    initPhysics();
}

void World::eagerAddRigidBody(btRigidBody *body, short group, short mask)
{
    try
    {
        dynamicsWorld->addRigidBody(body, group, mask);
    } catch(std::exception &e)
    {
        std::cerr << "Exception in World::eagerAddRigidBody: " << e.what() << std::endl;
    }
}

void World::addRigidBody(btRigidBody* body, short group, short mask)
{
    // dynamicsWorld->addRigidBody(body, group, mask);
    addRigidBodyMutex.lock();
    addRigidBodyDeque.push_back(addRigidBodyArgs(body, group, mask));
    addRigidBodyMutex.unlock();
}

void World::addSoftBody(btSoftBody* body, short group, short mask)
{
    // dynamicsWorld->addSoftBody(body, group, mask);
    addSoftBodyMutex.lock();
    addSoftBodyDeque.push_back(addSoftBodyArgs(body, group, mask));
    addSoftBodyMutex.unlock();
}

void World::addConstraint(btTypedConstraint* constraint, bool disableCollisionBetweenLinkedObjects)
{
    // dynamicsWorld->addConstraint(constraint, disableCollisionBetweenLinkedObjects);
    addConstraintMutex.lock();
    addConstraintDeque.push_back(addConstraintArgs(constraint, disableCollisionBetweenLinkedObjects));
    addConstraintMutex.unlock();
}

void World::addCollisionObject(btCollisionObject* object, short group, short mask)
{
    // dynamicsWorld->addCollisionObject(object, group, mask);
    addCollisionObjectMutex.lock();
    addCollisionObjectDeque.push_back(addCollisionObjectArgs(object, group, mask));
    addCollisionObjectMutex.unlock();
}

void World::eagerRemoveCollisionObject(btCollisionObject *object)
{
    if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
    {
        btRigidBody* body = btRigidBody::upcast(object);

        while(body->getNumConstraintRefs())
        {
            btTypedConstraint* constraint = body->getConstraintRef(0);
            dynamicsWorld->removeConstraint(constraint);
        }

        dynamicsWorld->removeRigidBody(body);
    }

    else if(object->getInternalType() == btCollisionObject::CO_SOFT_BODY)
    {
        btSoftBody* softBody = btSoftBody::upcast(object);
        dynamicsWorld->removeSoftBody(softBody);
    }

    else
    {
        dynamicsWorld->removeCollisionObject(object);
    }
}

void World::removeCollisionObject(btCollisionObject* object)
{
    removeCollisionObjectMutex.lock();

    if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
    {
        btRigidBody* body = btRigidBody::upcast(object);

        for(int i = 0; i < body->getNumConstraintRefs(); ++i)
        {
            btTypedConstraint* constraint = body->getConstraintRef(i);
            removeConstraint(constraint);
        }

        removeRigidBodyDeque.push_back(body);
    }

    else if(object->getInternalType() == btCollisionObject::CO_SOFT_BODY)
    {
        btSoftBody* softBody = btSoftBody::upcast(object);
        removeSoftBodyDeque.push_back(softBody);
    }

    else
    {
        removeCollisionObjectDeque.push_back(object);
    }

    removeCollisionObjectMutex.unlock();
}

void World::removeConstraint(btTypedConstraint* constraint)
{
    // dynamicsWorld->removeConstraint(constraint);

    removeConstraintMutex.lock();
    removeConstraintDeque.push_back(constraint);
    removeConstraintMutex.unlock();
}

void World::setDebugDrawer(btIDebugDraw* debugDraw)
{
    dynamicsWorld->setDebugDrawer(debugDraw);
    dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

btSoftRigidDynamicsWorld* World::getDynamicsWorld()
{
    return dynamicsWorld;
}

EventDispatcher* World::getDispatcher()
{
    return dispatcher;
}

collision_group_id_t World::addCollisionResponse(collision_group_id_t group0, collision_group_id_t group1,
                                                 gameplay::CollisionAction* collisionAction)
{
    return dispatcher->addCollisionResponse(group0, group1, collisionAction);
}

void World::removeCollisionResponse(collision_group_id_t groupCombination)
{
    dispatcher->removeCollisionResponse(groupCombination);
}

void World::removeCollisionResponse(collision_group_id_t group0, collision_group_id_t group1)
{
    dispatcher->removeCollisionResponse(group0, group1);
}

void World::addCollisionFilter(btOverlapFilterCallback* callback)
{
    if(overlapFilterCallback)
    {
        delete overlapFilterCallback;
    }

    dynamicsWorld->getPairCache()->setOverlapFilterCallback(callback);
    overlapFilterCallback = callback;
}

void World::removeCollisionFilter()
{
    dynamicsWorld->getPairCache()->setOverlapFilterCallback(0);
    delete overlapFilterCallback;
    overlapFilterCallback = 0;
}

void World::addVoxelFunction(VoxelPhysicsFunction_t func, int id, voxel::Mesh::ptr shape, Ogre::Vector3 position)
{
    voxelMutex.lock();
    VoxelPhysicsFuncStruct funcStruct = {func, id, shape, position};
    voxelPhysicsFunctions.push_back(funcStruct);
    voxelMutex.unlock();
}

bool World::getPaused()
{
    return paused;
}

void World::setPaused(bool paused)
{
    this->paused = paused;
}

} // physics namespace
