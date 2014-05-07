#ifndef FLOCKINGMESH_H
#define FLOCKINGMESH_H

#include "algorithm.h"

namespace Degenerator {

// Default args:
// 0, 0, 0, 50, 50, 50, false, true, 1, 3, 0, 200

enum FlockingMeshArgs
{
    BorderMinX, // int
    BorderMinY, // int
    BorderMinZ, // int
    BorderMaxX, // int
    BorderMaxY, // int
    BorderMaxZ, // int
    BorderWrapping, // bool
    BorderRepulsion, // bool
    NeighborDistance, // int
    NumberOfBoids, // int
    RandomSeed, // int
    NumberOfRuns // int
};

class FlockingMesh : public Algorithm
{
public:
    FlockingMesh();

    AStruct generateMesh(std::vector<std::string> params);
};

} // Degenerator

#endif // FLOCKINGMESH_H
