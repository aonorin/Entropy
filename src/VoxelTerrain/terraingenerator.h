#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "src/VoxelTerrain/voxelterrain.h"
#include "src/VoxelTerrain/graphicvoxelterrain.h"
#include "src/ArtificialLife/CellularAutomata.h"
#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"
#include "src/Utility/todolistener.h"
#include "src/ArtificialLife/diamondsquared.h"
#include "voxelphysics.h"
#include "blobedit.h"
#include "fresnelwater.h"
#include "terrainmultiverse.h"

namespace voxel {

class TerrainGenerator : public ToDoListener
{
public:
    TerrainGenerator(int width,int iterations,int depth,int scale = 0.25);
    ~TerrainGenerator();

    boost::shared_ptr<voxel::VoxelTerrain> terrain;
    boost::shared_ptr<voxel::GraphicVoxelTerrain> graphicTerrain;
    VoxelPhysics::ptr voxelPhysics;


    void saveTerrain();
    void loadTerrain();

    static void reset();
    void nonStaticReset();

    static void genGameOfLife();
    static void genGameOfLife3d(int initialIterations, int ratio, int minDeath, int maxDeath, int minBirth, int maxBirth);
    static void genWolfram(int ruleSet=1000,int numStates=2,int breakPoint = 0);
    static void genSimplex(float octaves, int featureFreq, float xMul, float yMul, float zMul, float offset);

    virtual void toDoGenSimplex(float octaves, int afeatureFreq, float xMul, float yMul, float zMul, float offset);
    virtual void toDogenGameOfLife();
    virtual void toDogenGameOfLife3d(int initialIterations, int ratio, int minDeath, int maxDeath, int minBirth, int maxBirth);
    virtual void toDogenWolfram(int ruleSet=1000,int numStates=2, int breakPoint=0);

    environment::FresnelWater::ptr getWater();
    void removeChunkxel(Ogre::Vector3 pos);
    void clear();

protected:
    int width,iterations,depth;
    int scale;
    boost::shared_ptr<boost::thread> thread;
    struct CellArea {
        Ogre::Vector3 pos;
        bool visited;
    };
    void createCaveEdit(BlobEdit::ptr blobEdit,int x, int y, int z);
    std::vector<CellArea> cells;
    environment::FresnelWater::ptr water;

//    TerrainMultiVerse multiVerse;
};


}

#endif // TERRAINGENERATOR_H
