#ifndef STRANGEMESH_H
#define STRANGEMESH_H

#include "algorithm.h"
#include "meshfunctions.h"

namespace Degenerator {

/*
int iterationFrames = 100;
int initializationFrames = 100;
double scale = 1.0;
int initX = 1;
int initY = 1;
int initZ = 1;
double a = 2.24;
double b = 0.43;
double c = -0.65;
double d = -2.43;*/

// Default values
// 10000, 100, 1.0, 1, 1, 1, 1, 2.24, 0.43, -0.65, -2.43

enum StrangeArgs
{
    IterationFrames, // int
    InitializationFrames, // int
    Scale, // double
    Skip, // int
    InitX, // double
    InitY, // double
    InitZ, // double
    A, // double
    B, // double
    C, // double
    D // double
};

class StrangeMesh : public Algorithm
{
public:
    StrangeMesh();

    AStruct generateMesh(std::vector<std::string> params);
};

} // Degenerator namespace

#endif // STRANGEMESH_H
