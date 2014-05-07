#ifndef WOLFRAMCA_H
#define WOLFRAMCA_H

#include "algorithm.h"

namespace Degenerator {

/////////////////
// WolframCAMesh
/////////////////

// Default arguments
// 1, 51, 51, 2, 1, 1

enum WolframCAArgs
{
    CARuleSet,
    CASize,
    CAIterations,
    CANumColors,
    CAInitialState,
    CAScale
};


class WolframCAMesh : public Algorithm
{
public:
    WolframCAMesh();

    AStruct generateMesh(std::vector<std::string> params);
};


///////////////////
// WolframCA3DMesh
///////////////////

// Some Different argument configurations
// 60999900, 31, 31, 31, 0, 2, 1, 1.5, false
// 70999952, 31, 31, 31, 1000, 2, 1, 1.5, false
// 60999901, 31, 31, 31, 0, 2, 1, 1.5, false
// 60999902, 31, 31, 31, 30, 2, 1, 1.5, true
// 60999924, 31, 31, 31, 10, 2, 1, 1.5, true
// 60999940, 31, 31, 31, 30, 2, 1, 1.5, true
// 70999950, 31, 31, 31, 0, 2, 1, 1.5, false
// 81999957, 31, 31, 31, 1000, 2, 1, 1.5, false

enum WolframCA3DArgs
{
    CA3DRuleSet,
    CA3DWidth,
    CA3DDepth,
    CA3DIterations,
    CA3DInitIterations,
    CA3DNumColors,
    CA3DInitialState,
    CA3DScale,
    CA3DRandInit
};


class WolframCA3DMesh : public Algorithm
{
public:
    WolframCA3DMesh();

    AStruct generateMesh(std::vector<std::string> params);
};

} // Degenerator namespace

#endif // WOLFRAMCA_H
