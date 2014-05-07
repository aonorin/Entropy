#include <sstream>
#include "meshfunctions.h"

namespace Degenerator {

float cubeVertices[24] = {
    -0.5,  0.5, -0.5, // 0
     0.5,  0.5, -0.5, // 1
     0.5, -0.5, -0.5, // 2
    -0.5, -0.5, -0.5, // 3
    -0.5,  0.5,  0.5, // 4
     0.5,  0.5,  0.5, // 5
     0.5, -0.5,  0.5, // 6
    -0.5, -0.5,  0.5, // 7
};



int cubeIndices[36] = {
    0, 2, 3, // Face 0
    0, 1, 2, // 1
    1, 6, 2, // 2
    1, 5, 6, // 3
    4, 6, 5, // 4
    4, 7, 6, // 5
    0, 7, 4, // 6
    0, 3, 7, // 7
    0, 5, 1, // 8
    0, 4, 5, // 9
    2, 7, 3, // 10
    2, 6, 7  // 11
};

double stringToDouble(std::string &string)
{
    double value;
    std::stringstream ss;
    ss << std::setprecision(9);
    ss << string;
    ss >> value;
    return value;
}

void cube(Ogre::Vector3 position, double size, AStruct &astruct)
{
    int startingIndex = astruct.vertices.size();

    for(int i = 0; i < 24; i += 3)
    {
        astruct.vertices.push_back(
                    Ogre::Vector3(
                        cubeVertices[i] * size,
                        cubeVertices[i + 1] * size,
                        cubeVertices[i + 2] * size
                    ) + position
        );

        astruct.texCoords.push_back(Ogre::Vector2(cubeVertices[i], cubeVertices[i + 2]));
    }

    for(int i = 0; i < 36; ++i)
    {
        astruct.indices.push_back(cubeIndices[i] + startingIndex);
    }

    /*
    for(int i = 0; i < meshResult.size(); ++i)
    {
        meshResult[i] += position;
        texCoordResult.push_back(Ogre::Vector2(meshResult[i].x, meshResult[i].z));
    }*/

    for(int i = 0; i < 36; i += 9)
    {
        Ogre::Vector3 normal = calcNormal(
                    Ogre::Vector3(
                        cubeVertices[cubeIndices[i + 0]],
                        cubeVertices[cubeIndices[i + 1]],
                        cubeVertices[cubeIndices[i + 2]]
                    ) * size,
                    Ogre::Vector3(
                        cubeVertices[cubeIndices[i + 3]],
                        cubeVertices[cubeIndices[i + 4]],
                        cubeVertices[cubeIndices[i + 5]]
                    ) * size,
                    Ogre::Vector3(
                        cubeVertices[cubeIndices[i + 6]],
                        cubeVertices[cubeIndices[i + 7]],
                        cubeVertices[cubeIndices[i + 8]]
                    ) * size
        );

        astruct.normals.push_back(normal);
        astruct.normals.push_back(normal);
        astruct.normals.push_back(normal);
    }
}

Ogre::Vector3 calcNormal(Ogre::Vector3 v0, Ogre::Vector3 v1, Ogre::Vector3 v2)
{
    Ogre::Vector3 u, v;
    u = v1 - v0;
    v = v2 - v0;

    Ogre::Vector3 normal(
          (u.y * v.z) - (u.z * v.y),
          (u.z * v.x) - (u.x * v.z),
          (u.x * v.y) - (u.y * v.x)
    );

    return normal;
}

} // Degenerator namespace
