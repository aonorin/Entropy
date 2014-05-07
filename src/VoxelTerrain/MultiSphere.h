#ifndef MULTISPHERE_H
#define MULTISPHERE_H

#ifndef WIN32
#include <bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h>
#else
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#endif

#include "Shape.h"
#include "OgreVector3.h"
#include "sphere.h"
#include "Core/global.h"

//typedef unsigned short uint16;

namespace voxel
{
    class RigidEntity;

class MultiSphere : public Shape
{
public:
    MultiSphere(std::vector<Sphere> spheres)
    {
        btVector3 *positions = new btVector3[spheres.count()];
        btScalar *radius = new btScalar[spheres.count()];
        uint16 ind(0);
        for (std::vector<Sphere>::iterator it = spheres.begin(); it != spheres.end(); ++it)
        {
            positions[ind] = it->center();
            radius[ind++] = it->radius();
        }
        m_shape = new btMultiSphereShape(positions, radius, ind);
        delete positions;
        delete radius;
    }

    ~MultiSphere()
    {
        delete m_shape;
    }

protected:
    friend class RigidEntity;
    btCollisionShape* _shape() const {return m_shape;}
private:
    btMultiSphereShape *m_shape;
};

}
#endif // MULTISPHERE_H
