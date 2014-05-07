#include "Entropy/src/ArtificialLife/Flocking.h"
#include "flockingmesh.h"
#include "Entropy/src/Utility/Math.h"
#include "greedysurface.h"

namespace Degenerator {

FlockingMesh::FlockingMesh()
{

}

AStruct FlockingMesh::generateMesh(std::vector<std::string> params)
{
    AStruct astruct;

    // Params
    geometry::Vec3i borderMin(0, 0, 0);
    geometry::Vec3i borderMax(50, 50, 50);
    bool borderWrapping = false;
    bool borderRepulsion = true;
    int neighborDist = 1;
    int numBoids = 3;
    int randomSeed = 0;
    int numRuns = 200;

    for(int i = 0; i < params.size(); ++i)
    {
        switch(i)
        {
        case BorderMinX:
            borderMin.x = atoi(params[i].c_str());
            break;
        case BorderMinY:
            borderMin.y = atoi(params[i].c_str());
            break;
        case BorderMinZ:
            borderMin.z = atoi(params[i].c_str());
            break;
        case BorderMaxX:
            borderMax.x = atoi(params[i].c_str());
            break;
        case BorderMaxY:
            borderMax.y = atoi(params[i].c_str());
            break;
        case BorderMaxZ:
            borderMax.z = atoi(params[i].c_str());
            break;
        case BorderWrapping:
            if(params[i].compare("true") == 0)
                borderWrapping = true;
            break;
        case BorderRepulsion:
            if(params[i].compare("false") == 0)
                borderWrapping = false;
            break;
        case NeighborDistance:
            neighborDist = atoi(params[i].c_str());
            break;
        case NumberOfBoids:
            numBoids = atoi(params[i].c_str());
            break;
        case RandomSeed:
            randomSeed = atoi(params[i].c_str());
            break;
        case NumberOfRuns:
            numRuns = atoi(params[i].c_str());
            break;
        }

    }

    srand(randomSeed);

    alife::Flock flock(
                        borderMin,
                        borderMax,
                        borderWrapping,
                        borderRepulsion
    );

    alife::Boid::neighborDist = neighborDist;
    alife::Boid::desiredSeparation = alife::Boid::neighborDist * 2;

    for(int i = 0; i < numBoids; ++i)
    {
        flock.addBoid(geometry::Vec3d(
                          utility::math::randomRange(borderMin.x, borderMax.x),
                          utility::math::randomRange(borderMin.y, borderMax.y),
                          utility::math::randomRange(borderMin.z, borderMax.z)
                      ),
                      geometry::Vec3d(0, 0, 0),
                      geometry::Vec3d(
                          utility::math::randomRange(-100, 100) / 100.0,
                          utility::math::randomRange(-100, 100) / 100.0,
                          utility::math::randomRange(-100, 100) / 100.0
                      ),
                      1,
                      0.05
        );
    }

    geometry::Vec3d seekPoint(
                ((borderMax.x - borderMin.x) / 2.0) + borderMin.x,
                ((borderMax.y - borderMin.y) / 2.0) + borderMin.y,
                ((borderMax.z - borderMin.z) / 2.0) + borderMin.z
    );

    // std::vector<Ogre::Vector3> pointCloud;

    for(int i = 0; i < numRuns; ++i)
    {
        flock.run();
        flock.seek(seekPoint);

        // Store the points
        for(int j = 0; j < flock.getNumberOfBoids(); ++j)
        {
            astruct.indices.push_back(astruct.indices.size());
            geometry::Vec3d velocity = flock.getBoids().at(j)->getVelocity();
            Ogre::Vector3 normal(velocity.x, velocity.y, velocity.z);
            normal.normalise();
            astruct.normals.push_back(normal);

            //geometry::Vec3d location = flock.getBoids().at(j)->getLocation();
            // Ogre::Vector3 vertex(location.x, location.y, location.z);
            //pointCloud.push_back(vertex);
        }
    }

    //greedySurface(pointCloud, astruct);
    astruct.mode = TriangleList;
    return astruct;
}

} // Degenerator namespace
