#include "rigidentity.h"
#include "Physics/physics.h"
#include "Physics/world.h"
using namespace voxel;

RigidEntity::RigidEntity() :
    entropyPhysicsObject(0, physics::VOXEL_GROUP_ID),
//    m_system(0),
    m_collisionShape(0)
//    m_rigidBody(0)
{

}

RigidEntity::RigidEntity(const Mesh::ptr &shape, double mass) :
//    m_system(sys),
    entropyPhysicsObject(0, physics::VOXEL_GROUP_ID),
    m_transform( btQuaternion(0,0,0,1), btVector3(0, 0, 0) )
{
//    std::cout << "RigidEntity(const Mesh::ptr &shape, double mass)" << std::endl;

    //RESTORE THIS
    m_collisionShape = shape->_shape();
    btVector3 fallInertia = btVector3(0, 0, 0);
    if (mass > 0)
        m_collisionShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,this,m_collisionShape,fallInertia);
    btRigidBody* body = new btRigidBody(rigidBodyCI);
    // Setup custom collision action detection. Used in conjunction with a ContactProcessedCallback (defined in physics::EventDispatcher)
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
//    m_rigidBody = btRigidBodyPtr(body);
    m_rigidBody = body;

    // EventDispatcher CollisionAction support
    m_rigidBody->setUserPointer(&entropyPhysicsObject);
    entropyPhysicsObject.setObject(body);

    //RESTORE THIS
//    sys->_dynamicsWorld()->addRigidBody(m_rigidBody);
}

RigidEntity::~RigidEntity()
{
    std::cout << "~rigidEntity()" << std::endl;
//    physics::world.removeCollisionObject(m_rigidBody.get());
//    m_rigidBody.reset();
//    if (m_system)
//    {
        //RESTORE THIS
//        utility::MutexLocker locker(*m_system);
        //RESTORE THIS
//        (void)locker;
//        m_system->_dynamicsWorld()->removeRigidBody(m_rigidBody);
//        delete m_rigidBody;
//        m_frameListenerConnection.disconnect();
//    }
}

//#ifndef __APPLE__
btScalar RigidEntity::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1,int index1)
{
    m_sigContact(cp.getPositionWorldOnA());
    (void)colObj0;
    (void)partId0;
    (void)index0;
    (void)colObj1;
    (void)partId1;
    (void)index1;
    m_isCollidating = true;
    return 0;
}
//#else
//btScalar RigidEntity::addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1,int index1)
//{
//    m_sigContact(cp.getPositionWorldOnA());
//    (void)colObj0;
//    (void)partId0;
//    (void)index0;
//    (void)colObj1;
//    (void)partId1;
//    (void)index1;
//    m_isCollidating = true;
//    return 0;
//}
//#endif

void RigidEntity::getWorldTransform(btTransform& worldTrans ) const
{
    worldTrans = m_transform;
}

void RigidEntity::setWorldTransform(const btTransform& worldTrans)
{
    m_transform = worldTrans;
}

void RigidEntity::update()
{

}

void RigidEntity::applyForce(Ogre::Vector3 force)
{
    if (force.squaredLength() == 0.0)
        return;
    m_rigidBody->applyCentralForce(btVector3(force.x,force.y,force.z));
    m_rigidBody->activate();
}

void RigidEntity::applyImpulse(Ogre::Vector3 imp)
{
    m_rigidBody->applyImpulse(btVector3(imp.x,imp.y,imp.z), btVector3(0,0,0));
}

void RigidEntity::velocity(Ogre::Vector3 vel)
{
    m_rigidBody->setLinearVelocity(btVector3(vel.x,vel.y,vel.z));
    m_rigidBody->activate();
}

void RigidEntity::angularFactor(Ogre::Vector3 fac)
{
    m_rigidBody->setAngularFactor(btVector3(fac.x,fac.y,fac.z));
    m_rigidBody->activate();
}

Ogre::Vector3 RigidEntity::velocity()
{
    return Ogre::Vector3(m_rigidBody->getLinearVelocity().x(),m_rigidBody->getLinearVelocity().y(),m_rigidBody->getLinearVelocity().z());
}

Ogre::Vector3 RigidEntity::angularFactor()
{
    return Ogre::Vector3(m_rigidBody->getAngularFactor().x(),m_rigidBody->getAngularFactor().y(),m_rigidBody->getAngularFactor().z());
}

void RigidEntity::enableCollisionTest(bool en)
{
//    if (en)
//        m_frameListenerConnection = m_system->addFrameListener(boost::bind(&RigidEntity::updateCollision, this, _1));
//    else
//        m_frameListenerConnection.disconnect();
}

bool RigidEntity::collidating()
{
    return m_isCollidating;
}

bool RigidEntity::updateCollision(double time)
{
    (void)time;
    m_isCollidating = false;
    // m_system->_dynamicsWorld()->contactTest(m_rigidBody, *this);
    return true;
}

btRigidBody *RigidEntity::getRigidBody()
{
    return m_rigidBody;
}

