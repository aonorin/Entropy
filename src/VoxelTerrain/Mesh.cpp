#include "src/VoxelTerrain/Mesh.h"

namespace voxel
{

Mesh::Mesh(Ogre::Vector3Ptr vertices, Triangle::vectorPtr triangles, Ogre::AxisAlignedBox partAAB) :
    m_shape(0)
{
    float scaleFactor = 8;
    //        std::cout << "Mesh(Ogre::Vector3Ptr vertices, Triangle::vectorPtr triangles) begin" << std::endl;
    for (std::vector<Triangle>::const_iterator it = triangles->begin(); it != triangles->end(); ++it)
    {
        /*
        m_mesh.addTriangle(
                scalePoint(
                    btVector3(vertices->at(it->ind[0]).x,vertices->at(it->ind[0]).y,vertices->at(it->ind[0]).z),
                    partAAB.getMinimum(),
                    scaleFactor
                ),
                scalePoint(
                    btVector3(vertices->at(it->ind[1]).x,vertices->at(it->ind[1]).y,vertices->at(it->ind[1]).z),
                    partAAB.getMinimum(),
                    scaleFactor
                ),
                scalePoint(
                    btVector3(vertices->at(it->ind[2]).x,vertices->at(it->ind[2]).y,vertices->at(it->ind[2]).z),
                    partAAB.getMinimum(),
                    scaleFactor
                ),
                false
                );
        */
        m_mesh.addTriangle(
                btVector3(vertices->at(it->ind[0]).x*GRAPHICS_SCALE,vertices->at(it->ind[0]).y*GRAPHICS_SCALE,vertices->at(it->ind[0]).z*GRAPHICS_SCALE),
                btVector3(vertices->at(it->ind[1]).x*GRAPHICS_SCALE,vertices->at(it->ind[1]).y*GRAPHICS_SCALE,vertices->at(it->ind[1]).z*GRAPHICS_SCALE),
                btVector3(vertices->at(it->ind[2]).x*GRAPHICS_SCALE,vertices->at(it->ind[2]).y*GRAPHICS_SCALE,vertices->at(it->ind[2]).z*GRAPHICS_SCALE),
                false
        );
    }

    // m_mesh.setScaling(btVector3(scaleFactor, scaleFactor, scaleFactor));
    m_shape = new btBvhTriangleMeshShape(&m_mesh, true, true);
    // m_shape->setMargin(0.01);

    // m_shape->setLocalScaling(btVector3(12, 12, 12));
    //        std::cout << "Mesh(Ogre::Vector3Ptr vertices, Triangle::vectorPtr triangles) end" << std::endl;
}

btVector3 Mesh::scalePoint(const btVector3& triangle, const Ogre::Vector3& origin,float scaleFactor)
{
    float x = triangle.x() - origin.x;
    x = x * scaleFactor;
    float y = triangle.y() - origin.y;
    y = y * scaleFactor;
    float z = triangle. z() - origin.z;
    z = z * scaleFactor;
    btVector3 scaledPoint(x+origin.x,y+origin.y,z+origin.z);
    return scaledPoint;
}

} // voxel namespace
