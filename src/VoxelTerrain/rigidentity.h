#ifndef RIGIDENTITY_H
#define RIGIDENTITY_H

#include "LinearMath/btMotionState.h"
#include "btBulletCollisionCommon.h"
#include "BulletCollision/NarrowPhaseCollision/btManifoldPoint.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "OGRE/OgreSphere.h"
#include "OGRE/OgreVector3.h"
#include "OGRE/OgreQuaternion.h"
#include "VoxelTerrain/triangle.h"

//#include "blub/physic/root.hpp"
#include "Shape.h"

#include "Graphics/manualobject.h"
#include "Utility/mutexlocker.h"
#include "boost/signals2.hpp"
#include "Mesh.h"
#include "src/Physics/physicstypes.h"
#include "src/Physics/physicsobject.h"


//#include <functional>
//#include <signal.h>

typedef boost::shared_ptr<btRigidBody> btRigidBodyPtr;

namespace voxel {

//class Shape;

class RigidEntity : public btMotionState, public btCollisionWorld::ContactResultCallback
{
public:
    typedef boost::shared_ptr<RigidEntity> ptr;
    RigidEntity();
    RigidEntity(const Mesh::ptr& shape, double mass);

     ~RigidEntity();

    Ogre::Vector3 position() const {
        return Ogre::Vector3(m_transform.getOrigin().x(),m_transform.getOrigin().y(),m_transform.getOrigin().z());
    }
    void position(const Ogre::Vector3 pos) {
        m_transform.setOrigin(btVector3(pos.x,pos.y,pos.z));
        m_rigidBody->setWorldTransform(m_transform);
    }
    Ogre::Quaternion rotation() const {
        return Ogre::Quaternion(m_transform.getRotation().w(),m_transform.getRotation().x(),m_transform.getRotation().y(),m_transform.getRotation().z());
    }
    void rotation(const Ogre::Quaternion rot) {
        m_transform.setRotation(btQuaternion(rot.x,rot.y,rot.z,rot.w));
        m_rigidBody->setWorldTransform(m_transform);
    }

    virtual void update();
    void applyForce(Ogre::Vector3 force);
    void applyImpulse(Ogre::Vector3 imp);
    void velocity(Ogre::Vector3 vel);
    void angularFactor(Ogre::Vector3 fac);

    Ogre::Vector3 velocity();
    Ogre::Vector3 angularFactor();

    void enableCollisionTest(bool en = true);
    bool collidating();


    typedef boost::signals2::signal<void (btVector3)> t_sigContactListener;

    /*
    boost::signals2::connection addContactListener(const t_sigContactListener::slot_type &subscriber)
    {
        return m_sigContact.connect(subscriber);
    }
    */


    /*
    typedef std::tr1::function<void (btVector3)> t_sigContactListener;
    void addContactListener(const t_sigContactListener::slot_type &subscriber)
    {
        m_sigContact.connect(subscriber);
    }
    */

    btRigidBody* getRigidBody();

    virtual void getWorldTransform(btTransform& worldTrans ) const;
    virtual void setWorldTransform(const btTransform& worldTrans);
//#ifndef __APPLE__
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1,int index1);
//#else
//    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1,int index1);
//#endif

protected:
    bool updateCollision(double time);

    friend class Rope;

private:
//    btRigidBodyPtr _rigidBody() {return m_rigidBody.get();}

    // graphic::manualObject m_manual;
//    Root *m_system;
    btTransform m_transform;
    btCollisionShape *m_collisionShape;
    btRigidBody* m_rigidBody;
    physics::RigidObject entropyPhysicsObject;

    bool m_isCollidating;

    t_sigContactListener m_sigContact;
    boost::signals2::connection m_frameListenerConnection;
};

}

#endif // RIGIDENTITY_H
