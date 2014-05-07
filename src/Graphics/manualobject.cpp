#include "manualobject.h"

//#include "blub/core/log.hpp"
#include "material.h"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreSubMesh.h>
#include <OGRE/OgreCommon.h>
#include <OGRE/OgreHardwareBuffer.h>
#include <OGRE/OgreHardwareBufferManager.h>
#include "Core/Entropy.h"

namespace graphics {

entropy::uint32 ManualObject::g_idCounter = 0;

ManualObject::ManualObject(Ogre::SceneManager* sceneManager, entropy::uint16 numLod)
    :
      sceneManager(sceneManager),
      //    , m_begin(false)
      m_id(g_idCounter++),
      m_mesh(0),
      m_numLod(numLod),
      m_staticVertexBuffer(true),
      ogreManualObject(Ogre::String("omO").append(Ogre::StringConverter::toString((int)m_id))),
      alreadyLoaded(false)

{

}

ManualObject::~ManualObject()
{
    std::cout << "ManualObject::~ManualObject()" << std::endl;
    deleteAllGeometry();
    if (m_mesh)
        delete m_mesh;
    if (!m_mainMesh.isNull())
    {
        //m_mainMesh.
        //Ogre::MeshManager::getSingletonPtr()->remove(Ogre::String("mO").append(Ogre::StringConverter::toString(m_id)));
        //Entropy::getSingletonPtr()->addQueuedDeleteManualObject(Ogre::String("mO").append(Ogre::StringConverter::toString(m_id)));

    }
}

void ManualObject::initialiseMesh(Ogre::MeshPtr mesh)
{

}

Ogre::Mesh* ManualObject::mesh()
{
    return m_mesh;
}

void ManualObject::deleteTriangles(entropy::uint16 id)
{
    if (m_lods[id].triangles != 0)
        delete [] m_lods[id].triangles;
    m_lods[id].triangles=0;
}

void ManualObject::aabb(Ogre::AxisAlignedBox box)
{
    m_aabb = box;
    // setPremadeAabb(box.minimum(), box.maximum());
}

void ManualObject::position(const Ogre::Vector3 *pos, const entropy::uint32 num, entropy::uint16 lod)
{
    try{
        //        std::cout << num << std::endl;
        if(num > 0) {
            memcpy(m_lods[lod].vertices, pos, num*sizeof(Ogre::Vector3));
        } else {
            m_lods[lod].numVertices = 0;
            //            std::cout << "Post 0, vertices: " << m_lods[lod].numVertices << std::endl;
            for(int i=0;i<m_lods[lod].numVertices;i++) {
                m_lods[lod].vertices[i] = Ogre::Vector3(0,0,0);
            }
        }
    } catch(std::exception& e) {
        std::cerr << "ManualObject::position exception: " << e.what() << std::endl;
        for(int i=0;i<num; ++i) {
            m_lods[lod].vertices[i] = Ogre::Vector3(0,0,0);
        }
    } catch(...) {
        std::cerr << "ManualObject::position exception: " << std::endl;
        for(int i=0;i<num; ++i) {
            m_lods[lod].vertices[i] = Ogre::Vector3(0,0,0);
        }
    }
}

void ManualObject::position(Ogre::Vector3Ptr pos, entropy::uint16 lod)
{
    entropy::uint32 counter = 0;
    if(pos!=Ogre::Vector3Ptr()) {
        m_lods[lod].vertices = new Ogre::Vector3[pos->size()];
        std::copy(pos->begin(),pos->end(),m_lods[lod].vertices);
        m_lods[lod].numVertices = pos->size();
        //        std::cout << "ManualObject::position size: " << pos->size() << std::endl;
    } else {
        std::cout << "ManualObject::position size: " << 0 << std::endl;
        m_lods[lod].normals = new Ogre::Vector3[0];
        m_lods[lod].vertices = new Ogre::Vector3[0];
        m_lods[lod].numVertices = 0;
    }
}

void ManualObject::position(const std::vector<Ogre::Vector3> &pos, entropy::uint16 lod)
{
    entropy::uint32 counter = 0;
    for (std::vector<Ogre::Vector3>::const_iterator it = pos.begin(); it != pos.end(); ++it, ++counter)
    {
        m_lods[lod].vertices[counter] = *it;
    }
}

void ManualObject::normal(Ogre::Vector3 *norms, entropy::uint32 num, entropy::uint16 lod)
{
    try{
        if(num > 0) {
            memcpy(m_lods[lod].normals, norms, num*sizeof(Ogre::Vector3));
        } else {
            for(int i=0; i< m_lods[lod].numVertices;++i)
            {
                m_lods[lod].normals[i] = Ogre::Vector3(0,0,0);
            }
        }
    } catch(std::exception& e) {
        std::cerr << "ManualObject::normal exception: " << e.what() << std::endl;
        for(int i=0;i<num; ++i) {
            m_lods[lod].normals[i] = Ogre::Vector3(0,0,0);
        }
    } catch(...) {
        std::cerr << "ManualObject::normal exception: " << std::endl;
        for(int i=0;i<num; ++i) {
            m_lods[lod].normals[i] = Ogre::Vector3(0,0,0);
        }
    }
}

void ManualObject::normal(std::vector<Ogre::Vector3> normal, entropy::uint16 lod)
{
    entropy::uint32 counter = 0;
    for (std::vector<Ogre::Vector3>::const_iterator it = normal.begin(); it != normal.end(); ++it, ++counter)
    {
        m_lods[lod].normals[counter] = *it;
    }
}

void ManualObject::normal(Ogre::Vector3Ptr normal, entropy::uint16 lod)
{
    entropy::uint32 counter = 0;
    if(normal!=Ogre::Vector3Ptr()) {
        m_lods[lod].normals = new Ogre::Vector3[normal->size()];
        std::copy(normal->begin(),normal->end(),m_lods[lod].normals);
    } else {
        m_lods[lod].normals = new Ogre::Vector3[0];
        m_lods[lod].vertices = new Ogre::Vector3[0];
        m_lods[lod].numVertices = 0;
    }
}

void ManualObject::triangle(voxel::Triangle *tris, entropy::uint32 num, entropy::uint16 lod)
{
    try{
        //        std::cout << num << std::endl;
        if(num > 0)
        {
            memcpy(m_lods[lod].triangles, tris, num*sizeof(voxel::Triangle));
        } else {
            std::cout << "Post 0, vertices: " << m_lods[lod].numTriangles << std::endl;
            for(int i=0; i< m_lods[lod].numTriangles;++i)
            {
                m_lods[lod].triangles[i] = voxel::Triangle();
            }
        }
    } catch(std::exception& e) {
        std::cerr << "ManualObject::position exception: " << e.what() << std::endl;
        for(int i=0;i<num; ++i) {
            m_lods[lod].triangles[i] = voxel::Triangle();
        }
    } catch(...) {
        std::cerr << "ManualObject::position exception: " << std::endl;
        for(int i=0;i<num; ++i) {
            m_lods[lod].triangles[i] = voxel::Triangle();
        }
    }
}

void ManualObject::triangle(const std::vector<voxel::Triangle> &tris, entropy::uint16 lod)
{
    entropy::uint32 counter(0);
    for (std::vector<voxel::Triangle>::const_iterator it = tris.begin(); it != tris.end(); ++it, ++counter)
    {
        m_lods[lod].triangles[counter] = *it;
    }
}

void ManualObject::triangle(voxel::Triangle::vectorPtr tris, entropy::uint16 lod)
{
    entropy::uint32 counter(0);
    if(tris != voxel::Triangle::vectorPtr()) {
        m_lods[lod].triangles = new voxel::Triangle[tris->size()];
        std::copy(tris->begin(),tris->end(),m_lods[lod].triangles);
    }
}

void ManualObject::triangle(const std::vector<entropy::uint32> &tris, entropy::uint16 lod)
{
    entropy::uint32 counter(0);
    for (std::vector<entropy::uint32>::const_iterator it = tris.begin(); it < tris.end(); it+=3, ++counter)
    {
        m_lods[lod].triangles[counter].ind[0] = *(it+0);
        m_lods[lod].triangles[counter].ind[1] = *(it+1);
        m_lods[lod].triangles[counter].ind[2] = *(it+2);
    }
}

void ManualObject::deleteAllGeometry()
{
    deleteAllTriangles();
    deleteAllVertices();
}

void ManualObject::deleteAllTriangles()
{
    for (entropy::uint16 ind = 0; ind < m_numLod; ++ind)
    {
        deleteTriangles(ind);
    }
}

void ManualObject::deleteAllVertices()
{
    for (entropy::uint16 ind = 0; ind < m_numLod; ++ind)
    {
        if (m_lods[ind].vertices != 0)
        {
            delete [] m_lods[ind].vertices; m_lods[ind].vertices=0;
        }
        if (m_lods[ind].normals != 0)
        {
            delete [] m_lods[ind].normals; m_lods[ind].normals=0;
        }
    }
}

void ManualObject::staticVertexBuffer(bool set)
{
    m_staticVertexBuffer = set;
}

void ManualObject::createSubmesh(Ogre::MeshPtr mesh, entropy::uint16 _lod, std::string materialName)
{
    lod *work = &m_lods[_lod];
    if(!work->numVertices>0)
    {
        std::cerr << "ManualObject::createSubmesh Zero vertices in Manual Object, aborting" << std::endl;
        return;
    }
    if (!work->numTriangles>0)
    {
        std::cerr << "ManualObject::createSubmesh Zero triangles in Manual Object, aborting" << std::endl;
        return;
    }
    /*if (mesh->getNumSubMeshes() > 0)
        mesh->destroySubMesh(0);*/
    Ogre::SubMesh* sub = mesh->createSubMesh();
    sub->setBuildEdgesEnabled(false);
    sub->useSharedVertices = true;
    /*sub->vertexData = new Ogre::VertexData();
    createVertexData(sub->vertexData, work);*/

    // indexBuffer
    entropy::uint16 numIndices(work->numTriangles*3);
    Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().
            createIndexBuffer(
                Ogre::HardwareIndexBuffer::IT_32BIT,
                numIndices,
                Ogre::HardwareBuffer::HBU_STATIC);

    // Upload the index data to the card
    ibuf->writeData(0, ibuf->getSizeInBytes(), work->triangles, true);

    // Set parameters of the submesh
    sub->indexData->indexBuffer = ibuf;
    sub->indexData->indexCount = numIndices;
    sub->indexData->indexStart = 0;

    sub->setMaterialName(materialName); // "triPlanarMaterial1"
}

void ManualObject::createSharedVertexData(Ogre::MeshPtr mesh)
{
    (void) mesh;
}

void ManualObject::createVertexData(Ogre::VertexData *data, const lod *work)
{

}

void ManualObject::reInitGraphic(Material* mat,Ogre::Vector3Ptr vertices, Ogre::Vector3Ptr normals,voxel::Triangle::vectorPtr triangles)
{
    std::string materialName = mat->_material()->getName();

    ogreManualObject.setDynamic(true);
    if(!alreadyLoaded)
    {
        alreadyLoaded = true;
        ogreManualObject.begin(materialName);


        ogreManualObject.estimateVertexCount(vertices->size());
        ogreManualObject.estimateIndexCount(triangles->size()*3);

        for(int i=0;i<vertices->size(); ++i)
        {
            ogreManualObject.position(vertices->at(i));
            ogreManualObject.normal(normals->at(i));
            //ogreManualObject.textureCoord(0,0);
        }

        for(int i=0;i<triangles->size(); ++i)
        {
            ogreManualObject.triangle(triangles->at(i).ind[0],triangles->at(i).ind[1],triangles->at(i).ind[2]);
        }

        ogreManualObject.end();
        ogreManualObject.convertToMesh(Ogre::String("mO").append(Ogre::StringConverter::toString((int)m_id)));

    }
    else
    {
        ogreManualObject.beginUpdate(0);
        ogreManualObject.estimateVertexCount(vertices->size());
        ogreManualObject.estimateIndexCount(triangles->size()*3);

        for(int i=0;i<vertices->size(); ++i)
        {
            ogreManualObject.position(vertices->at(i));
            ogreManualObject.normal(normals->at(i));
            //ogreManualObject.textureCoord(0,0);
        }

        for(int i=0;i<triangles->size(); ++i)
        {
            ogreManualObject.triangle(triangles->at(i).ind[0],triangles->at(i).ind[1],triangles->at(i).ind[2]);
        }
        ogreManualObject.end();
        ogreManualObject.convertToMesh(Ogre::String("mO").append(Ogre::StringConverter::toString((int)m_id)));
    }
}

void ManualObject::numVertices(entropy::uint32 num, entropy::uint16 _lod)
{
    deleteAllVertices();
    m_lods[_lod].numVertices = num;
}

void ManualObject::numTriangles(entropy::uint32 num, entropy::uint16 _lod)
{
    //CHANGE ALL OF THESE TO BOOST ARRAYS OR VECTORS
    deleteTriangles(_lod);
    m_lods[_lod].numTriangles = num;
}


ManualObject::lod *ManualObject::lods()
{
    return m_lods.data();
}

}
