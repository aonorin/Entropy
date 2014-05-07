#ifndef MULTISHAPE_H
#define MULTISHAPE_H

#ifndef WIN32
#include <bullet/BulletCollision/CollisionShapes/btCompoundShape.h>
#else
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#endif

#include "Shape.h"
#include "OgreVector3.h"

namespace voxel
{
    class RigidEntity;
class MultiShape : public Shape
{
public:
    MultiShape()
    {
        m_shape = new btCompoundShape();
    }

    ~MultiShape()
    {
        delete m_shape;
    }

    void add (shape *sh, Ogre::Vector3 offset)
    {
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(offset);
        m_shape->addChildShape(trans, sh->_shape());
    }

protected:
    friend class RigidEntity;
    btCollisionShape* _shape() const {return m_shape;}
private:
    btCompoundShape *m_shape;
};

}
#endif // MULTISHAPE_H
