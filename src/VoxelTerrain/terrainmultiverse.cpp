#include "terrainmultiverse.h"
#include "Core/Entropy.h"

using namespace voxel;

//You have to STORE THE EDITS1

TerrainMultiVerse::TerrainMultiVerse(uint16 macroXSize, uint16 macroYSize, uint16 macroZSize, uint16 xSize, uint16 ySize, uint16 zSize,float chunkSize) :
    macroXSize(macroXSize),
    macroYSize(macroYSize),
    macroZSize(macroZSize),
    xSize(xSize),
    ySize(ySize),
    zSize(zSize),
    chunkSize(chunkSize),
    terrainMultiVerse(boost::extents[macroXSize][macroYSize][macroZSize]),
    running(false)
{

    for(TerrainVerseArrayIndex y = 0;y<macroXSize;++y)
    {
        for(TerrainVerseArrayIndex x = 0;x<macroYSize;++x)
        {
            for(TerrainVerseArrayIndex z = 0;z<macroZSize;++z)
            {
                TerrainVersePtr tVerse(new TerrainVerse());
                tVerse->pos = Ogre::Vector3(x*xSize,y*ySize,z*zSize);
                tVerse->terrains = boost::shared_ptr<voxel::VoxelTerrain>(new voxel::VoxelTerrain(1,xSize*chunkSize,zSize*chunkSize,ySize*chunkSize));
                tVerse->graphicsz = boost::shared_ptr<voxel::GraphicVoxelTerrain>(new voxel::GraphicVoxelTerrain(tVerse->terrains.get(),Entropy::getSingletonPtr()->mSceneMgr,Entropy::getSingletonPtr()->mCameraMan));
                tVerse->physicsz = VoxelPhysics::ptr(new VoxelPhysics(tVerse->terrains.get()));
                terrainMultiVerse[x][y][z] = tVerse;
            }
        }
    }

    start();
}

void TerrainMultiVerse::update()
{
    Ogre::Vector3 playerPosition = physics::Vec3(Entropy::getSingletonPtr()->mCameraMan->getPosition()).toOgre();

    int direction = 0;
    if(playerPosition.x < terrainMultiVerse[1][1][1]->pos.x)
    {
        direction |= entropy::LEFT;
    }
    else if(playerPosition.x > terrainMultiVerse[1][1][1]->pos.x+xSize)
    {
        direction |= entropy::RIGHT;
    }

    if(playerPosition.y < terrainMultiVerse[1][1][1]->pos.y)
    {
        direction |= entropy::UP;
    }
    else if(playerPosition.y > terrainMultiVerse[1][1][1]->pos.y+ySize)
    {
        direction |= entropy::DOWN;
    }

    if(playerPosition.z < terrainMultiVerse[1][1][1]->pos.z)
    {
        direction |= entropy::BACK;
    }
    else if(playerPosition.z > terrainMultiVerse[1][1][1]->pos.z+zSize)
    {
        direction |= entropy::FORWARD;
    }
}

void TerrainMultiVerse::planeswalk(int direction)
{
    TerrainVersePtr temp0,temp1,temp2;
    if(direction&entropy::UP)
    {
        for(int x=0;x<3;++x)
        {
            for(int z=0;z<3;++z)
            {
                temp2 = terrainMultiVerse[x][2][z];
                temp1 = terrainMultiVerse[x][1][z];
                temp0 = terrainMultiVerse[x][0][z];

                terrainMultiVerse[x][1][z] = temp2;
                terrainMultiVerse[x][0][z] = temp1;

                //Construct
                terrainMultiVerse[x][2][z] = temp0;
                terrainMultiVerse[x][2][z]->newPos = temp2->pos;
                terrainMultiVerse[x][2][z]->newPos.y += ySize;
                constructTerrainVerse(terrainMultiVerse[x][2][z]);
            }
        }
    }
    else if(direction&entropy::DOWN)
    {
        for(int x=0;x<3;++x)
        {
            for(int z=0;z<3;++z)
            {
                temp2 = terrainMultiVerse[x][2][z];
                temp1 = terrainMultiVerse[x][1][z];
                temp0 = terrainMultiVerse[x][0][z];

                terrainMultiVerse[x][2][z] = temp1;
                terrainMultiVerse[x][1][z] = temp0;

                //Construct
                terrainMultiVerse[x][0][z] = temp2;
                terrainMultiVerse[x][0][z]->newPos = temp2->pos;
                terrainMultiVerse[x][0][z]->newPos.y -= ySize;
                constructTerrainVerse(terrainMultiVerse[x][0][z]);
            }
        }
    }

    if(direction&entropy::RIGHT)
    {
        for(int y=0;y<3;++y)
        {
            for(int z=0;z<3;++z)
            {
                temp2 = terrainMultiVerse[2][y][z];
                temp1 = terrainMultiVerse[1][y][z];
                temp0 = terrainMultiVerse[0][y][z];

                terrainMultiVerse[1][y][z] = temp2;
                terrainMultiVerse[0][y][z] = temp1;

                //Construct
                terrainMultiVerse[2][y][z] = temp0;
                terrainMultiVerse[2][y][z]->newPos = temp2->pos;
                terrainMultiVerse[2][y][z]->newPos.x += xSize;
                constructTerrainVerse(terrainMultiVerse[2][y][z]);
            }
        }
    }
    else if(direction&entropy::LEFT)
    {
        for(int y=0;y<3;++y)
        {
            for(int z=0;z<3;++z)
            {
                temp2 = terrainMultiVerse[2][y][z];
                temp1 = terrainMultiVerse[1][y][z];
                temp0 = terrainMultiVerse[0][y][z];

                terrainMultiVerse[2][y][z] = temp1;
                terrainMultiVerse[1][y][z] = temp0;

                //Construct
                terrainMultiVerse[0][y][z] = temp2;
                terrainMultiVerse[0][y][z]->newPos = temp2->pos;
                terrainMultiVerse[0][y][z]->newPos.z -= zSize;
                constructTerrainVerse(terrainMultiVerse[0][y][z]);
            }
        }
    }

    if(direction&entropy::BACK)
    {
        for(int x=0;x<3;++x)
        {
            for(int y=0;y<3;++y)
            {
                temp2 = terrainMultiVerse[x][y][2];
                temp1 = terrainMultiVerse[x][y][1];
                temp0 = terrainMultiVerse[x][y][0];

                terrainMultiVerse[x][y][2] = temp1;
                terrainMultiVerse[x][y][1] = temp0;

                //Construct
                terrainMultiVerse[x][y][0] = temp2;
                terrainMultiVerse[x][y][0]->newPos = temp2->pos;
                terrainMultiVerse[x][y][0]->newPos.z -= zSize;
                constructTerrainVerse(terrainMultiVerse[x][y][0]);
            }
        }
    } else if(direction&entropy::FORWARD)
    {
        for(int x=0;x<3;++x)
        {
            for(int y=0;y<3;++y)
            {
                temp2 = terrainMultiVerse[x][y][2];
                temp1 = terrainMultiVerse[x][y][1];
                temp0 = terrainMultiVerse[x][y][0];

                terrainMultiVerse[x][y][1] = temp2;
                terrainMultiVerse[x][y][0] = temp1;

                //Construct
                terrainMultiVerse[x][y][2] = temp0;
                terrainMultiVerse[x][y][2]->newPos = temp2->pos;
                terrainMultiVerse[x][y][2]->newPos.z += zSize;
                constructTerrainVerse(terrainMultiVerse[x][y][2]);
            }
        }
    }
}

void TerrainMultiVerse::constructTerrainVerse(TerrainVersePtr terrainVerse)
{
//    terrainVerse->algorithm->calculateVoxels();
    terrainVerse->terrains->edit(terrainVerse->algorithm->getBlobEdit(),true);
}

void TerrainMultiVerse::start()
{
    if(running) {
        stop();
    }
    updateThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&TerrainMultiVerse::run,this)));
    running = true;
}

void TerrainMultiVerse::stop()
{
    running = false;
    updateThread->join();
}

void TerrainMultiVerse::run()
{
    while(running) {
        update();
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> TerrainMultiVerse* Ogre::Singleton<TerrainMultiVerse>::ms_Singleton = 0;
TerrainMultiVerse* TerrainMultiVerse::getSingletonPtr(void)
{
    return ms_Singleton;
}
TerrainMultiVerse& TerrainMultiVerse::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> TerrainMultiVerse* Ogre::Singleton<TerrainMultiVerse>::msSingleton = 0;
TerrainMultiVerse* TerrainMultiVerse::getSingletonPtr(void)
{
    return msSingleton;
}
TerrainMultiVerse& TerrainMultiVerse::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
