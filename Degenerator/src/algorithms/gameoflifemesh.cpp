#include "gameoflifemesh.h"
#include "Entropy/src/ArtificialLife/gameoflife.h"
#include "meshfunctions.h"

namespace Degenerator {

//////////////////
// GameOfLifeMesh
//////////////////

GameOfLifeMesh::GameOfLifeMesh()
{

}

AStruct GameOfLifeMesh::generateMesh(std::vector<std::string> params)
{
    AStruct astruct;

    int width = 50;
    int depth = 50;
    int iterations = 50;
    int initialIterations = 0;
    bool wrapping = false;
    double scale = 1.0;
    Ogre::Vector3 offset(0, 0, 0);

    for(int i = 0; i < params.size(); ++i)
    {
        switch(i)
        {
        case GameWidth:
            width = stringToDouble(params[i]);
            break;

        case GameDepth:
            depth = stringToDouble(params[i]);
            break;

        case GameIterations:
            iterations = stringToDouble(params[i]);
            break;

        case GameInitialIterations:
            initialIterations = stringToDouble(params[i]);
            break;

        case GameWrapping:
            if(params[i].compare("true") == 0 || params[i].compare(" true") == 0);
                wrapping = true;
            break;

        case GameScale:
            scale = stringToDouble(params[i]);
            break;
        }
    }

    offset.x -= ((width * scale) / 2.0);
    offset.y -= ((iterations * scale) / 2.0);
    offset.z -= ((depth * scale) / 2.0);

    alife::cell::GameOfLife gameOfLife(width, depth, wrapping);

    for(int x = 0; x < width; ++x)
    {
        for(int z = 0; z < depth; ++z)
        {
            int random = rand() % 2;
            gameOfLife.set(random, x, z);
            gameOfLife.setFuture(random, x, z);
        }
    }

    for(int i = 0; i < initialIterations; ++i)
    {
        gameOfLife.update();
    }

    for(int y = 0; y < iterations; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            for(int z = 0; z < depth; ++z)
            {
                if(gameOfLife.at(x, z))
                    cube(Ogre::Vector3(x * scale, y * scale, z * scale) + offset, scale, astruct);
            }
        }

        gameOfLife.update();
    }

    astruct.mode = TriangleList;
    return astruct;
}

////////////////////
// GameOfLife3DMesh
////////////////////

GameOfLife3DMesh::GameOfLife3DMesh()
{

}

AStruct GameOfLife3DMesh::generateMesh(std::vector<std::string> params)
{
    // 50, 50, 50, random, 50, false, 1, 6, 20, 6, 10
    //50, 50, 50, random, 80, false, 1, 6, 11, 9, 11
    int width = 50;
    int height = 50;
    int depth = 50;
    FillType fillType = Random;
    int initialIterations = 50;
    bool wrapping = false;
    double scale = 1.0;
    int minDeath = 6;
    int maxDeath = 9;
    int minBirth = 6;
    int maxBirth = 10;
    Ogre::Vector3 offset(0, 0, 0);

    for(int i = 0; i < params.size(); ++i)
    {
        switch(i)
        {
        case Game3DWidth:
            width = stringToDouble(params[i]);
            break;

        case Game3DHeight:
            height = stringToDouble(params[i]);
            break;

        case Game3DDepth:
            depth = stringToDouble(params[i]);
            break;

        case Game3DInitialFillType:
            if(params[i].compare("random") == 0 || params[i].compare(" random") == 0)
                fillType = Random;
            else if(params[i].compare("stripe") == 0 || params[i].compare(" stripe") == 0)
                fillType = Stripe;
            break;

        case Game3DInitialIterations:
            initialIterations = stringToDouble(params[i]);
            break;

        case Game3DWrapping:
            if(params[i].compare("true") == 0 || params[i].compare(" true") == 0)
                wrapping = true;
            break;

        case Game3DScale:
            scale = stringToDouble(params[i]);
            break;

        case Game3DMinDeath:
            minDeath = stringToDouble(params[i]);
            break;

        case Game3DMaxDeath:
            maxDeath = stringToDouble(params[i]);
            break;

        case Game3DMinBirth:
            minBirth = stringToDouble(params[i]);
            break;

        case Game3DMaxBirth:
            maxBirth = stringToDouble(params[i]);
            break;
        }
    }

    offset.x -= ((width * scale) / 2.0);
    offset.y -= ((height * scale) / 2.0);
    offset.z -= ((depth * scale) / 2.0);

    alife::cell::GameOfLifeRules rules(minDeath, maxDeath, minBirth, maxBirth);
    alife::cell::GameOfLife3D gameOfLife(width, height, depth, wrapping, rules);

    switch(fillType)
    {
    case Random:
        gameOfLife.randomize();
        break;

    case Stripe:
        gameOfLife.setOneEvery(2, 1);
        break;
    }

    for(int i = 0; i < initialIterations; ++i)
    {
        gameOfLife.update();
    }

    AStruct astruct;
    astruct.mode = TriangleList;

    for(int z = 0; z < depth; ++z)
    {
        for(int y = 0; y < height; ++y)
        {
            for(int x = 0; x < width; ++x)
            {
                if(gameOfLife.at(x, y, z))
                    cube(Ogre::Vector3(x * scale, y * scale, z * scale) + offset, scale, astruct);
            }
        }
    }

    return astruct;
}

} // Degenerator namespace
