#ifndef MANUALOBJECT_H
#define MANUALOBJECT_H

#include "OGRE/OgreVector3.h"
#include "OGRE/OgreVector2.h"
#include "OGRE/OgreColourValue.h"
#include <vector>
#include "src/VoxelTerrain/triangle.h"
#include "OGRE/OgreAxisAlignedBox.h"
//#include "blub/graphic/graphic.hpp"
#include "OGRE/OgreMesh.h"
//#include "blub/graphic/scene.hpp"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreMesh.h>

#ifndef WIN32
#include <tr1/array>
#else
#include <array>
#endif

#include "Core/global.h"

#include "OGRE/OgreManualObject.h"

//typedef unsigned short uint16;
//typedef unsigned int uint32;

namespace voxel {
    class Triangle;
}

namespace Ogre{
    typedef boost::shared_ptr<std::vector<Ogre::Vector3> > Vector3Ptr;
}


namespace graphics {

class Material;

class ManualObject// : public btStridingMeshInterface
{
public:
    struct lod {
        lod()
            : triangles(0)
            , vertices(0)
//            , textures(0)
            , normals(0)
            , colours(0)
            , numVertices(0)
            , numTriangles(0)
        {
            ;
        }

        voxel::Triangle* triangles;
        Ogre::Vector3* vertices;
//        Ogre::Vector2* textures;
        Ogre::Vector3* normals;
        Ogre::ColourValue* colours;

        entropy::uint32 numVertices;
        entropy::uint32 numTriangles;
    };

    ManualObject(Ogre::SceneManager* sceneManager, entropy::uint16 numLod);
    ~ManualObject();

    void numVertices(entropy::uint32 num, entropy::uint16 _lod = 0);
    void numTriangles(entropy::uint32 num, entropy::uint16 _lod = 0);
    // vector<vector3> *position(entropy::uint16 lod = 0);
    void position(const Ogre::Vector3* pos, const entropy::uint32 num, entropy::uint16 _lod = 0);
    /*void position(const vector3 pos, entropy::uint16 lod = 0);*/
    void position(const std::vector<Ogre::Vector3> &pos, entropy::uint16 lod = 0);
    void position(Ogre::Vector3Ptr pos, entropy::uint16 lod = 0);
    void normal(Ogre::Vector3 *norms, entropy::uint32 num, entropy::uint16 _lod = 0);
    void normal(std::vector<Ogre::Vector3> normal, entropy::uint16 lod = 0);
    void normal(Ogre::Vector3Ptr normal, entropy::uint16 lod = 0);
    /*void normal(vector3 pos, entropy::uint16 lod = 0);
    void colour(colour col, entropy::uint16 lod = 0);
    void texture(vector2 tex, entropy::uint16 lod = 0);*/
    // vector<blub::triangle> *triangles(entropy::uint16 lod = 0);
    void triangle(voxel::Triangle*, entropy::uint32 num, entropy::uint16 _lod = 0);
    void triangle(const std::vector<voxel::Triangle> &tris, entropy::uint16 lod = 0);
    void triangle(const std::vector<entropy::uint32> &tris, entropy::uint16 lod = 0);
    void triangle(voxel::Triangle::vectorPtr tris, entropy::uint16 lod = 0);
//    void triplanarMap(Ogre::Vector3Ptr positions, Ogre::Vector3Ptr normals, int lod);
    lod *lods();

    void aabb(Ogre::AxisAlignedBox box);
    Ogre::AxisAlignedBox aabb(void) const {return m_aabb;}

    Ogre::Mesh* mesh();

    void deleteTriangles(entropy::uint16 id);
    void deleteAllTriangles();
    void deleteAllVertices();

    void deleteAllGeometry();

    void reInitGraphic(Material* mat,Ogre::Vector3Ptr vertices, Ogre::Vector3Ptr normals,voxel::Triangle::vectorPtr triangles);

    void staticVertexBuffer(bool set);
    // void updateVertices();
protected:

    Ogre::SceneManager *scene() {return sceneManager;}
    entropy::uint32 id() {return m_id;}

    Ogre::ManualObject ogreManualObject;

 private:
    void createSubmesh(Ogre::MeshPtr mesh, entropy::uint16 lod, std::string materialName);
    void createSharedVertexData(Ogre::MeshPtr mesh);
    void createVertexData(Ogre::VertexData* data, const lod *work);

    void initialiseMesh(Ogre::MeshPtr mesh);

    Ogre::SceneManager *sceneManager;

    std::tr1::array<lod,6> m_lods;

    static entropy::uint32 g_idCounter;
    entropy::uint32 m_id;

    Ogre::Mesh* m_mesh;
    Ogre::MeshPtr m_mainMesh;
    Ogre::MeshPtr m_lodMesh[2];

    Ogre::AxisAlignedBox m_aabb;

    entropy::uint16 m_numLod;

    bool m_staticVertexBuffer;
    bool alreadyLoaded;

};

}

#endif // MANUALOBJECT_H
