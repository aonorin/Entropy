#include "astar.h"

namespace neuro {


AStar::AStar(int mapWidth, int mapHeight, int tileSize, int numberPeople,WalkArray walkability) :
    mapWidth(mapWidth),
    mapHeight(mapHeight),
    tileSize(tileSize),
    maxPeople(numberPeople+1),
    onClosedList(10),
    walkability(walkability),

    openList(boost::extents[mapWidth*mapHeight+2]),
    whichList(boost::extents[mapWidth+1][mapHeight+1]),
    openX(boost::extents[mapWidth*mapHeight+2]),
    openY(boost::extents[mapWidth*mapHeight+2]),
    parentX(boost::extents[mapWidth+1][mapHeight+1]),
    parentY(boost::extents[mapWidth+1][mapHeight+1]),
    fCost(boost::extents[mapWidth*mapHeight+2]),
    gCost(boost::extents[mapWidth+1][mapHeight+1]),
    hCost(boost::extents[mapWidth*mapHeight+2]),

    pathLength(boost::extents[maxPeople+1]),
    pathLocation(boost::extents[maxPeople+1]),
    pathStatus(boost::extents[maxPeople+1]),
    xPath(boost::extents[maxPeople+1]),
    yPath(boost::extents[maxPeople+1]),
    pathBank(new int*[maxPeople+1]),
    xLoc(boost::extents[maxPeople+1]),
    yLoc(boost::extents[maxPeople+1]),
    speed(boost::extents[maxPeople+1]),
    ids(boost::extents[maxPeople+1])
{
    initialize();
}

AStar::~AStar()
{
    end();
}

void AStar::initialize()
{
    for (int x = 0; x < maxPeople+1; x++) {
        pathBank [x] = (int*) malloc(4);
        ids[x] = Empty;
    }
}

void AStar::end()
{
    for (int x = 0; x < maxPeople+1; x++)
    {
        free (pathBank [x]);
    }
}

//==========================================================
//READ PATH DATA: These functions read the path data and convert
//it to screen pixel coordinates.
void AStar::readPath(int pathfinderid,int currentX,int currentY,
                     int pixelsPerFrame)
{
    int id = pathfinderid; //redundant, but makes the following easier to read

    //If a path has been found for the pathfinder	...
    if (pathStatus[id] == Found)
    {

        //If path finder is just starting a new path or has reached the
        //center of the current path square (and the end of the path
        //hasn't been reached), look up the next path square.
        if (pathLocation[id] < pathLength[id])
        {
            //if just starting or if close enough to center of square
            if (pathLocation[id] == 0 ||
                    (abs(currentX - xPath[id]) < pixelsPerFrame && abs(currentY - yPath[id]) < pixelsPerFrame))
                pathLocation[id] = pathLocation[id] + 1;
        }

        //Read the path data.
        xPath[id] = readPathX(id,pathLocation[id]);
        yPath[id] = readPathY(id,pathLocation[id]);

        //If the center of the last path square on the path has been
        //reached then reset.
        if (pathLocation[id] == pathLength[id])
        {
            if (abs(currentX - xPath[id]) < pixelsPerFrame
                    && abs(currentY - yPath[id]) < pixelsPerFrame) //if close enough to center of square
                pathStatus[id] = NotStarted;
        }
    }

    //If there is no path for this pathfinder, simply stay in the current
    //location.
    else
    {
        xPath[id] = currentX;
        yPath[id] = currentY;
    }
}

//-----------------------------------------------------------------------------
// Name: ReadPathX
// Desc: Reads the x coordinate of the next path step
//-----------------------------------------------------------------------------
int AStar::readPathX(int pathfinderid,int pathLocation)
{
    int x;
    if (pathLocation <= pathLength[pathfinderid])
    {

        //Read coordinate from bank
        x = pathBank[pathfinderid] [pathLocation*2-2];

        //Adjust the coordinates so they align with the center
        //of the path square (optional). This assumes that you are using
        //sprites that are centered -- i.e., with the midHandle command.
        //Otherwise you will want to adjust this.
        x = tileSize*x + .5*tileSize;

    }
    return x;
}


//-----------------------------------------------------------------------------
// Name: ReadPathY
// Desc: Reads the y coordinate of the next path step
//-----------------------------------------------------------------------------
int AStar::readPathY(int pathfinderid,int pathLocation)
{
    int y;
    if (pathLocation <= pathLength[pathfinderid])
    {

        //Read coordinate from bank
        y = pathBank[pathfinderid] [pathLocation*2-1];

        //Adjust the coordinates so they align with the center
        //of the path square (optional). This assumes that you are using
        //sprites that are centered -- i.e., with the midHandle command.
        //Otherwise you will want to adjust this.
        y = tileSize*y + .5*tileSize;

    }
    return y;
}

//-----------------------------------------------------------------------------
// Name: FindPath
// Desc: Finds a path using A*
//-----------------------------------------------------------------------------
PathStatus AStar::findPath (int pathfinderid,int startingX, int startingY,
                            int targetX, int targetY)
{
    int onOpenList=0, parentXval=0, parentYval=0,
            a=0, b=0, m=0, u=0, v=0, temp=0, corner=0, numberOfOpenListItems=0,
            addedgCost=0, tempgCost = 0,
            tempx, pathX, pathY, cellPosition,
            newOpenListItemid=0;
    PathStatus path = NotStarted;

    //1. Convert location data (in pixels) to coordinates in the walkability array.
    int startX = startingX/tileSize;
    int startY = startingY/tileSize;
    targetX = targetX/tileSize;
    targetY = targetY/tileSize;

    //2.Quick Path Checks: Under the some circumstances no path needs to
    //	be generated ...

    //	If starting location and target are in the same location...
    if (startX == targetX && startY == targetY && pathLocation[pathfinderid] > 0)
        return Found;
    if (startX == targetX && startY == targetY && pathLocation[pathfinderid] == 0)
        return NonExistent;

    //	If target square is Unwalkable, return that it's a nonexistent path.
    if (walkability[targetX][targetY] == Unwalkable)
        goto noPath;

    //3.Reset some variables that need to be cleared
    if (onClosedList > 1000000) //reset whichList occasionally
    {
        for (int x = 0; x < mapWidth;x++) {
            for (int y = 0; y < mapHeight;y++)
                whichList [x][y] = 0;
        }
        onClosedList = 10;
    }
    onClosedList = onClosedList+2; //changing the values of onOpenList and onClosed list is faster than redimming whichList() array
    onOpenList = onClosedList-1;
    pathLength [pathfinderid] = NotStarted;//i.e, = 0
    pathLocation [pathfinderid] = NotStarted;//i.e, = 0
    gCost[startX][startY] = 0; //reset starting square's G value to 0

    //4.Add the starting location to the open list of squares to be checked.
    numberOfOpenListItems = 1;
    openList[1] = 1;//assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
    openX[1] = startX ; openY[1] = startY;

    //5.Do the following until a path is found or deemed nonexistent.
    do
    {

        //6.If the open list is not empty, take the first cell off of the list.
        //	This is the lowest F cost cell on the open list.
        if (numberOfOpenListItems != 0)
        {

            //7. Pop the first item off the open list.
            parentXval = openX[openList[1]];
            parentYval = openY[openList[1]]; //record cell coordinates of the item
            whichList[parentXval][parentYval] = onClosedList;//add the item to the closed list

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

            }
            //    while (!KeyDown(27));//reorder the binary heap
            while (true);


            //7.Check the adjacent squares. (Its "children" -- these path children
            //	are similar, conceptually, to the binary heap children mentioned
            //	above, but don't confuse them. They are different. Path children
            //	are portrayed in Demo 1 with grey pointers pointing toward
            //	their parents.) Add these adjacent child squares to the open list
            //	for later consideration if appropriate (see various if statements
            //	below).
            for (b = parentYval-1; b <= parentYval+1; b++){
                for (a = parentXval-1; a <= parentXval+1; a++){
                    //Check for diagonols. If not enabled, ignore
                    if(
                            (

                                (b==parentYval-1&&a==parentXval-1||
                                 b==parentYval+1&&a==parentXval+1||
                                 b==parentYval-1&&a==parentXval-1||
                                 b==parentYval+1&&a==parentXval+1)&&!diagonolMovement)
                            ) {
                        continue;
                    }
                    //	If not off the map (do this first to avoid array out-of-bounds errors)
                    if (a != -1 && b != -1 && a != mapWidth && b != mapHeight){

                        //	If not already on the closed list (items on the closed list have
                        //	already been considered and can now be ignored).
                        if (whichList[a][b] != onClosedList) {

                            //	If not a wall/obstacle square.
                            if (walkability [a][b] != Unwalkable) {

                                //	Don't cut across corners
                                corner = Walkable;
                                if (a == parentXval-1)
                                {
                                    if (b == parentYval-1)
                                    {
                                        if (walkability[parentXval-1][parentYval] == Unwalkable
                                                || walkability[parentXval][parentYval-1] == Unwalkable) \
                                            corner = Unwalkable;
                                    }
                                    else if (b == parentYval+1)
                                    {
                                        if (walkability[parentXval][parentYval+1] == Unwalkable
                                                || walkability[parentXval-1][parentYval] == Unwalkable)
                                            corner = Unwalkable;
                                    }
                                }
                                else if (a == parentXval+1)
                                {
                                    if (b == parentYval-1)
                                    {
                                        if (walkability[parentXval][parentYval-1] == Unwalkable
                                                || walkability[parentXval+1][parentYval] == Unwalkable)
                                            corner = Unwalkable;
                                    }
                                    else if (b == parentYval+1)
                                    {
                                        if (walkability[parentXval+1][parentYval] == Unwalkable
                                                || walkability[parentXval][parentYval+1] == Unwalkable)
                                            corner = Unwalkable;
                                    }
                                }
                                if (corner == Walkable) {

                                    //	If not already on the open list, add it to the open list.
                                    if (whichList[a][b] != onOpenList)
                                    {

                                        //Create a new open list item in the binary heap.
                                        newOpenListItemid = newOpenListItemid + 1; //each new item has a unique id #
                                        m = numberOfOpenListItems+1;
                                        openList[m] = newOpenListItemid;//place the new open list item (actually, its id#) at the bottom of the heap
                                        openX[newOpenListItemid] = a;
                                        openY[newOpenListItemid] = b;//record the x and y coordinates of the new item

                                        //Figure out its G cost
                                        if (abs(a-parentXval) == 1 && abs(b-parentYval) == 1)
                                            if(diagonolMovement) {
                                                addedgCost = 14;//cost of going to diagonal squares
                                            } else {
                                                addedgCost = 140;
                                            }
                                        else
                                            addedgCost = 10;//cost of going to non-diagonal squares
                                        if(walkability [a][b] != Walkable) {
                                            addedgCost+=walkability [a][b];
                                        }
                                        gCost[a][b] = gCost[parentXval][parentYval] + addedgCost;

                                        //Figure out its H and F costs and parent
                                        hCost[openList[m]] = 10*(abs(a - targetX) + abs(b - targetY));
                                        fCost[openList[m]] = gCost[a][b] + hCost[openList[m]];
                                        parentX[a][b] = parentXval ; parentY[a][b] = parentYval;

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
                                        whichList[a][b] = onOpenList;
                                    }

                                    //8.If adjacent cell is already on the open list, check to see if this
                                    //	path to that cell from the starting location is a better one.
                                    //	If so, change the parent of the cell and its G and F costs.
                                    else //If whichList(a,b) = onOpenList
                                    {

                                        //Figure out the G cost of this possible new path
                                        if (abs(a-parentXval) == 1 && abs(b-parentYval) == 1)
                                            if(diagonolMovement) {
                                                addedgCost = 14;//cost of going to diagonal squares
                                            } else {
                                                addedgCost = 140;
                                            }
                                        else
                                            addedgCost = 10;//cost of going to non-diagonal tiles
                                        tempgCost = gCost[parentXval][parentYval] + addedgCost;

                                        //If this path is shorter (G cost is lower) then change
                                        //the parent cell, G cost and F cost.
                                        if (tempgCost < gCost[a][b]) //if G cost is less,
                                        {
                                            parentX[a][b] = parentXval; //change the square's parent
                                            parentY[a][b] = parentYval;
                                            gCost[a][b] = tempgCost;//change the G cost

                                            //Because changing the G cost also changes the F cost, if
                                            //the item is on the open list we need to change the item's
                                            //recorded F cost and its position on the open list to make
                                            //sure that we maintain a properly ordered open list.
                                            for (int x = 1; x <= numberOfOpenListItems; x++) //look for the item in the heap
                                            {
                                                if (openX[openList[x]] == a && openY[openList[x]] == b) //item found
                                                {
                                                    fCost[openList[x]] = gCost[a][b] + hCost[openList[x]];//change the F cost

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
                }//for (a = parentXval-1; a <= parentXval+1; a++){
            }//for (b = parentYval-1; b <= parentYval+1; b++){

        }//if (numberOfOpenListItems != 0)

        //9.If open list is empty then there is no path.
        else
        {
            path = NonExistent; break;
        }

        //If target is added to open list then path has been found.
        if (whichList[targetX][targetY] == onOpenList)
        {
            path = Found; break;
        }

    }
    while (1);//Do until path is found or deemed nonexistent

    //10.Save the path if it exists.
    if (path == Found)
    {

        //a.Working backwards from the target to the starting location by checking
        //	each cell's parent, figure out the length of the path.
        pathX = targetX; pathY = targetY;
        do
        {
            //Look up the parent of the current cell.
            tempx = parentX[pathX][pathY];
            pathY = parentY[pathX][pathY];
            pathX = tempx;

            //Figure out the path length
            try{
//                std::cout <<"PATHLENGTH: "<<pathLength[pathfinderid]<<std::endl;
                if(pathLength[pathfinderid]<1000) {
                    pathLength[pathfinderid] = pathLength[pathfinderid] + 1;
                } else {
                    path = Found;
                    break;
                }
            } catch(...) {
                path = NotStarted;
                pathLength[pathfinderid] = 1;
                break;
            }
        }
        while (pathX != startX || pathY != startY);

        //b.Resize the data bank to the right size in bytes
        pathBank[pathfinderid] = (int*) realloc (pathBank[pathfinderid],
                                                 pathLength[pathfinderid]*8);

        //c. Now copy the path information over to the databank. Since we are
        //	working backwards from the target to the start location, we copy
        //	the information to the data bank in reverse order. The result is
        //	a properly ordered set of path data, from the first step to the
        //	last.
        pathX = targetX ; pathY = targetY;
        cellPosition = pathLength[pathfinderid]*2;//start at the end
        do
        {
            cellPosition = cellPosition - 2;//work backwards 2 integers
            //If negative, Break for safety.
            try{
                if(cellPosition>=0) {
                    pathBank[pathfinderid] [cellPosition] = pathX;
                    pathBank[pathfinderid] [cellPosition+1] = pathY;
                } else {
                    break;
                }
            } catch(...) {
                path = NotStarted;
                break;
            }

            //d.Look up the parent of the current cell.
            tempx = parentX[pathX][pathY];
            pathY = parentY[pathX][pathY];
            pathX = tempx;

            //e.If we have reached the starting square, exit the loop.
        }
        while (pathX != startX || pathY != startY);

        //11.Read the first path step into xPath/yPath arrays
        readPath(pathfinderid,startingX,startingY,1);

    }
    return path;


    //13.If there is no path to the selected target, set the pathfinder's
    //	xPath and yPath equal to its current location and return that the
    //	path is nonexistent.
noPath:
    xPath[pathfinderid] = startingX;
    yPath[pathfinderid] = startingY;
    return NonExistent;
}

Vec2i AStar::move(int id, int targetid, int tilesToUpdate)
{
    Vec2i pos(0,0);

    //1. Find Path: If target and chaser are not at the same location on the
    //	screen and no path is currently active, find a new path.
    if (xLoc[id] != xLoc[targetid] || yLoc[id] != yLoc[targetid]) {
        //If no path has been generated, generate one. Update it when
        //the chaser reaches its fifth step on the current path.
        if (pathStatus[id] == NotStarted || pathLocation[id] > tilesToUpdate)
        {
            //Generate a new path. Enter coordinates of smiley sprite (xLoc(1)/
            //yLoc(1)) as the target.
            pathStatus[id] = findPath(id,xLoc[id],yLoc[id],xLoc[targetid],yLoc[targetid]);

        }
    }
    //    std::cout << "Home loc =["<<xLoc[id]<<","<<yLoc[id]<<"]"<<std::endl;
    //    std::cout << "Id:"<<id<<std::endl;

    //2.Move chaser.
    if (pathStatus[id] == Found) {

        readPath(id,xLoc[id],yLoc[id],speed[id]);

        //2.Move sprite. xLoc/yLoc = current location of sprite. xPath and
        //	yPath = coordinates of next step on the path that were/are
        //	read using the readPath function.
//        if(!diagonolMovement) {
//            int direction = nrand(0,1);
//            if(direction) {
//                if (xLoc[id] > xPath[id]) xLoc[id] = xLoc[id] - speed[id];
//                if (xLoc[id] < xPath[id]) xLoc[id] = xLoc[id] + speed[id];
//            } else {
//                if (yLoc[id] > yPath[id]) yLoc[id] = yLoc[id] - speed[id];
//                if (yLoc[id] < yPath[id]) yLoc[id] = yLoc[id] + speed[id];
//            }

//        } else {
            if (xLoc[id] > xPath[id]) xLoc[id] = xLoc[id] - speed[id];
            if (xLoc[id] < xPath[id]) xLoc[id] = xLoc[id] + speed[id];
            if (yLoc[id] > yPath[id]) yLoc[id] = yLoc[id] - speed[id];
            if (yLoc[id] < yPath[id]) yLoc[id] = yLoc[id] + speed[id];
//        }

        //3.When sprite reaches the end location square	(end of its current
        //	path) ...
        if (pathLocation[id] == pathLength[id])
        {
            //		Center the chaser in the square (not really necessary, but
            //		it looks a little better for the chaser, which moves in 3 pixel
            //		increments and thus isn't always centered when it reaches its
            //		target).
            if (abs(xLoc[id] - xPath[id]) < speed[id]) xLoc[id] = xPath[id];
            if (abs(yLoc[id] - yPath[id]) < speed[id]) yLoc[id] = yPath[id];
        }


    }
    pos.x = xLoc[id];
    pos.y = yLoc[id];
    //    pos.print();
    return pos;
}

void AStar::setWalkabilityMap(WalkArray walkability)
{
    this->walkability = walkability;
}

void AStar::setTileSize(int tileSize)
{
    this->tileSize = tileSize;
}

int AStar::registerMover(Vec2i startingPos, int unitSpeed)
{
    //Returning zero means that there is no space
    int id = 0;
    for(int i = 1;i<maxPeople;i++) {
        if(ids[i]==Empty) {
            //            std::cout<<"Empty id found at: "<<i<<std::endl;
            id = i;
            xLoc[id] = startingPos.x;
            yLoc[id] = startingPos.y;
            //            std::cout << "Home loc =["<<xLoc[id]<<","<<yLoc[id]<<"]"<<std::endl;
            speed[id] = unitSpeed;
            pathStatus[id] = NotStarted;
            ids[i] = Taken;
            break;
        }
    }
    return id;
}

void AStar::removeMover(int id)
{
    xLoc[id] = 0;
    yLoc[id] = 0;
    speed[id] = 0;
    pathStatus[id] = NotStarted;
    ids[id] = Empty;
}

bool AStar::closedOnTarget(int id,int targetID) {
    if (xLoc[id] == xLoc[targetID] && yLoc[id] == yLoc[targetID]) {
        return true;
    } else {
        return false;
    }
}

void AStar::setDiaganolMovement(bool diaganolMovement)
{
    this->diagonolMovement = diaganolMovement;
}

void AStar::setPos(int id, Vec2i pos)
{
    xLoc[id] = pos.x;
    yLoc[id] = pos.y;
}

Vec2i AStar::getPos(int id)
{
    Vec2i pos;
    pos.x = xLoc[id];
    pos.y = yLoc[id];
    return pos;
}

}
