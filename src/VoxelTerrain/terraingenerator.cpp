#include "terraingenerator.h"
#include "VoxelTerrain/editsphere.h"
#include <time.h>

#include "ArtificialLife/wolframca.h"
#include "Core/Entropy.h"
#include "chunkedit.h"
#include "editaxisalignedbox.h"
#include "caveedit.h"
#include <time.h>
#include "megachunk.h"
#include "gameplay/actorupdateactions.h"
#include "ArtificialLife/simplexnoise1234.h"
#include "Utility/Math.h"
#include <cmath>

using namespace voxel;

TerrainGenerator::TerrainGenerator(int width, int iterations, int depth,int scale) :
    ToDoListener(1),
    width(width),
    iterations(iterations),
    depth(depth),
    scale(scale)
//    multiVerse(3,3,3,width*scale,iterations*scale,depth*scale,0.1)
{
    water = environment::FresnelWater::ptr(new environment::FresnelWater(Ogre::Vector3(0,iterations*(scale*0.1)*GRAPHICS_SCALE, 0)));
}

TerrainGenerator::~TerrainGenerator()
{
    std::cout << "TerrainGenerator::~TerrainGenerator()" << std::endl;
}

void TerrainGenerator::genSimplex(float octaves, int featureFreq, float xMul, float yMul, float zMul, float offset)
{
    Entropy::getSingletonPtr()->terrainGenerator->addToDo(boost::bind(&TerrainGenerator::toDoGenSimplex,Entropy::getSingletonPtr()->terrainGenerator,octaves,featureFreq,xMul,yMul,zMul,offset));
}

void TerrainGenerator::toDoGenSimplex(float octaves, int afeatureFreq, float xMul, float yMul, float zMul, float offset)
{
    srand ( time(NULL) );
    //BEST SETTINGS FOR PERLIN CAVES!
    //genSimplex(2,5,2,0.115,0.115,0.2)
    //FerroFluids Rivers with spikes and strange flowing?
    //Combine flow maps with Height Maps?
    terrain = boost::shared_ptr<voxel::VoxelTerrain>(new voxel::VoxelTerrain(1,width*(scale*0.15),depth*(scale*0.15),iterations*(scale*0.15)));
//    terrain = boost::shared_ptr<voxel::VoxelTerrain>(new voxel::VoxelTerrain(1,width*(scale*0.2),depth*(scale*0.2),iterations*(scale*0.2)));
    graphicTerrain = boost::shared_ptr<voxel::GraphicVoxelTerrain>(new voxel::GraphicVoxelTerrain(terrain.get(),Entropy::getSingletonPtr()->mSceneMgr,Entropy::getSingletonPtr()->mCameraMan));
    voxelPhysics = VoxelPhysics::ptr(new VoxelPhysics(terrain.get()));

    std::cout << "Calculating SimplexNoise float octaves: " <<octaves<<"  int afeatureFreq: "<<afeatureFreq<<" int xMul: "<<xMul<<" int yMul: "<<yMul<<" int zMul: "<<zMul<<" float offset: " <<offset << std::endl;
//    Ogre::Vector3 pos(width*scale* -0.5,iterations*scale* -0.5,depth*scale* -0.5);
    voxel::BlobEdit::ptr blobEdit(new BlobEdit(width*scale,iterations*scale,depth*scale));
    int count = 0;

    //Carve out Caves
    std::vector<float> simplexNoise;

    int xRand = rand() % 10000;
    int yRand = rand() % 10000;
    int zRand = rand() % 10000;
    //Create SimplexNoise
    for(int i = 1; i<octaves;++i)
    {
        //featureFreq = 15?
        int featureFreq = (int)((float)afeatureFreq/(float)i);
        for(int y = 0; y < ((float)(iterations*scale))/(float)featureFreq; ++y)
        {
            for(int x = 0; x < ((float)(width*scale))/(float)featureFreq; ++x)
            {
                for(int z = 0; z < ((float)(depth*scale))/(float)featureFreq; ++z)
                {
                    float value = blobEdit->calcVoxel(Ogre::Vector3(x,y,z)*featureFreq,1);
                    float tempValue = SimplexNoise1234::noise(
                                ((x*xMul)+xRand)*pow((float)2, i),
                                ((y*yMul)+yRand)*pow((float)2, i),
                                ((z*zMul)+zRand)*pow((float)2, i)
                                );
                    tempValue = tempValue/(1.0f/(float)i);
                    value += tempValue;
                    simplexNoise.push_back(value);
                }
            }
        }

        //Interpolate Features
        for(int y = 0; y < (iterations*scale)/featureFreq; ++y)
        {
            for(int x = 0; x < (width*scale)/featureFreq; ++x)
            {
                for(int z = 0; z < (depth*scale)/featureFreq; ++z)
                {

                    //Feature Cube
                    double features[4][4][4];
                    for(int fx=-2;fx<2;++fx)
                    {
                        for(int fy=-2;fy<2;++fy)
                        {
                            for(int fz=-2;fz<2;++fz)
                            {
                                int fxi,fyi,fzi;
                                fxi = std::min(std::max(0,fx+x),((width*scale)/featureFreq)-1);
                                fyi = std::min(std::max(0,fy+y),((iterations*scale)/featureFreq)-1);
                                fzi = std::min(std::max(0,fz+z),((depth*scale)/featureFreq)-1);
                                int oneDpos = fxi + ((width*scale)/featureFreq)  * (fyi + ((depth*scale)/featureFreq)  *fzi);
                                features[fx+2][fy+2][fz+2] = simplexNoise.at(oneDpos);
                            }
                        }
                    }
                    //Interpolation Cube
                    for(int xx=0;xx<featureFreq;++xx)
                    {
                        for(int yy=0;yy<featureFreq;++yy)
                        {
                            for(int zz=0;zz<featureFreq;++zz)
                            {
                                float xi = (float)xx/(float)featureFreq;
                                float yi = (float)yy/(float)featureFreq;
                                float zi = (float)zz/(float)featureFreq;
                                float value = utility::math::tricubicInterpolate(features,xi,yi,zi);
                                blobEdit->insertPoint(
                                            Ogre::Vector3(xx+(x*featureFreq),yy+(y*featureFreq),zz+(z*featureFreq)),
                                            //utility::math::easeOutCubic(value,0,2,1)
                                            value
                                            );
                            }
                        }
                    }
                    //Next Feature
                }
            }
        }
        simplexNoise.clear();
    }

    /*
    //RescaleBlobValues
    for(int x=0;x<(width*scale);++x)
    {
        for(int y=0;y<(iterations*scale);++y)
        {
            for(int z=0;z<(depth*scale);++z)
            {
                int oneDpos = x + ((width*scale))  * (y + ((depth*scale))  *z);
                float value = blobEdit->blobMap->at(oneDpos);
                value = value + offset;
                if(value>0.75)
                {
//                    value = (value*0.25)+ 0.75;
                }
                else if(value< - 0.75)
                {
//                    value = 0.75 - (value*0.25);
                }
                else
                {
//                    value *= 0.125;
                }
                blobEdit->insertPoint(
                            Ogre::Vector3(x,y,z),
                            value+offset
                            );
            }
        }
    }
    */
    /*
    //Bounds
    for(int x=0;x<(width*scale);++x)
    {
        for(int y=0;y<(iterations*scale);++y)
        {
            blobEdit->insertPoint(Ogre::Vector3(x,y,0),1);
            blobEdit->insertPoint(Ogre::Vector3(x,y,depth*scale),1);
        }
    }
    for(int x=0;x<(width*scale);++x)
    {
        for(int z=0;z<(depth*scale);++z)
        {
            blobEdit->insertPoint(Ogre::Vector3(x,0,z),1);
            blobEdit->insertPoint(Ogre::Vector3(x,iterations*scale,z),1);
        }
    }
    for(int z=0;z<(depth*scale);++z)
    {
        for(int y=0;y<(iterations*scale);++y)
        {
            blobEdit->insertPoint(Ogre::Vector3(0,y,z),1);
            blobEdit->insertPoint(Ogre::Vector3(width*scale,y,z),1);
        }
    }
    */

    std::cout << "Blob size: " << sizeof(blobEdit->blobMap) << std::endl;

    terrain->edit(blobEdit,true);
//    terrain->edit(blobEdit,false);
//    boost::shared_ptr<voxel::EditSphere> ed(new voxel::EditSphere(Ogre::Sphere(Ogre::Vector3(0,0,0),2),false));
//    terrain->edit(ed,true);

//    gameplay::astar.setMap(blobEdit->blobMap);
    std::cout << "All edits applied" << std::endl;
}

void TerrainGenerator::genGameOfLife3d(int initialIterations, int ratio, int minDeath, int maxDeath, int minBirth, int maxBirth)
{
    Entropy::getSingletonPtr()->terrainGenerator->addToDo(boost::bind(&TerrainGenerator::toDogenGameOfLife3d,Entropy::getSingletonPtr()->terrainGenerator,initialIterations,ratio,minDeath, maxDeath, minBirth, maxBirth));
}

void TerrainGenerator::toDogenGameOfLife3d(int initialIterations, int ratio,int minDeath,int maxDeath, int minBirth,int maxBirth)
{
    srand ( time(NULL) );
    terrain = boost::shared_ptr<voxel::VoxelTerrain>(new voxel::VoxelTerrain(1,width*(scale*0.1),depth*(scale*0.1),iterations*(scale*0.1)));
    graphicTerrain = boost::shared_ptr<voxel::GraphicVoxelTerrain>(new voxel::GraphicVoxelTerrain(terrain.get(),Entropy::getSingletonPtr()->mSceneMgr,Entropy::getSingletonPtr()->mCameraMan));
    voxelPhysics = VoxelPhysics::ptr(new VoxelPhysics(terrain.get()));

    //    diamondSquared->generate();

    alife::cell::GameOfLife3D gameOfLife(width,iterations,depth,false);
    std::cout << "Calculating Cellular Automata" << std::endl;

    gameOfLife.randomize();

    for(int i = 0; i < 50; ++i)
    {
        gameOfLife.update();
    }

    std::cout << "Creating Edits" << std::endl;
    voxel::BlobEdit::ptr blobEdit(new BlobEdit(width*scale,iterations*scale,depth*scale));
    int count = 0;

    //Carve out Caves
    for(int y = 0; y < iterations*scale; ++y)
    {
        for(int x = 0; x < width*scale; ++x)
        {
            for(int z = 0; z < depth*scale; ++z)
            {

                if((y%scale==0)&&(x%scale==0)&&(z%scale==0))
                {
                    if(gameOfLife.at(x/scale,y/scale,z/scale))
                    {
                        createCaveEdit(blobEdit,x,y,z);
                        CellArea cell = {Ogre::Vector3(x/scale,y/scale,z/scale),false};
                        cells.push_back(cell);
                        count++;
                    }
                }
            }
        }
    }
    createCaveEdit(blobEdit,20,20,20);
    createCaveEdit(blobEdit,18,20,20);
    createCaveEdit(blobEdit,22,20,20);
    createCaveEdit(blobEdit,20,20,18);
    createCaveEdit(blobEdit,20,20,20);
    createCaveEdit(blobEdit,20,20,22);
    createCaveEdit(blobEdit,20,18,20);
    createCaveEdit(blobEdit,20,22,20);

    //Using AStar algorithm make sure all caves are connected

    std::cout << "Total edit count: " << count << std::endl;
    std::cout << "Blob size: " << sizeof(blobEdit->blobMap) << std::endl;
    //    gameplay::astar.setMap(blobEdit->blobMap);

    terrain->edit(blobEdit,true);
    boost::shared_ptr<voxel::EditSphere> ed(new voxel::EditSphere(Ogre::Sphere(Ogre::Vector3(10,10,10),1),true));
    terrain->edit(ed,true);
    std::cout << "All edits applied" << std::endl;
}

void TerrainGenerator::createCaveEdit(BlobEdit::ptr blobEdit, int x, int y, int z)
{
    int random = (rand()%(int)((scale*2)));
    Ogre::Vector3 min = Ogre::Vector3(x,y,z)-random-1;
    Ogre::Vector3 max = Ogre::Vector3(x,y,z)+random+1;
    Ogre::Sphere sphere(Ogre::Vector3(x,y,z),(max.x - min.x)*0.5);

    for(int xx = min.x;xx<max.x;++xx)
    {
        for(int yy = min.y;yy<max.y;++yy)
        {
            for(int zz = min.z;zz<max.z;++zz)
            {
                //                                    if(yy<=y) {
                double distance = sphere.getRadius() - sphere.getCenter().distance(Ogre::Vector3(xx,yy,zz));
                if(distance>-1)
                {
                    if(
                            xx>0&&xx<(width*scale)-1&&
                            yy>0&&yy<(iterations*scale)-1&&
                            zz>0&&zz<(depth*scale)-1
                            )
                    {
                        blobEdit->insertPoint(Ogre::Vector3(xx,yy,zz),-1);

                    }
                }
            }
        }
    }
}

void TerrainGenerator::genGameOfLife()
{
    //    std::cout << "TerrainGenerator::genGameOfLife()"<<std::endl;
    Entropy::getSingletonPtr()->terrainGenerator->addToDo(boost::bind(&TerrainGenerator::toDogenGameOfLife,Entropy::getSingletonPtr()->terrainGenerator));
}

void TerrainGenerator::toDogenGameOfLife()
{
    srand ( time(NULL) );
    int initialIterations = (rand()%1000)+200;

    bool wrapping = true;
    //    terrain.reset();
    //    graphicTerrain.reset();
    terrain = boost::shared_ptr<VoxelTerrain>(new voxel::VoxelTerrain(2,width*0.5,depth*0.5,iterations*0.5));
    graphicTerrain = boost::shared_ptr<GraphicVoxelTerrain>(new voxel::GraphicVoxelTerrain(terrain.get(),Entropy::getSingletonPtr()->mSceneMgr,Entropy::getSingletonPtr()->mCameraMan));

    boost::shared_ptr<BlobEdit> blobEdit(new BlobEdit(width*2,iterations*2,depth*2));

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


    for(int y = 0; y < iterations*2; ++y)
    {
        for(int x = 0; x < width*2; ++x)
        {
            for(int z = 0; z < depth*2; ++z)
            {
                if(gameOfLife.at(x/2,z/2)) {
                    blobEdit->insertPoint(Ogre::Vector3(x,y,z),1);
                }
            }
        }
        if(y%2==0) {
            gameOfLife.update();
        }
    }
    terrain->edit(blobEdit,true);
}

void TerrainGenerator::genWolfram(int ruleSet,int numStates, int breakPoint)
{
    //    std::cout << "TerrainGenerator::genWolfram("<<ruleSet<<","<<numStates<<")"<<std::endl;
    Entropy::getSingletonPtr()->terrainGenerator->addToDo(boost::bind(&TerrainGenerator::toDogenWolfram,Entropy::getSingletonPtr()->terrainGenerator,ruleSet,numStates,breakPoint));
}

void TerrainGenerator::toDogenWolfram(int ruleSet,int numStates,int breakPoint)
{
    srand ( time(NULL) );
    int initialIterations = rand()%100;
    //    terrain.reset();
    //    graphicTerrain.reset();
    terrain = boost::shared_ptr<VoxelTerrain>(new voxel::VoxelTerrain(2,width*1,depth*1,iterations*1));
    graphicTerrain = boost::shared_ptr<GraphicVoxelTerrain>(new voxel::GraphicVoxelTerrain(terrain.get(),Entropy::getSingletonPtr()->mSceneMgr,Entropy::getSingletonPtr()->mCameraMan));

    boost::shared_ptr<BlobEdit> blobEdit(new BlobEdit(width*4,iterations*4,depth*4));

    alife::cell::WolframCA3D wa3d(ruleSet,numStates,0,width,depth);
    wa3d.randomizeCurrentState();
    for(int i = 0; i < initialIterations; ++i)
    {
        wa3d.advance();
    }
    for(int y = 0; y < iterations*4; ++y)
    {
        for(int x = 0; x < width*4; ++x)
        {
            for(int z = 0; z < depth*4; ++z)
            {
                if(wa3d.getCurrentState().at(x/4).at(z/2)>breakPoint) {
                    //                    blobEdit->insertPoint(Ogre::Vector3(x,y,z),1);
                }
            }
        }
        if(y%2==0) {
            wa3d.advance();
        }
    }
    //    terrain->edit(blobEdit,true);
}

void TerrainGenerator::reset()
{
    Entropy::getSingletonPtr()->terrainGenerator->nonStaticReset();
}

void TerrainGenerator::nonStaticReset()
{
    terrain.reset();
    graphicTerrain.reset();
}

void TerrainGenerator::removeChunkxel(Ogre::Vector3 pos)
{
    std::cout << "void TerrainGenerator::removeChunkxel(Ogre::Vector3 pos)" << std::endl;
    Ogre::Vector3 inFront = CollisionCamera::getSingletonPtr()->getCamera()->getDirection()*2;
    inFront = inFront + physics::Vec3(CollisionCamera::getSingletonPtr()->getPosition()).toOgre();
    Ogre::Vector3 editPos((float)inFront.x/(float)GRAPHICS_SCALE,(float)inFront.y/(float)GRAPHICS_SCALE,(float)inFront.z/(float)GRAPHICS_SCALE);
    boost::shared_ptr<voxel::EditSphere> ed(new voxel::EditSphere(Ogre::Sphere(editPos,4),true));
    terrain->edit(ed,true);
}

void TerrainGenerator::clear()
{
    boost::shared_ptr<BlobEdit> blobEdit(new BlobEdit(width*2,iterations*2,depth*2));
    terrain->edit(blobEdit,true);
}

environment::FresnelWater::ptr TerrainGenerator::getWater()
{
    return water;
}
