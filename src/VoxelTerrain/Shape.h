#ifndef SHAPE_H
#define SHAPE_H

#include "boost/shared_ptr.hpp"

namespace voxel
{
    class RigidEntity;
    class MultiShape;

class Shape
{
public:
    boost::shared_ptr<Shape> ptr;
protected:
    friend class RigidEntity;
    friend class MultiShape;
    virtual btCollisionShape* _shape() const = 0;
};

}


#endif // SHAPE_H
