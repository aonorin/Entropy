#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include <vector>

#include "Ogre.h"

namespace Degenerator {

enum AStructPrimitive
{
    TriangleList,
    TriangleStrip
};

struct AStruct {
    std::vector<Ogre::Vector3> vertices;
    std::vector<int> indices;
    std::vector<Ogre::Vector2> texCoords;
    std::vector<Ogre::Vector3> normals;
    AStructPrimitive mode;
};

class Algorithm
{
public:
    Algorithm();

    virtual AStruct generateMesh(std::vector<std::string> params);
};

}

#endif // ALGORITHM_H
