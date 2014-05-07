#include "astar3d.h"
#include "OGRE/OgreColourValue.h"

using namespace entropy;
using namespace utility;

AStar3D::AStar3D(uint16 width, uint16 height, uint16 depth, uint16 maxNumberMovers, float tileSize, FloatArrayPtr_t map) :
    width(width),
    height(height),
    depth(depth),
    maxNumberMovers(maxNumberMovers),
    tileSize(tileSize),
    tileWidth(width*tileSize),
    tileHeight(height*tileSize),
    tileDepth(depth*tileSize),
    map(map),
    ids(maxNumberMovers,Empty),
    onClosedList(10),
    running(false)
{
    start();
}

AStar3D::AStar3D()
{
    //This is uninitialized, will do nothing on its own;
}

AStar3D::~AStar3D()
{
    stop();
}

float distanceMultiplier(Ogre::Vector3 pos, Ogre::Vector3 targetPos)
{
    float distance = pos.squaredDistance(targetPos);
    if(distance <600)
    {
        return 0.1f;
    } else if(distance <5000)
    {
        return 0.25f;
    } else if(distance <30000)
    {
        return 0.5f;
    } else {
        return 1.0f;
    }
}

void AStar3D::run()
{
    while(running)
    {
        std::map<int,Mover>::const_iterator it;
        for(it = movers.begin();it!=movers.end();++it)
        {
            int id = (*it).second.id;
            int targetid = (*it).second.targetid;
            if(movers.find(targetid)!=movers.end())
            {
                //1. Find Path: If target and chaser are not at the same location on the
                //	screen and no path is currently active, find a new path.
                if(movers[id].pos!=movers[targetid].pos)
                {
                    movers[id].moves = movers[id].moves+1;
                    //If no path has been generated, generate one.
                    //Update it when the chaser reaches the tilesToUpdate limit, or the movesTilUpdate limit;
                    float distanceMul = distanceMultiplier(movers[id].pos,movers[targetid].pos);
                    if (
                            movers[id].pathStatus == NotStarted ||
                            movers[id].pathLocation > movers[id].tilesToUpdate ||
                            movers[id].moves > (movers[id].movesTilUpdate * distanceMul)
                            )
                    {
                        std::cout << "searching for new path. movetilUpdate: "<<movers[id].movesTilUpdate<<" moves: "<<movers[id].moves<<" pathtilesTilUpdate: "<<movers[id].tilesToUpdate<<" pathLocation: "<<movers[id].pathLocation << " distanceMul: "<<distanceMul << std::endl;
                        //Generate a new path.
                        movers[id].pathStatus = findPath(id,targetid);
                        movers[id].moves = 0;
                    }
                }

                //2.Move mover.
                if (movers[id].pathStatus == Found)
                {
                    readPath(id);
                } else
                {
                    movers[id].pathPos = movers[id].pos;
                }
            } else
            {
                movers[id].pathPos = movers[id].pos;
            }
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
}

void AStar3D::start()
{
    if(running)
        stop();
    running = true;
    thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&AStar3D::run,this)));
}

void AStar3D::stop()
{
    running = false;
    thread->join();
}

Ogre::Vector3 AStar3D::move(uint32 id, uint32 targetid, Ogre::Vector3 pos)
{
    if(movers.find(id)!=movers.end())
    {
        movers[id].pos = pos;
        movers[id].targetid = targetid;
        return movers[id].pathPos;
    } else
    {
        return pos;
    }
}

Ogre::Vector3 AStar3D::nonThreadedMove(uint32 id, uint32 targetid, Ogre::Vector3 pos)
{
    //    upgrade_write_t lock(mutex);
    //    write_t writeLock(lock);

    //Do we have that Id?
    if(movers.find(id)!=movers.end())
    {
        movers[id].pos = pos;
        movers[id].targetid = targetid;
        //Do we have the target id?
        if(movers.find(targetid)!=movers.end())
        {
            movers[id].moves = movers[id].moves+1;

            //1. Find Path: If target and chaser are not at the same location on the
            //	screen and no path is currently active, find a new path.
            if(movers[id].pos!=movers[targetid].pos)
            {
                //If no path has been generated, generate one.
                //Update it when the chaser reaches the tilesToUpdate limit, or the movesTilUpdate limit;
                if (movers[id].pathStatus == NotStarted || movers[id].pathLocation > movers[id].tilesToUpdate || movers[id].moves > movers[id].movesTilUpdate)
                {
                    std::cout << "searching for new path. Tiles checked: "<<movers[id].tiles<<", movetilUpdate: "<<movers[id].movesTilUpdate<<" moves: "<<movers[id].moves<<" pathtilesTilUpdate: "<<movers[id].tilesToUpdate<<" pathLocation: "<<movers[id].pathLocation << std::endl;
                    //Generate a new path.
                    movers[id].pathStatus = findPath(id,targetid);
                    movers[id].moves = 0;
                }
            }

            //2.Move mover.
            if (movers[id].pathStatus == Found)
            {
                readPath(id);
            }

            return movers[id].pathPos;
        } else
        {
            return pos;
        }
    } else
    {
        return pos;
    }

}

void AStar3D::readPath(uint32 id)
{
    //If a path has been found for the pathfinder	...
    if (movers[id].pathStatus == Found)
    {
        //If path finder is just starting a new path or has reached the
        //center of the current path square (and the end of the path
        //hasn't been reached), look up the next path square.
        if (movers[id].pathLocation < movers[id].path.size())
        {
            //if just starting or if close enough to center of square
            if (
                    movers[id].pathLocation == 0 ||
                    abs(movers[id].pos.x - movers[id].pathPos.x) < ((1.0f/tileSize)*0.25) &&
                    abs(movers[id].pos.y - movers[id].pathPos.y) < ((1.0f/tileSize)*0.25) &&
                    abs(movers[id].pos.z - movers[id].pathPos.z) < ((1.0f/tileSize)*0.25)
                    )
            {
                movers[id].pathLocation = movers[id].pathLocation + 1;
            }

            movers[id].pathPos = (movers[id].path.at(movers[id].pathLocation)/tileSize)+((1.0f/tileSize)*0.5);

            //If the center of the last path square on the path has been reached then reset.
            if (movers[id].pathLocation == movers[id].path.size()-1)
            {
                //if close enough to center of square
                if(
                        abs(movers[id].pos.x - movers[id].pathPos.x) < ((1.0f/tileSize)*0.25) &&
                        abs(movers[id].pos.y - movers[id].pathPos.y) < ((1.0f/tileSize)*0.25) &&
                        abs(movers[id].pos.z - movers[id].pathPos.z) < ((1.0f/tileSize)*0.25)
                        )
                {
                    std::cout << "Finished path!" << std::endl;
                    movers[id].pathStatus = NotStarted;
                }
            }
        } else
        {
            movers[id].pathPos = movers[id].pos;
        }
    } else
    {
        movers[id].pathPos = movers[id].pos;
    }
}

PathStatus AStar3D::findPath(uint32 id, uint32 targetid)
{
    //    std::cout << "0: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
    //============================================================================================================
    //0. Init local variables;
    int onOpenList=0;
    Ogre::Vector3 parentVals;
    int a=0, b=0, c=0,m=0, u=0, v=0;
    int temp=0;
    int corner=0;
    int numberOfOpenListItems=0;
    int addedgCost=0;
    int tempgCost = 0;
    Ogre::Vector3 tempPos;
    Ogre::Vector3 localPathPos;
    int cellPosition;
    int newOpenListItemid=0;
    Ogre::Vector3 currentPos;

    PathStatus path = NotStarted;

    //============================================================================================================
    //1. Convert location data (in pixels) to coordinates in the walkability array.
    //    std::cout << "1: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
    Ogre::Vector3 startingPos = movers[id].pos*tileSize;
    Ogre::Vector3 targetPos = movers[targetid].pos*tileSize;

    //============================================================================================================
    //2.Quick Path Checks: Under the some circumstances no path needs to be generated ...
    //    std::cout << "2: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
    if (startingPos.x == targetPos.x && startingPos.y == targetPos.y && startingPos.z == targetPos.z && movers[id].pathLocation > 0)
        return Found;
    if (startingPos.x == targetPos.x && startingPos.y == targetPos.y && startingPos.z == targetPos.z && movers[id].pathLocation == 0)
        return NonExistent;
    if(startingPos.x < 0 || startingPos.y < 0 || startingPos.z < 0)
        return NonExistent;
    if(targetPos.x < 0 || targetPos.y < 0 || targetPos.z < 0)
        return NonExistent;
    if(startingPos.x >= tileWidth || startingPos.y >= tileHeight || startingPos.z >= tileDepth)
        return NonExistent;
    if(targetPos.x >= tileWidth || targetPos.y >= tileHeight || targetPos.z >= tileDepth)
        return NonExistent;

    //	If target square is Unwalkable, return that it's a nonexistent path.
    if (getWalkabilityAnyClear(id,startingPos) == Unwalkable || getWalkabilityAnyClear(id,targetPos) == Unwalkable)
    {
        goto noPath;
    }

    //============================================================================================================
    //3.Reset some variables that need to be cleared
    //    std::cout << "3: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
    //    if (onClosedList > 1000000) //reset whichList occasionally
    //    {
    //        for (int x = 0; x < tileWidth;x++) {
    //            for (int y = 0; y < tileHeight;y++)
    //                for(int z = 0; z < tileDepth; ++z)
    //                    whichList[x][y][z] = 0;
    //        }
    //ADDED RESET, THIS ISN'T IN ORIGINAL
    openList.clear();
    whichList.clear();
    openPositions.clear();
    parentPositions.clear();
    fCost.clear();
    gCost.clear();
    hCost.clear();
    onClosedList = 10;
    //    }
    onClosedList = onClosedList+2; //changing the values of onOpenList and onClosed list is faster than redimming whichList() array
    onOpenList = onClosedList-1;
    movers[id].pathLength = 0;
    movers[id].pathLocation = 0;
    gCost[startingPos.x][startingPos.y][startingPos.z] = 0; //reset starting square's G value to 0
    movers[id].tiles = 0;

    //============================================================================================================
    //4.Add the starting location to the open list of squares to be checked.
    //    std::cout << "4: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
    numberOfOpenListItems = 1;
    openList[1] = 1;//assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
    openPositions[1] = startingPos;


    //============================================================================================================
    //5.Do the following until a path is found or deemed nonexistent.
    //    std::cout << "5: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
    do
    {
        movers[id].tiles = movers[id].tiles+1;
        //        std::cout << movers[id].tiles << std::endl;
        //============================================================================================================
        //6.If the open list is not empty, take the first cell off of the list.
        //	This is the lowest F cost cell on the open list.
        //        std::cout << "6: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
        if (numberOfOpenListItems != 0)
        {
            //============================================================================================================
            //7. Pop the first item off the open list.
            //            std::cout << "7: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
            parentVals = openPositions[openList[1]]; //record cell coordinates of the item
            whichList[parentVals.x][parentVals.y][parentVals.z] = onClosedList;//add the item to the closed list

            //	Open List = Binary Heap: Delete this item from the open list, which
            //  is maintained as a binary heap. For more information on binary heaps, see:
            //	http://www.policyalmanac.org/games/binaryHeaps.htm
            numberOfOpenListItems = numberOfOpenListItems - 1;//reduce number of open list items by 1

            //	Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top.
            openList[1] = openList[numberOfOpenListItems+1];//move the last item in the heap up to slot #1
            v = 1;

            //	Repeat the following until the new item in slot #1 sinks to its proper spot in the heap.
            do
            {
                u = v;
                if (2*u+1 <= numberOfOpenListItems) //if both children exist
                {
                    //Check if the F cost of the parent is greater than each child.
                    //Select the lowest of the two children.
                    if (fCost[openList[u]] >= fCost[openList[2*u]])
                        v = 2*u;
                    if (fCost[openList[v]] >= fCost[openList[2*u+1]])
                        v = 2*u+1;
                }
                else
                {
                    if (2*u <= numberOfOpenListItems) //if only child #1 exists
                    {
                        //Check if the F cost of the parent is greater than child #1
                        if (fCost[openList[u]] >= fCost[openList[2*u]])
                            v = 2*u;
                    }
                }

                if (u != v) //if parent's F is > one of its children, swap them
                {
                    temp = openList[u];
                    openList[u] = openList[v];
                    openList[v] = temp;
                }
                else
                    break; //otherwise, exit loop

            } while (true);//    while (!KeyDown(27));//reorder the binary heap

            //============================================================================================================
            //7.Check the adjacent squares. (Its "children" -- these path children
            //	are similar, conceptually, to the binary heap children mentioned
            //	above, but don't confuse them. They are different. Path children
            //	are portrayed in Demo 1 with grey pointers pointing toward
            //	their parents.) Add these adjacent child squares to the open list
            //	for later consideration if appropriate (see various if statements
            //	below).
            //            std::cout << "7-2: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
            for (b = parentVals.y-1; b <= parentVals.y+1; b++)
            {
                for (a = parentVals.x-1; a <= parentVals.x+1; a++)
                {
                    for (c = parentVals.z-1; c <= parentVals.z+1; c++)
                    {
                        //Check for diagonols. If not enabled, ignore
                        if(
                                (
                                    //Four corners on the same plane
                                    ((c==parentVals.z-1&&a==parentVals.x-1||
                                      c==parentVals.z+1&&a==parentVals.x+1||
                                      c==parentVals.z-1&&a==parentVals.x-1||
                                      c==parentVals.z+1&&a==parentVals.x+1)||
                                     //If it's on a different plane, it must be exactly in the middle
                                     ((c!=parentVals.z||a!=parentVals.x&&(b!=parentVals.y))))
                                    &&
                                    !movers[id].diagonolMovement)
                                )
                        {
                            continue;
                        }
                        //	If not off the map (do this first to avoid array out-of-bounds errors)
                        if (
                                a != -1 && b != -1 && c!=-1 &&
                                a != tileWidth && b != tileHeight && c != tileDepth
                                )
                        {

                            currentPos.x = a;
                            currentPos.y = b;
                            currentPos.z = c;
                            //	If not already on the closed list (items on the closed list have
                            //	already been considered and can now be ignored).
                            if (whichList[a][b][c] != onClosedList) {
                                bool walkCheck = false;
                                if(currentPos==targetPos)
                                {
                                    walkCheck = (getWalkabilityAnyClear(id,currentPos) != Unwalkable);
                                } else
                                {
                                    walkCheck = (getWalkability(id,currentPos) != Unwalkable);
                                }

                                //	If not a wall/obstacle square.
                                if (walkCheck) {

                                    //Don't cut across corners
                                    //First we check for standard corner cutting on the x-plane
                                    corner = Walkable;
                                    if (a == parentVals.x-1)
                                    {
                                        if (c == parentVals.z-1)
                                        {
                                            if (getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z) == Unwalkable
                                                    || getWalkability(id,parentVals.x,parentVals.y,parentVals.z-1) == Unwalkable) \
                                                corner = Unwalkable;
                                        }
                                        else if (c == parentVals.z+1)
                                        {
                                            if (getWalkability(id,parentVals.x,parentVals.y,parentVals.z+1) == Unwalkable
                                                    || getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z) == Unwalkable)
                                                corner = Unwalkable;
                                        }
                                    }
                                    else if (a == parentVals.x+1)
                                    {
                                        if (c == parentVals.z-1)
                                        {
                                            if (getWalkability(id,parentVals.x,parentVals.y,parentVals.z-1) == Unwalkable
                                                    || getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z) == Unwalkable)
                                                corner = Unwalkable;
                                        }
                                        else if (c == parentVals.z+1)
                                        {
                                            if (getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z) == Unwalkable
                                                    || getWalkability(id,parentVals.x,parentVals.y,parentVals.z+1) == Unwalkable)
                                                corner = Unwalkable;
                                        }
                                    }
                                    //Now we check for standard corner cutting on the y-plane
                                    else if(b == parentVals.y-1)
                                    {
                                        if (a == parentVals.x-1)
                                        {
                                            if (getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z) == Unwalkable
                                                    || getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z) == Unwalkable) \
                                                corner = Unwalkable;
                                        }
                                        else if (a == parentVals.x+1)
                                        {
                                            if (getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z) == Unwalkable
                                                    || getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z) == Unwalkable)
                                                corner = Unwalkable;
                                        }
                                    }
                                    else if(b == parentVals.y+1)
                                    {
                                        if (a == parentVals.x-1)
                                        {
                                            if (getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z) == Unwalkable
                                                    || getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z) == Unwalkable)
                                                corner = Unwalkable;
                                        }
                                        else if (a == parentVals.x+1)
                                        {
                                            if (getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z) == Unwalkable
                                                    || getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z) == Unwalkable)
                                                corner = Unwalkable;
                                        }
                                    }
                                    //Now we check for standard corner cutting on the z-plane
                                    else if(c == parentVals.z-1)
                                    {
                                        if (b == parentVals.y-1)
                                        {
                                            if (getWalkability(id,parentVals.x,parentVals.y,parentVals.z-1) == Unwalkable
                                                    || getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z) == Unwalkable) \
                                                corner = Unwalkable;
                                        }
                                        else if (b == parentVals.y+1)
                                        {
                                            if (getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z) == Unwalkable
                                                    || getWalkability(id,parentVals.x,parentVals.y,parentVals.z-1) == Unwalkable)
                                                corner = Unwalkable;
                                        }
                                    }
                                    else if(c == parentVals.z+1)
                                    {
                                        if (b == parentVals.y-1)
                                        {
                                            if (getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z) == Unwalkable
                                                    || getWalkability(id,parentVals.x,parentVals.y,parentVals.z+1) == Unwalkable)
                                                corner = Unwalkable;
                                        }
                                        else if (b == parentVals.y+1)
                                        {
                                            if (getWalkability(id,parentVals.x,parentVals.y,parentVals.z-1) == Unwalkable
                                                    || getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z) == Unwalkable)
                                                corner = Unwalkable;
                                        }
                                    }
                                    //Now we check for corner cutting when changing all 3 planes
                                    //NEW FOR THREE DIMENSIONS, YEAH!
                                    //Should be 8 cases...
                                    //Case 1 (-1 -1 -1)
                                    else if(a == parentVals.x-1 && b == parentVals.y-1 && c == parentVals.z-1)
                                    {
                                        if(
                                                getWalkability(id,parentVals.x-1,parentVals.y-1,parentVals.z) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y,parentVals.z-1) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z-1) == Unwalkable &&
                                                getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z-1) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                    }
                                    //Case 2 (-1 -1 +1)
                                    else if(a == parentVals.x-1 && b == parentVals.y-1 && c == parentVals.z+1)
                                    {
                                        if(
                                                getWalkability(id,parentVals.x-1,parentVals.y-1,parentVals.z) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y,parentVals.z+1) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z+1) == Unwalkable &&
                                                getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z+1) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                    }
                                    //Case 3 (-1 +1 +1)
                                    else if(a == parentVals.x-1 && b == parentVals.y+1 && c == parentVals.z+1)
                                    {
                                        if(
                                                getWalkability(id,parentVals.x-1,parentVals.y+1,parentVals.z) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y,parentVals.z+1) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z+1) == Unwalkable &&
                                                getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z+1) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                    }
                                    //Case 4 (-1 +1 -1)
                                    else if(a == parentVals.x-1 && b == parentVals.y+1 && c == parentVals.z-1)
                                    {
                                        if(
                                                getWalkability(id,parentVals.x-1,parentVals.y+1,parentVals.z) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y,parentVals.z-1) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z-1) == Unwalkable &&
                                                getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x-1,parentVals.y,parentVals.z-1) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                    }
                                    //Case 5 (+1 -1 -1)
                                    else if(a == parentVals.x+1 && b == parentVals.y-1 && c == parentVals.z-1)
                                    {
                                        if(
                                                getWalkability(id,parentVals.x+1,parentVals.y-1,parentVals.z) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y,parentVals.z-1) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z-1) == Unwalkable &&
                                                getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z-1) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                    }
                                    //Case 6 (+1 -1 +1)
                                    else if(a == parentVals.x+11 && b == parentVals.y-1 && c == parentVals.z+1)
                                    {
                                        if(
                                                getWalkability(id,parentVals.x+1,parentVals.y-1,parentVals.z) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y,parentVals.z+1) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z+1) == Unwalkable &&
                                                getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z+1) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y-1,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                    }
                                    //Case 7 (+1 +1 +1)
                                    else if(a == parentVals.x+1 && b == parentVals.y+1 && c == parentVals.z+1)
                                    {
                                        if(
                                                getWalkability(id,parentVals.x+1,parentVals.y+1,parentVals.z) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y,parentVals.z+1) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z+1) == Unwalkable &&
                                                getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z+1) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                    }
                                    //Case 7 (+1 +1 -1)
                                    else if(a == parentVals.x+1 && b == parentVals.y+1 && c == parentVals.z-1)
                                    {
                                        if(
                                                getWalkability(id,parentVals.x+1,parentVals.y+1,parentVals.z) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y,parentVals.z-1) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z-1) == Unwalkable &&
                                                getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                        else if(
                                                getWalkability(id,parentVals.x+1,parentVals.y,parentVals.z-1) == Unwalkable &&
                                                getWalkability(id,parentVals.x,parentVals.y+1,parentVals.z) == Unwalkable
                                                )
                                        {
                                            corner = Unwalkable;
                                        }
                                    }


                                    if (corner == Walkable) {

                                        //	If not already on the open list, add it to the open list.
                                        if (whichList[a][b][c] != onOpenList)
                                        {

                                            //Create a new open list item in the binary heap.
                                            newOpenListItemid = newOpenListItemid + 1; //each new item has a unique id #
                                            m = numberOfOpenListItems+1;
                                            openList[m] = newOpenListItemid;//place the new open list item (actually, its id#) at the bottom of the heap
                                            openPositions[newOpenListItemid] = Ogre::Vector3(a,b,c);//record the x,y,zi coordinates of the new item

                                            //Figure out its G cost
                                            if (
                                                    (abs(a-parentVals.x) == 1 && abs(b-parentVals.y) == 1)||
                                                    (abs(a-parentVals.x) == 1 && abs(c-parentVals.z) == 1)||
                                                    (abs(b-parentVals.y) == 1 && abs(c-parentVals.z) == 1)
                                                    )
                                            {
                                                if(movers[id].diagonolMovement) {
                                                    addedgCost = 14;//cost of going to diagonal squares
                                                } else {
                                                    addedgCost = 140;
                                                }
                                            }
                                            else
                                            {
                                                addedgCost = 10;//cost of going to non-diagonal squares
                                            }
                                            if(getWalkability(id,a,b,c) != Walkable)
                                            {
                                                addedgCost+=getWalkability(id,a,b,c);
                                            }
                                            gCost[a][b][c] = gCost[parentVals.x][parentVals.y][parentVals.z] + addedgCost;

                                            //Figure out its H and F costs and parent
                                            hCost[openList[m]] = 10*(abs(a - targetPos.x) + abs(b - targetPos.y) + abs(c - targetPos.z));
                                            fCost[openList[m]] = gCost[a][b][c] + hCost[openList[m]];
                                            parentPositions[a][b][c] = parentVals;

                                            //Move the new open list item to the proper place in the binary heap.
                                            //Starting at the bottom, successively compare to parent items,
                                            //swapping as needed until the item finds its place in the heap
                                            //or bubbles all the way to the top (if it has the lowest F cost).
                                            while (m != 1) //While item hasn't bubbled to the top (m=1)
                                            {
                                                //Check if child's F cost is < parent's F cost. If so, swap them.
                                                if (fCost[openList[m]] <= fCost[openList[m/2]])
                                                {
                                                    temp = openList[m/2];
                                                    openList[m/2] = openList[m];
                                                    openList[m] = temp;
                                                    m = m/2;
                                                }
                                                else
                                                    break;
                                            }
                                            numberOfOpenListItems = numberOfOpenListItems+1;//add one to the number of items in the heap

                                            //Change whichList to show that the new item is on the open list.
                                            whichList[a][b][c] = onOpenList;
                                        }

                                        //============================================================================================================
                                        //8.If adjacent cell is already on the open list, check to see if this
                                        //	path to that cell from the starting location is a better one.
                                        //	If so, change the parent of the cell and its G and F costs.
                                        else //If whichList(a,b) = onOpenList
                                        {
                                            //                                            std::cout << "8: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
                                            //Figure out the G cost of this possible new path
                                            if (
                                                    (abs(a-parentVals.x) == 1 && abs(b-parentVals.y) == 1)||
                                                    (abs(a-parentVals.x) == 1 && abs(c-parentVals.z) == 1)||
                                                    (abs(b-parentVals.y) == 1 && abs(c-parentVals.z) == 1)
                                                    )
                                            {
                                                if(movers[id].diagonolMovement) {
                                                    addedgCost = 10;//cost of going to diagonal squares
                                                } else {
                                                    addedgCost = 140;
                                                }
                                            }
                                            else
                                            {
                                                addedgCost = 10;//cost of going to non-diagonal tiles
                                            }
                                            tempgCost = gCost[parentVals.x][parentVals.y][parentVals.z] + addedgCost;

                                            //If this path is shorter (G cost is lower) then change
                                            //the parent cell, G cost and F cost.
                                            if (tempgCost < gCost[a][b][c]) //if G cost is less,
                                            {
                                                parentPositions[a][b][c] = parentVals; //change the square's parent
                                                gCost[a][b][c] = tempgCost;//change the G cost

                                                //Because changing the G cost also changes the F cost, if
                                                //the item is on the open list we need to change the item's
                                                //recorded F cost and its position on the open list to make
                                                //sure that we maintain a properly ordered open list.
                                                for (int x = 1; x <= numberOfOpenListItems; x++) //look for the item in the heap
                                                {
                                                    if(openPositions[openList[x]]==Ogre::Vector3(a,b,c))
                                                    {
                                                        fCost[openList[x]] = gCost[a][b][c] + hCost[openList[x]];//change the F cost

                                                        //See if changing the F score bubbles the item up from it's current location in the heap
                                                        m = x;
                                                        while (m != 1) //While item hasn't bubbled to the top (m=1)
                                                        {
                                                            //Check if child is < parent. If so, swap them.
                                                            if (fCost[openList[m]] < fCost[openList[m/2]])
                                                            {
                                                                temp = openList[m/2];
                                                                openList[m/2] = openList[m];
                                                                openList[m] = temp;
                                                                m = m/2;
                                                            }
                                                            else
                                                                break;
                                                        }
                                                        break; //exit for x = loop
                                                    } //If openX(openList(x)) = a
                                                } //For x = 1 To numberOfOpenListItems
                                            }//If tempgCost < gCost(a,b)

                                        }//else If whichList(a,b) = onOpenList
                                    }//If not cutting a corner
                                }//If not a wall/obstacle square.
                            }//If not already on the closed list
                        }//If not off the map

                    }//for (c = parentZval-1; c <= parentZval+1; c++){
                }//for (a = parentXval-1; a <= parentXval+1; a++){
            }//for (b = parentYval-1; b <= parentYval+1; b++){


        }//if (numberOfOpenListItems != 0)

        //9.If open list is empty then there is no path.
        else
        {
            //            std::cout << "8.5/9: path = NonExistent; break;" << std::endl;
            path = NonExistent; break;
        }
        //        std::cout << "9: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
        //If target is added to open list then path has been found.
        if (whichList[targetPos.x][targetPos.y][targetPos.z] == onOpenList)
        {
            path = Found;
            break;
        }
    } while(movers[id].tiles<movers[id].maxTiles);//Do until path is found or deemed nonexistent
    if(movers[id].tiles>=movers[id].maxTiles)
    {
        std::cout << "Max tiles reached. Current Tiles: " << movers[id].tiles<<" Max tiles allowed: " << movers[id].maxTiles <<std::endl;
        goto noPath;
        //        path = Found;
    }

    //============================================================================================================
    //10.Save the path if it exists.
    //    std::cout << "10: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
    if (path == Found)
    {

        //a.Working backwards from the target to the starting location by checking
        //	each cell's parent, figure out the length of the path.
        localPathPos = targetPos;
        do
        {
            //Look up the parent of the current cell.
            localPathPos = parentPositions[localPathPos.x][localPathPos.y][localPathPos.z];

            //Figure out the path length
            try{
                //                std::cout <<"PATHLENGTH: "<<pathLength[pathfinderid]<<std::endl;
                if(movers[id].pathLength<1000) {
                    movers[id].pathLength = movers[id].pathLength + 1;
                } else {
                    path = Found;
                    break;
                }
            } catch(...) {
                path = NotStarted;
                movers[id].pathLength = 1;
                break;
            }
        } while (localPathPos!=startingPos);

        //b.Resize the data bank to the right size in bytes
        /*
        if(movers[id].path.size()>movers[id].pathLength)
        {
            movers[id].path.resize(movers[id].pathLength);
            std::vector<Ogre::Vector3>(movers[id].path).swap(movers[id].path);
        } else
        {
            movers[id].path.reserve(movers[id].pathLength);
        }
        */
        movers[id].path.clear();
        movers[id].path.resize(movers[id].pathLength);

        //c. Now copy the path information over to the databank. Since we are
        //	working backwards from the target to the start location, we copy
        //	the information to the data bank in reverse order. The result is
        //	a properly ordered set of path data, from the first step to the
        //	last.
        localPathPos = targetPos;
        cellPosition = movers[id].pathLength;
        do
        {
            cellPosition = cellPosition - 1;//work backwards 2 integers
            //If negative, Break for safety.
            try{
                if(cellPosition>=0) {
                    movers[id].path.at(cellPosition) = localPathPos;
                } else {
                    break;
                }
            } catch(...) {
                path = NotStarted;
                break;
            }

            //d.Look up the parent of the current cell.
            localPathPos = parentPositions[localPathPos.x][localPathPos.y][localPathPos.z];

            //e.If we have reached the starting square, exit the loop.
        }
        while (localPathPos!=startingPos);

        //============================================================================================================
        //11.Read the first path step into xPath/yPath arrays
        //        std::cout << "11: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
        readPath(id);

    }
    return path;

    //13.If there is no path to the selected target, set the pathfinder's
    //	xPath and yPath equal to its current location and return that the
    //	path is nonexistent.
noPath:
    //    std::cout << "13: AStar3D::findPath(uint32 id, uint32 targetid)" << std::endl;
    movers[id].path.clear();
    movers[id].pathPos = movers[id].pos;
    return NonExistent;
}

int AStar3D::getWalkability(uint32 id, Ogre::Vector3 pos)
{
    //    read_t lock(mutex);

    if(map!=FloatArrayPtr_t())
    {
        if(
                pos == (movers[movers[id].targetid].pos*(1.0f/tileSize)) ||
                pos == (movers[id].pos*(1.0f/tileSize))
                )
        {
            return getWalkabilityAnyClear(id, pos);
        }

        int finalValue = Walkable;
        int tempValue;
        int oneDpos;
        for(int x=pos.x;x<=pos.x+((1.0f/tileSize)*0.5f);++x)
        {
            for(int y=pos.y;y<=pos.y+((1.0f/tileSize)*0.5f);++y)
            {
                for(int z=pos.z;z<=pos.z+((1.0f/tileSize)*0.5f);++z)
                {
                    if(x<width&&y<height&&z<depth)
                    {
                        oneDpos = ((int)(x*(1.0f/tileSize))) + width  * (((int)(y*(1.0f/tileSize))) + depth * ((int)(z*(1.0f/tileSize))));
                        tempValue = movers[id].walkability.at(map->at(oneDpos));
                        if(tempValue>finalValue)
                        {
                            finalValue = tempValue;
                        }
                    }
                }
            }
        }
        return finalValue;
    }
    else
    {
        return Walkable;
    }
}

int AStar3D::getWalkability(uint32 id, int x, int y, int z)
{
    if(map!=FloatArrayPtr_t())
    {
        Ogre::Vector3 pos(x,y,z);
        if(
                pos == (movers[movers[id].targetid].pos*(1.0f/tileSize)) ||
                pos == (movers[id].pos*(1.0f/tileSize))
                )
        {
            return getWalkabilityAnyClear(id, pos);
        }
        int finalValue = Walkable;
        int tempValue;
        int oneDpos;
        for(int xx=x;xx<=x+((1.0f/tileSize)*0.5f);++xx)
        {
            for(int yy=y;yy<=y+((1.0f/tileSize)*0.5f);++yy)
            {
                for(int zz=z;zz<=z+((1.0f/tileSize)*0.5f);++zz)
                {
                    if(xx<width&&yy<height&&zz<depth)
                    {
                        oneDpos = ((int)(xx*(1.0f/tileSize))) + width  * (((int)(yy*(1.0f/tileSize))) + depth * ((int)(zz*(1.0f/tileSize))));
                        tempValue = movers[id].walkability.at(map->at(oneDpos));
                        if(tempValue>finalValue)
                        {
                            finalValue = tempValue;
                        }
                    }
                }
            }
        }
        return finalValue;
    }
    else
    {
        return Walkable;
    }
}
/**
  *Same as the other two getWalkability methods, only this checks for the lowest (most walkable) value
  */
int AStar3D::getWalkabilityAnyClear(uint32 id, Ogre::Vector3 pos)
{
    //    read_t lock(mutex);

    if(map!=FloatArrayPtr_t())
    {
        int finalValue = Unwalkable;
        int tempValue;
        int oneDpos;
        for(int x=pos.x;x<=pos.x+((1.0f/tileSize)*0.5f);++x)
        {
            for(int y=pos.y;y<=pos.y+((1.0f/tileSize)*0.5f);++y)
            {
                for(int z=pos.z;z<=pos.z+((1.0f/tileSize)*0.5f);++z)
                {
                    if(x<width&&y<height&&z<depth)
                    {
                        oneDpos = ((int)(x*(1.0f/tileSize))) + width  * (((int)(y*(1.0f/tileSize))) + depth * ((int)(z*(1.0f/tileSize))));
                        tempValue = movers[id].walkability.at(map->at(oneDpos));
                        if(tempValue<finalValue)
                        {
                            finalValue = tempValue;
                        }
                    }
                }
            }
        }
        return finalValue;
    }
    else
    {
        return Walkable;
    }
}

int AStar3D::registerMover(Ogre::Vector3 startingPos, WalkMap_t walkability,bool diagonolMovement, uint32 tilesToUpdate, uint32 movesTilUpdate,uint32 maxTiles)
{
    //    upgrade_write_t lock(mutex);
    //    write_t writeLock(lock);

    //Returning zero means that there is no space
    int id = 0;
    for(int i = 1;i<maxNumberMovers;i++) {
        if(ids[i]==Empty) {
            id = i;
            Mover mover = {startingPos,i,walkability,NotStarted,0,0,Ogre::Vector3(0,0,0),tilesToUpdate,movesTilUpdate,diagonolMovement,maxTiles};
            movers[i] = mover;
            ids[i] = Taken;
            break;
        }
    }
    return id;
}

void AStar3D::removeMover(uint32 id)
{
    //    upgrade_write_t lock(mutex);
    //    write_t writeLock(lock);

    movers.erase(id);
    ids[id] = Empty;
}

PathStatus AStar3D::getPathStatus(uint32 id)
{
    //    read_t lock(mutex);

    if(movers.find(id)!=movers.end())
    {
        return movers[id].pathStatus;
    } else
    {
        return NonExistent;
    }
}

bool AStar3D::isClosedOnTarget(uint32 id, uint32 targetID)
{
    //    read_t lock(mutex);

    if(movers.find(id)!=movers.end())
    {
        if(movers.find(targetID)!=movers.end())
        {
            if(movers[id].pos == movers[targetID].pos)
            {
                return true;
            } else
            {
                return false;
            }
        } else
        {
            //Maybe this should be flipped to true??? Not sure.
            return false;
        }
    } else
    {
        //Maybe this should be flipped to true??? No sure.
        return false;
    }
}

void AStar3D::setPos(uint32 id, Ogre::Vector3 pos)
{
    //    read_t lock(mutex);

    if(movers.find(id)!=movers.end())
    {
        movers[id].pos = pos;
    }
}

Ogre::Vector3 AStar3D::getPos(uint32 id)
{
    //    read_t lock(mutex);

    if(movers.find(id)!=movers.end())
    {
        return movers[id].pos;
    } else {
        return Ogre::Vector3(0,0,0);
    }
}

void AStar3D::setMap(FloatArrayPtr_t map)
{
    //    upgrade_write_t lock(mutex);
    //    write_t writeLock(lock);

    //Convert to AStar3D mode
    for(int i=0;i<map->size();++i)
    {
        if(map->at(i) < 1)
        {
            map->at(i) = -1;
        }
    }

    this->map = map;
}

WalkMap_t AStar3D::getWalkMap(int walkType)
{
    WalkMap_t walkMap;
    switch(walkType)
    {
    case Flier:
        walkMap[-1] = Walkable;
        walkMap[1] = Unwalkable;
        break;
    case Free:
        walkMap[-1] = Walkable;
        walkMap[1] = Walkable;
        break;
    default:
        break;
    }

    return walkMap;
}

void AStar3D::debugDraw()
{
    //    read_t lock(mutex);
    //    std::cout << "AStar3D::debugDraw()"<< std::endl;
    for(std::map<int,Mover>::const_iterator it = movers.begin();it!=movers.end();++it)
    {
        //        std::cout << "drawing sphere at: " << (*it).second.pos.x <<","<<(*it).second.pos.y<<","<<(*it).second.pos.z<<std::endl;
        DebugDrawer::getSingleton().drawSphere((*it).second.pos,(1.0f/tileSize)*0.5,Ogre::ColourValue(1, 0, 0));
        for(int i=0;i<(*it).second.path.size();++i)
        {
            //            std::cout << "drawing PATH SPHERE at: " << (*it).second.path.at(i).x <<","<<(*it).second.path.at(i).y<<","<<(*it).second.path.at(i).z<<std::endl;
            if(i==(*it).second.path.size()-1)
            {
                DebugDrawer::getSingleton().drawSphere((*it).second.path.at(i)*(1.0f/tileSize),(1.0f/tileSize)*0.5,Ogre::ColourValue(0, 0.8, 0.6));
            } else
            {
                DebugDrawer::getSingleton().drawSphere((*it).second.path.at(i)*(1.0f/tileSize),(1.0f/tileSize)*0.5,Ogre::ColourValue(0, 0.6, 0.75));
            }
        }
    }
    Ogre::Vector3 vert0 = Ogre::Vector3(0,0,0)*width;
    Ogre::Vector3 vert1 = Ogre::Vector3(0,1,0)*width;
    Ogre::Vector3 vert2 = Ogre::Vector3(1,1,0)*width;
    Ogre::Vector3 vert3 = Ogre::Vector3(1,0,0)*width;

    Ogre::Vector3 vert4 = Ogre::Vector3(0,0,1)*width;
    Ogre::Vector3 vert5 = Ogre::Vector3(0,1,1)*width;
    Ogre::Vector3 vert6 = Ogre::Vector3(1,1,1)*width;
    Ogre::Vector3 vert7 = Ogre::Vector3(1,0,1)*width;


    Ogre::Vector3 face1[4] = {vert0,vert1,vert2,vert3};
    Ogre::Vector3 face2[4] = {vert0,vert1,vert5,vert4};
    Ogre::Vector3 face3[4] = {vert2,vert1,vert5,vert6};
    Ogre::Vector3 face4[4] = {vert3,vert2,vert6,vert7};
    Ogre::Vector3 face5[4] = {vert0,vert4,vert7,vert3};
    Ogre::Vector3 face6[4] = {vert4,vert5,vert6,vert7};

    DebugDrawer::getSingletonPtr()->drawQuad(face1,Ogre::ColourValue(0, 0.6, 0.75));
    DebugDrawer::getSingletonPtr()->drawQuad(face2,Ogre::ColourValue(0, 0.6, 0.75));
    DebugDrawer::getSingletonPtr()->drawQuad(face3,Ogre::ColourValue(0, 0.6, 0.75));
    DebugDrawer::getSingletonPtr()->drawQuad(face4,Ogre::ColourValue(0, 0.6, 0.75));
    DebugDrawer::getSingletonPtr()->drawQuad(face5,Ogre::ColourValue(0, 0.6, 0.75));
    DebugDrawer::getSingletonPtr()->drawQuad(face6,Ogre::ColourValue(0, 0.6, 0.75));
    //    std::cout << "AStarDebugDraw" << std::endl;
}
