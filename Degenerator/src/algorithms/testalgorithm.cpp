#include "testalgorithm.h"

namespace Degenerator {

TestAlgorithm::TestAlgorithm()
{

}

AStruct TestAlgorithm::generateMesh(std::vector<std::string> params)
{
    std::cout << "Test Algorithm generating mesh" << std::endl;
    std::vector<Ogre::Vector3> vertices;
    std::vector<int> indices;
    std::vector<Ogre::Vector2> texCoords;

    //Vertices
    vertices.push_back(Ogre::Vector3(-20.0,20.0,-20.0));
    vertices.push_back(Ogre::Vector3(20.0,20.0,-20.0));
    vertices.push_back(Ogre::Vector3(20.0,-20.0,-20.0));
    vertices.push_back(Ogre::Vector3(-20.0,-20.0,-20.0));
    vertices.push_back(Ogre::Vector3(-20.0,20.0,20.0));
    vertices.push_back(Ogre::Vector3(20.0,20.0,20.0));
    vertices.push_back(Ogre::Vector3(20.0,-20.0,20.0));
    vertices.push_back(Ogre::Vector3(-20.0,-20.0,20.0));

    texCoords.push_back(Ogre::Vector2(0,1));
    texCoords.push_back(Ogre::Vector2(1,1));
    texCoords.push_back(Ogre::Vector2(1,0));
    texCoords.push_back(Ogre::Vector2(0,0));
    texCoords.push_back(Ogre::Vector2(1,0));
    texCoords.push_back(Ogre::Vector2(0,0));
    texCoords.push_back(Ogre::Vector2(0,1));
    texCoords.push_back(Ogre::Vector2(1,1));

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(1);
    indices.push_back(6);
    indices.push_back(2);

    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(6);

    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(5);

    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(6);

    indices.push_back(0);
    indices.push_back(7);
    indices.push_back(4);

    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(7);

    indices.push_back(0);
    indices.push_back(5);
    indices.push_back(1);

    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(5);

    indices.push_back(2);
    indices.push_back(7);
    indices.push_back(3);

    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(7);

    AStruct aStruct = {vertices,indices,texCoords};
    aStruct.mode = TriangleList;
    return aStruct;
}

}
