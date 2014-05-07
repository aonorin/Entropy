#ifndef ASTAR_H
#define ASTAR_H

#include "neuro/Global.h"
#include "boost/multi_array.hpp"
#include "vector"

namespace neuro {
enum PathStatus {Walkable=0,Unwalkable=1,Found,NonExistent=2,NotFinished=3,NotStarted=4,Empty=5,Taken=6};

typedef boost::multi_array<int, 2> WalkArray;
typedef boost::multi_array<int, 2> xyIntArray;
typedef boost::multi_array<int, 1> IntArray;
typedef boost::multi_array<float, 1> FloatArray;

class AStar {
public:
    AStar(int mapWidth,int mapHeight,int tileSize,int maxPeople, WalkArray walkability);
    ~AStar();
    Vec2i move(int id,int targetID,int tilesToUpdate);
    Vec2f move(int id,int targetID,int tilesToUpdate,float mul);
    int registerMover(Vec2i startingPos, int unitSpeed);
    void removeMover(int id);
    void setWalkabilityMap(WalkArray walkability);
    void setTileSize(int tileSize);
    PathStatus getPathStatus(int id);
    bool closedOnTarget(int id,int targetID);
    void setDiaganolMovement(bool diagonolMovement);
    void setPos(int id, Vec2i pos);
    Vec2i getPos(int id);

private:
    int mapWidth;
    int mapHeight;
    int tileSize;
    int maxPeople;
    int onClosedList;
    bool diagonolMovement;

    //Arrays
    WalkArray walkability;
    IntArray openList;
    xyIntArray whichList;
    IntArray openX;
    IntArray openY;
    xyIntArray parentX;
    xyIntArray parentY;
    IntArray fCost;
    xyIntArray gCost;
    IntArray hCost;
    IntArray pathLength;
    IntArray pathLocation;
    int** pathBank;
    IntArray pathStatus;
    IntArray xPath;
    IntArray yPath;
    IntArray xLoc;
    IntArray yLoc;
    IntArray speed;
    boost::multi_array<PathStatus, 1> ids;

    void initialize();
    void end();

    PathStatus findPath (int pathfinderID,int startingX, int startingY,int targetX, int targetY);
    void readPath(int pathfinderID,int currentX,int currentY,int pixelsPerFrame);
    int readPathX(int pathfinderID,int pathLocation);
    int readPathY(int pathfinderID,int pathLocation);
};

}
#endif // ASTAR_H
