#ifndef MESH_H
#define MESH_H


#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>

#include "Shape.h"
#include "Utility/Vector.h"
#include "OGRE/OgreVector3.h"
#include "VoxelTerrain/triangle.h"
#include "Core/global.h"
#include "OGRE/OgreAxisAlignedBox.h"

//typedef unsigned int uint32;

namespace Ogre
{
typedef boost::shared_ptr<std::vector<Ogre::Vector3> > Vector3Ptr;
}

namespace voxel
{
class rigidEntity;

class Mesh : public Shape
{
public:
    typedef boost::shared_ptr<Mesh> ptr;

    Mesh(Ogre::Vector3Ptr vertices, Triangle::vectorPtr triangles, Ogre::AxisAlignedBox partAAB);
    /*
    Mesh(const Ogre::Vector3 *vertices, Triangle *tris, uint32 numTriangles)
    {
        for (uint32 ind = 0; ind < numTriangles; ++ind)
        {
            m_mesh.addTriangle(vertices[tris[ind].ind[0]], vertices[tris[ind].ind[1]], vertices[tris[ind].ind[2]], false);
        }
        m_shape = new btBvhTriangleMeshShape(&m_mesh,false,true);
    }
    */

    ~Mesh()
    {
        std::cout << "Mesh::~Mesh()" << std::endl;
        if(m_shape)
            delete m_shape;
    }

protected:
    friend class RigidEntity;
    btCollisionShape* _shape() const {return m_shape;}

    btVector3 scalePoint(const btVector3& triangle, const Ogre::Vector3& origin,float scaleFactor);

private:
    btBvhTriangleMeshShape* m_shape;
    btTriangleMesh m_mesh;
};

}

#endif // MESH_H
