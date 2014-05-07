#ifndef SPHERE_H
#define SPHERE_H

#ifndef WIN32
#include <bullet/BulletCollision/CollisionShapes/btSphereShape.h>
#else
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#endif

#include "Shape.h"

namespace voxel
{
    class RigidEntity;

class Sphere : public Shape
{
public:
    Sphere(double rad)
    {
        m_shape = new btSphereShape(rad);
    }

    ~Sphere()
    {
        delete m_shape;
    }

protected:
    friend class RigidEntity;
    btCollisionShape* _shape() const {return m_shape;}
private:
    btSphereShape *m_shape;
};
}

#endif // SPHERE_H
