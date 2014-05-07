#ifndef ASTAR3D_H
#define ASTAR3D_H

#include "OGRE/OgreAxisAlignedBox.h"
#include "Core/global.h"
#include "boost/thread/locks.hpp"
#include "boost/multi_array.hpp"
#include "Graphics/OgreDebugDraw/DebugDrawer.h"
//#include "boost/unordered_map.hpp"
#ifndef WIN32
#include "tr1/unordered_map"
#else
#include <unordered_map>
#endif

namespace entropy
{

enum PathStatus {Walkable=0,Unwalkable=1,Found,NonExistent,NotFinished,NotStarted,Empty,Taken};

//typedef boost::multi_array<uint16, 3> xyzIntArray_t;
//typedef boost::multi_array<Ogre::Vector3, 3> xyzVector3Array_t;
//typedef boost::multi_array<uint16, 1> IntArray_t;
//typedef boost::multi_array<float, 1> FloatArray_t;
//typedef boost::multi_array<Ogre::Vector3,1> Vector3Array_t;

typedef std::tr1::unordered_map< uint16, std::tr1::unordered_map< uint16, std::tr1::unordered_map< uint16, uint16> > > xyzIntArray_t;
typedef std::tr1::unordered_map< uint16, std::tr1::unordered_map< uint16, std::tr1::unordered_map< uint16, Ogre::Vector3> > > xyzVector3Array_t;
typedef std::tr1::unordered_map<int,uint16> IntArray_t;
typedef std::tr1::unordered_map<int,float> floatArray_t;
typedef std::tr1::unordered_map< int, Ogre::Vector3> Vector3Array_t;

typedef std::map<int,int> WalkMap_t;
enum WalkType{Flier,Walker,Stalker,Free};

namespace utility
{


class AStar3D
{
public:
    AStar3D();
    AStar3D(uint16 width, uint16 height, uint16 depth, uint16 maxNumberMovers, float tileSize, FloatArrayPtr_t map);
    ~AStar3D();
    int registerMover(Ogre::Vector3 startingPos,WalkMap_t walkability,bool diagonolMovement = true, uint32 tilesToUpdate=5,uint32 movesTilUpdate=90, uint32 maxTiles=5000);
    void removeMover(uint32 id);
    PathStatus getPathStatus(uint32 id);
    bool isClosedOnTarget(uint32 id,uint32 targetID);
    void setPos(uint32 id, Ogre::Vector3 pos);
    Ogre::Vector3 getPos(uint32 id);

    Ogre::Vector3 nonThreadedMove(uint32 id, uint32 targetid, Ogre::Vector3 pos);
    Ogre::Vector3 move(uint32 id, uint32 targetid, Ogre::Vector3 pos);

    //void setMap(Int8ArrayPtr_t map);
    void setMap(entropy::FloatArrayPtr_t map);
    static WalkMap_t getWalkMap(int walkType);

    void debugDraw();

private:
    PathStatus findPath(uint32 id,uint32 targetid);
    void readPath(uint32 id);
    uint16 width;
    uint16 height;
    uint16 depth;
    uint16 tileWidth;
    uint16 tileHeight;
    uint16 tileDepth;
    uint16 maxNumberMovers;
    float tileSize;
    FloatArrayPtr_t map;
    int getWalkability(uint32 id,Ogre::Vector3 pos);
    int getWalkability(uint32 id,int x, int y, int z);
    int getWalkabilityAnyClear(uint32 id,Ogre::Vector3 pos);

    uint32 onClosedList;

    IntArray_t openList;
    xyzIntArray_t whichList;
    Vector3Array_t openPositions;
    xyzVector3Array_t parentPositions;
    IntArray_t fCost;
    xyzIntArray_t gCost;
    IntArray_t hCost;

    boost::shared_mutex mutex;
    typedef boost::upgrade_to_unique_lock<boost::shared_mutex> write_t;
    typedef boost::upgrade_lock<boost::shared_mutex> upgrade_write_t;
    typedef boost::shared_lock<boost::shared_mutex> read_t;

    std::vector<PathStatus> ids;
    struct Mover {
        Ogre::Vector3 pos;
        uint32 id;
        WalkMap_t walkability;
        PathStatus pathStatus;
        uint32 pathLocation;
        uint32 pathLength;
        Ogre::Vector3 pathPos;
        uint32 tilesToUpdate;
        uint32 movesTilUpdate;
        bool diagonolMovement;
        uint32 maxTiles;
        uint32 moves;
        uint32 tiles;
        std::vector<Ogre::Vector3> path;
        int targetid;
    };
    std::map<int,Mover> movers;
    void start();
    void stop();
    bool running;
    void run();
    boost::shared_ptr<boost::thread> thread;
};

}
}

#endif // ASTAR3D_H
