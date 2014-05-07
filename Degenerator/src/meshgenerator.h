#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

#include "OgreSingleton.h"
#include "OgreSceneManager.h"
#include "OgreManualObject.h"
#include "algorithm.h"

#include "vector"

namespace Degenerator {

class MeshGenerator : public Ogre::Singleton<MeshGenerator>
{
public:
    enum MeshType {TriangleStrip,Height,Lines,Point};
    MeshGenerator();

    Ogre::MeshPtr createMesh(std::string meshName, std::string groupName, Ogre::SceneManager *sceneManager,
                    std::vector<Ogre::Vector3> vertices, std::vector<int> indices,std::vector<Ogre::Vector2> texCoords,std::vector<Ogre::Vector3> normals, std::string materialName, AStructPrimitive type);

    static MeshGenerator& getSingleton(void);
    static MeshGenerator* getSingletonPtr(void);
};

}

#endif // MESHGENERATOR_H
