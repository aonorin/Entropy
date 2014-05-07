#include <sstream>
#include "strangemesh.h"
#include "Entropy/src/Fractal/Strange.h"
#include "greedysurface.h"

namespace Degenerator {

StrangeMesh::StrangeMesh()
{

}

AStruct StrangeMesh::generateMesh(std::vector<std::string> params)
{
    AStruct astruct;

    int iterationFrames = 100;
    int initializationFrames = 100;
    double scale = 1.0;
    int skip;
    double initX = 1;
    double initY = 1;
    double initZ = 1;
    double a = 2.24;
    double b = 0.43;
    double c = -0.65;
    double d = -2.43;

    for(int i = 0; i < params.size(); ++i)
    {
        switch(i)
        {
        case IterationFrames:
            iterationFrames = stringToDouble(params.at(i));
            break;
        case InitializationFrames:
            initializationFrames = stringToDouble(params.at(i));
            break;
        case Scale:
            scale = stringToDouble(params.at(i));
            break;
        case Skip:
            skip = stringToDouble(params.at(i));
        case InitX:
            initX = stringToDouble(params.at(i));
            break;
        case InitY:
            initY = stringToDouble(params.at(i));
            break;
        case InitZ:
            initZ = stringToDouble(params.at(i));
            break;
        case A:
            a = stringToDouble(params.at(i));
            break;
        case B:
            b = stringToDouble(params.at(i));
            break;
        case C:
            c = stringToDouble(params.at(i));
            break;
        case D:
            d = stringToDouble(params.at(i));
            break;
        }
    }

    //fractal::pickover3DFill(array,iterationFrames, initializationFrames, scale, initX, initY, initZ, a, b, c, d);

    fractal::Pickover3D pickover(false, initializationFrames, initX, initY, initZ, a, b, c, d);

    geometry::Vec3d currentPoint, lastPoint;
    currentPoint = lastPoint = geometry::Vec3d(0, 0, 0);
    // std::vector<Ogre::Vector3> pointCloud;

    for(int i = 0; i < iterationFrames; ++i)
    {
        if(i % skip == 0)
            lastPoint = currentPoint;

        currentPoint = pickover.update();
        currentPoint *= scale;

        if(i % skip == 0)
        {
            //pointCloud.push_back(Ogre::Vector3(currentPoint.x, currentPoint.y, currentPoint.z));

            astruct.indices.push_back(astruct.indices.size());
            Ogre::Vector3 v1(currentPoint.x, currentPoint.y, currentPoint.z);
            Ogre::Vector3 v2(lastPoint.x, lastPoint.y, lastPoint.z);
            Ogre::Vector3 normal = v1.crossProduct(v2);
            astruct.normals.push_back(normal);
            astruct.vertices.push_back(v1);
            astruct.texCoords.push_back(Ogre::Vector2(v1.x, v2.z));
        }
    }

    // greedySurface(pointCloud, astruct);

    astruct.mode = TriangleList;
    return astruct;
}

} // Degenerator namespace
