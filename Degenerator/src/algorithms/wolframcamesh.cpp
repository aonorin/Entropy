#include "wolframcamesh.h"
#include "Entropy/src/ArtificialLife/wolframca.h"
#include "meshfunctions.h"

using namespace alife::cell;

namespace Degenerator {

//////////////////
// WolframCAMesh
//////////////////

WolframCAMesh::WolframCAMesh()
{

}

AStruct WolframCAMesh::generateMesh(std::vector<std::string> params)
{
    AStruct astruct;

    int ruleSet = 1;
    int caSize = 50;
    int iterations = 50;
    int numColors = 2;
    int initialState = 1;
    float scale = 1;
    Ogre::Vector3 offset(0, 0, 0);

    for(int i = 0; i < params.size(); ++i)
    {
        switch(i)
        {
        case CARuleSet:
            ruleSet = stringToDouble(params[i]);
            break;

        case CASize:
            caSize = stringToDouble(params[i]);
            break;

        case CAIterations:
            iterations = stringToDouble(params[i]);
            break;

        case CANumColors:
            numColors = stringToDouble(params[i]);
            break;

        case CAInitialState:
            initialState = stringToDouble(params[i]);

        case CAScale:
            scale = stringToDouble(params[i]);
            break;
        }
    }

    offset.x -= (caSize * scale);
    offset.y -= (iterations * scale);
    offset.z -= ((numColors - 1) * scale);

    Automata automata;
    WolframCA ca(ruleSet, numColors, initialState, caSize);

    for(int i = 0; i < iterations; ++i)
    {
        automata.history.push_back(ca.getCurrentState());
        ca.advance();
    }


    for(int y = 0; y < automata.history.size(); ++y)
    {
        for(int x = 0; x < automata.history[y].size(); ++x)
        {
            unsigned int color = automata.history[y][x];

            if(color > 0)
                Degenerator::cube(Ogre::Vector3((x * scale * 2), y * scale * 2, color * scale * 2) + offset, scale, astruct);
        }
    }

    astruct.mode = TriangleList;
    return astruct;
}

///////////////////
// WolframCA3DMesh
///////////////////

WolframCA3DMesh::WolframCA3DMesh()
{

}

AStruct WolframCA3DMesh::generateMesh(std::vector<std::string> params)
{
    AStruct astruct;

    int ruleSet = 1;
    int width = 50;
    int depth = 50;
    int iterations = 50;
    int initIterations = 0;
    int numColors = 2;
    int initialState = 1;
    float scale = 1;
    bool randInit = false;

    Ogre::Vector3 offset(0, 0, 0);

    for(int i = 0; i < params.size(); ++i)
    {
        switch(i)
        {
        case CA3DRuleSet:
            ruleSet = stringToDouble(params[i]);
            break;

        case CA3DWidth:
            width = stringToDouble(params[i]);
            break;

        case CA3DDepth:
            depth = stringToDouble(params[i]);
            break;

        case CA3DIterations:
            iterations = stringToDouble(params[i]);
            break;

        case CA3DInitIterations:
            initIterations = stringToDouble(params[i]);
            break;

        case CA3DNumColors:
            numColors = stringToDouble(params[i]);
            break;

        case CA3DInitialState:
            initialState = stringToDouble(params[i]);
            break;

        case CA3DScale:
            scale = stringToDouble(params[i]);
            break;

        case CA3DRandInit:
            if(params[i].compare("true") == 0 || params[i].compare(" true") == 0)
                randInit = true;
            break;
        }
    }

    offset.x -= ((width * scale) / 2.0);
    offset.y -= ((iterations * scale) / 2.0);
    offset.z -= ((depth * scale) / 2.0);


    std::vector<slice_t> grid;
    WolframCA3D ca(ruleSet, numColors, initialState, width, depth);

    if(randInit)
        ca.randomizeCurrentState();

    for(int i = 0; i < initIterations; ++i)
    {
        ca.advance();
    }

    for(int i = 0; i < iterations; ++i)
    {
        grid.push_back(ca.getCurrentState());
        ca.advance();
    }

    for(int y = 0; y < grid.size(); ++y)
    {
        for(int z = 0; z < grid[y].size(); ++z)
        {
            for(int x = 0; x < grid[y][z].size(); ++x)
            {
                unsigned int color = grid[y][z][x];

                if(color > 0)
                {
                    Degenerator::cube(Ogre::Vector3(x * scale, y * scale, z * scale) + offset, scale, astruct);
                }
            }
        }
    }

    astruct.mode = TriangleList;
    return astruct;
}

} // Degenerator namespace
