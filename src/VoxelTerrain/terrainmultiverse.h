#ifndef MEGATERRAIN_H
#define MEGATERRAIN_H

#include "voxelterrain.h"
#include "graphicvoxelterrain.h"
#include "voxelphysics.h"
#include "boost/multi_array.hpp"
#include "OGRE/OgreSingleton.h"
#include "VoxelTerrain/edit.h"
#include "voxelalgorithm.h"
#include "Physics/physicsobject.h"
namespace voxel
{

class TerrainMultiVerse : public Ogre::Singleton<TerrainMultiVerse>
{
public:
    TerrainMultiVerse(uint16 macroXSize, uint16 macroYSize, uint16 macroZSize,uint16 xSize,uint16 ySize, uint16 zSize, float chunkSize);
    void update();

    static TerrainMultiVerse& getSingleton(void);
    static TerrainMultiVerse* getSingletonPtr(void);

private:

    uint16 macroXSize,macroYSize,macroZSize,xSize,ySize,zSize;
    float chunkSize;

    //Also include edits
    struct TerrainVerse
    {
        Ogre::Vector3 pos;
        boost::shared_ptr<VoxelTerrain> terrains;
        boost::shared_ptr<VoxelPhysics> physicsz;
        boost::shared_ptr<GraphicVoxelTerrain> graphicsz;
        Ogre::Vector3 newPos;
        std::string material;
        int seed;
        Algorithm::ptr algorithm;
        std::vector<Edit::ptr> edits;
        std::vector<std::string> physicsActors;
    };

    typedef boost::shared_ptr<TerrainVerse> TerrainVersePtr;
    typedef boost::multi_array<TerrainVersePtr,3> TerrainVerseArray;
    typedef TerrainVerseArray::index TerrainVerseArrayIndex;

    TerrainVerseArray terrainMultiVerse;

    void planeswalk(int direction);
    void constructTerrainVerse(TerrainVersePtr terrainVerse);

    boost::shared_ptr<boost::thread> updateThread;
    void start();
    void stop();
    void run();
    bool running;
};

}

#endif // MEGATERRAIN_H
