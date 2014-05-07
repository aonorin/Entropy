#ifndef GAMEOFLIFEMESH_H
#define GAMEOFLIFEMESH_H

#include "algorithm.h"

namespace Degenerator {

///////////////////
// GameOfLifeMesh
///////////////////

// Default Values
// 50, 50, 50, 100, true, 1.5

enum GameOfLifeArgs
{
    GameWidth,
    GameDepth,
    GameIterations,
    GameInitialIterations,
    GameWrapping,
    GameScale
};

class GameOfLifeMesh : public Algorithm
{
public:
    GameOfLifeMesh();

    AStruct generateMesh(std::vector<std::string> params);
};


////////////////////
// GameOfLife3DMesh
////////////////////

// Default Values
// 50, 50, 50, random, 80, false, 1, 6, 11, 9, 11
// 30, 30, 30, random, 80, false, 1.25, 6, 11, 10, 11
// 30, 30, 30, random, 13, false, 1.25, 7, 14, 13, 14
// 30, 30, 30, stripe, 13, true, 1.25, 7, 12, 7, 8

enum GameOfLife3DArgs
{
    Game3DWidth,
    Game3DHeight,
    Game3DDepth,
    Game3DInitialFillType, // random, stripe
    Game3DInitialIterations,
    Game3DWrapping,
    Game3DScale,
    Game3DMinDeath,
    Game3DMaxDeath,
    Game3DMinBirth,
    Game3DMaxBirth
};

class GameOfLife3DMesh : public Algorithm
{
public:
    GameOfLife3DMesh();

    enum FillType
    {
        Random,
        Stripe
    };

    AStruct generateMesh(std::vector<std::string> params);
};

}

#endif // GAMEOFLIFEMESH_H
