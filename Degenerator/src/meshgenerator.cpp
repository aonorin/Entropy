#include "meshgenerator.h"

namespace Degenerator {

MeshGenerator::MeshGenerator()
{

}

Ogre::MeshPtr MeshGenerator::createMesh(std::string meshName,std::string groupName, Ogre::SceneManager* sceneManager, std::vector<Ogre::Vector3> vertices,
                               std::vector<int> indices, std::vector<Ogre::Vector2> texCoords, std::vector<Ogre::Vector3> normals, std::string materialName, AStructPrimitive type)
{
    Ogre::ManualObject* manual = sceneManager->createManualObject(meshName);
    if(type == TriangleList) {
        manual->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST);
    } else if(type == TriangleStrip){
        manual->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
    }
    for(int i=0;i<vertices.size();i++) {
        manual->position(vertices.at(i));
        manual->textureCoord(texCoords.at(i));
        if(normals.size()>0) {
            manual->normal(normals.at(i));
        }
    }
    for(int i=0;i<indices.size();i++) {
        manual->index(indices.at(i));
    }
    manual->end();

    Ogre::MeshPtr mesh = manual->convertToMesh(meshName);
    return mesh;
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> MeshGenerator* Ogre::Singleton<MeshGenerator>::ms_Singleton = 0;
MeshGenerator* MeshGenerator::getSingletonPtr(void)
{
    return ms_Singleton;
}
MeshGenerator& MeshGenerator::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}

#else
template<> MeshGenerator* MeshGenerator::Singleton<MeshGenerator>::msSingleton = 0;
MeshGenerator* MeshGenerator::getSingletonPtr(void)
{
    return msSingleton;
}
MeshGenerator& MeshGenerator::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif

}

