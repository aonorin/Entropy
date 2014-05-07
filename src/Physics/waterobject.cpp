#include "waterobject.h"
#include "src/Physics/physics.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace physics {

///////////////
// WaterObject
///////////////

WaterObject::WaterObject(btGhostObject *object, collision_group_id_t collisionGroup, CursorType cursorType) :
    GhostObject(object, collisionGroup, cursorType)
{

}

WaterObject::WaterObject(const Vec3& origin, const Vec3& boxHalfExtents, const Quaternion& rotation, bool addToWorld, collision_group_id_t collisionGroup, CursorType cursorType) :
    GhostObject(0, collisionGroup, cursorType)
{
    object = physics::createCollisionBox(origin, boxHalfExtents, 0.0, rotation, Ghost, addToWorld, this, COL_SENSOR, sensorMask);
}

void WaterObject::objectCollisionStarted(btCollisionObject* object)
{
    if(object)
    {
        PhysicsObject* physicsObject = (PhysicsObject*) object->getUserPointer();

        if(physicsObject)
        {
            physicsObject->setPhysicsMode(WaterPhysics);

            if(physicsObject->type() == Rigid)
            {
                btRigidBody* body = btRigidBody::upcast(physicsObject->getObject());
                body->setLinearVelocity(body->getLinearVelocity() * 0.2);
            }
        }
    }
}

void WaterObject::objectCollisionEnded(btCollisionObject* object)
{
    if(object)
    {
        PhysicsObject* physicsObject = (PhysicsObject*) object->getUserPointer();

        if(physicsObject)
        {
            if(physicsObject->getPhysicsMode() == WaterPhysics)
                physicsObject->setPhysicsMode(StandardPhysics);
        }
    }
}

} // physics namespace
