#include "neuro/room.h"
#include "neuro/Global.h"
#include "neuro/port.h"
#include "neuro/node.h"

namespace neuro {

Room::Room(int gridX, int gridY,int gridSizeX,int gridSizeY,int mapSizeX,int mapSizeY) :
    gridX(gridX+1),
    gridY(gridY+1),
    gridSizeX(gridSizeX),
    gridSizeY(gridSizeY),
    mapSizeX(mapSizeX),
    mapSizeY(mapSizeY),
    connected(false),
    gone(false),
    start(false),
    end(false),
    sideWall(SIDEWALL),
    topWall(TOPWALL),
    corner(CORNER),
    floor(FLOOR),
    status(UNEXPLORED),
    rid(0),
    doorFlag(nrand(0,1))
{
    for(int x=0;x<gridSizeX;x++) {
        std::vector<int > v;
        for(int y=0;y<gridSizeX;y++) {
            v.push_back(0);
        }
        connectedMap.push_back(v);
    }
}

Room::~Room()
{
    if(!gone) {

    }
}

void Room::initRoom()
{
    start = false;
    int offsetX = neuro::nrand(0,(mapSizeX/gridSizeX)*0.35) - (mapSizeX/gridSizeX)*0.175;
    int offsetY = neuro::nrand(0,(mapSizeY/gridSizeY)*0.35) - (mapSizeY/gridSizeY)*0.175;
    x = (mapSizeX/gridSizeX) * (gridX - 0.7) + offsetX;
    y = (mapSizeY/gridSizeY) * (gridY - 0.7) + offsetY;
//    width = neuro::nrand(5,6);
//    height = neuro::nrand(5,6);
    width = 5;
    height = 5;
    for(int x=0;x<width;x++) {
        std::vector<boost::shared_ptr<Square> > sqv;
        for(int y=0;y<height;y++) {
            boost::shared_ptr<Square> square(new  Square(BLANK,x,y));
            sqv.push_back(square);
        }
        squares.push_back(sqv);
    }
    //Check for Gone
    if(!gone) {
        //X
        for(int ix=0;ix<width;ix++) {
            //Y
            for(int iy=0;iy<height;iy++) {
                if(ix==0&&iy==0) {
                    //Bottom Left Corner
//                    delete squares[ix][iy];
                    boost::shared_ptr<Square> square(new Square(TOPCORNER,ix+x,iy+y));
                    squares[ix][iy] = square;
                } else if(ix==0&&iy==(height-1)) {
                    //Top Left Corner
//                    delete squares[ix][iy];
                    boost::shared_ptr<Square> square(new Square(corner,ix+x,iy+y));
                    squares[ix][iy] = square;
                } else if(ix==(width-1)&&y==(height-1)) {
                    //Top Right Corner
                    boost::shared_ptr<Square> square(new Square(corner,ix+x,iy+y));
                    squares[ix][iy] = square;
                } else if(ix==(width-1)&&y==0) {
                    //Bottom Right Corner
                    boost::shared_ptr<Square> square(new Square(TOPCORNER,ix+x,iy+y));
                    squares[ix][iy] = square;
                } else if(ix==0) {
                    //Left Wall
                    boost::shared_ptr<Square> square(new Square(sideWall,ix+x,iy+y));
                    squares[ix][iy] = square;
                } else if(ix==(width-1)) {
                    //Right Wall
                    boost::shared_ptr<Square> square(new Square(sideWall,ix+x,iy+y));
                    squares[ix][iy] = square;
                } else if(iy==(height-1)) {
                    //Top Row
                    boost::shared_ptr<Square> square(new Square(topWall,ix+x,iy+y));
                    squares[ix][iy] = square;
                } else if(iy==0) {
                    //Bottom Row
                    boost::shared_ptr<Square> square(new Square(topWall,ix+x,iy+y));
                    squares[ix][iy] = square;
                } else {
                    //Inside Square
                    boost::shared_ptr<Square> square(new Square(floor,ix+x,iy+y));
                    squares[ix][iy] = square;
                }
            }//Y
        }//X
        createDoors();
    } else {
        //Gone
        //X
        for(int ix=0;ix<width;ix++) {
            //Y
            for(int iy=0;iy<height;iy++) {
                boost::shared_ptr<Square> square(new Square(BLANK,ix+x,iy+y));
                squares[ix][iy] = square;
            }//Y
        }//X
    }
}

void Room::createDoors()
{
    int direction =nrand(0,1);
    Vec2i door1,door2;
    switch(direction) {
    case 0:
        door1.x = x + 2;
        door1.y = y;

        door2.x = x+2;
        door2.y = y+4;
        break;
    case 1:
        door1.x = x;
        door1.y = y+2;

        door2.x = x+4;
        door2.y = y+2;
        break;
    case 2:
        door1.x = x + 2;
        door1.y = y;

        door2.x = x+2;
        door2.y = y+4;
        break;
    case 3:
        door1.x = x + 2;
        door1.y = y;

        door2.x = x+2;
        door2.y = y+4;
        break;

    }
    int portFlags = Port::Closed | Port::EnemyOwned;
    boost::shared_ptr<Port> sport(new Port(door1,portFlags));
    boost::shared_ptr<Port> sport2(new Port(door2,portFlags));
    port1 = sport;
    port2 = sport2;
    boost::shared_ptr<Room> room(this);
    port1->setParent(room);
    port2->setParent(room);
}

Vec2i Room::getGridPos()
{
    Vec2i gridPos;
    gridPos.x = gridX-1;
    gridPos.y = gridY-1;
    return gridPos;
}

int Room::getDoorFlag() {
    if(doorFlag==0) {
        doorFlag = 1;
    } else {
        doorFlag = 0;
    }
    return doorFlag;
}

boost::shared_ptr<Port> Room::getPort1()
{
    return port1;
}

boost::shared_ptr<Port> Room::getPort2()
{
    return port2;
}

boost::shared_ptr<Square> Room::getSquare(int w, int h)
{
    return squares[w][h];
}

int Room::getX()
{
    return x;
}

int Room::getY()
{
    return y;
}

//Check to see if a room is connected to this room
bool Room::checkIfConnected(Vec2i room)
{
    if(connectedMap[room.x][room.y]==0) {
        return false;
    } else {
        return true;
    }
}

//Add a room to this room's connection map
void Room::addConnectedRoom(Vec2i room)
{
    //If it's already connected to that room, ignor this
    if(connectedMap[room.x][room.y]==0) {
        connectedMap[room.x][room.y] = 1;
        connectedRooms.push_back(room);
    }
}

int Room::getMidX()
{
    return x + (width*0.5);
}

int Room::getMidY()
{
    return y + (height*0.5);
}

void Room::setConnected(bool connected)
{
    this->connected = connected;
}

bool Room::isConnected()
{
    return connected;
}

void Room::setGone()
{
    this->gone = true;
}

void Room::setStart(bool start)
{
    this->start = start;
}

bool Room::getStart()
{
    return start;
}

void Room::setEnd(bool end)
{
    this->end = end;
}

bool Room::getEnd()
{
    return end;
}

int Room::getWidth()
{
    return width;
}

int Room::getHeight()
{
    return height;
}

std::vector<Vec2i> Room::getConnectedRooms()
{
    return connectedRooms;
}

bool Room::isGone()
{
    return gone;
}

void Room::setStatus(STATUS status)
{
    this->status = status;
}

bool Room::checkContains(Vec2i pos)
{
    if(
            x <=pos.x&&
            y <=pos.y&&
            x+width>pos.x&&
            y+height>pos.y
            ) {
        return true;
    } else {
        return false;
    }
}

bool Room::statusChange(Vec2i heroPos)
{
    STATUS newStatus;
    bool contains = checkContains(heroPos);
    //Check to see if hero is in this room
    if(contains) {
        //If they are, then the room is occupied
        newStatus = OCCUPIED;
        //Does this change the status of this room?
        if(status==newStatus) {
            //If not, do nothing and return false
            return false;
        } else {
            //If so, change the status and return true
            status = newStatus;
            return true;
        }//Change Check
    }//Contains Check
    else
        //The hero is not in the room
    {
        //Figure out what previous status was
        switch(status) {
        case UNEXPLORED:
            //It's still unexplored, no change
            return false;
            break;
        case EXPLORED:
            //It's still explored, but no one in here, no change
            return false;
            break;
        case OCCUPIED:
            //It was occupied, but now they've left, return true
            this->status = EXPLORED;
            return true;
            break;
        }//Status Check
    }//No one in Room Check
}

STATUS Room::getStatus() {
    return status;
}

void Room::setID(int rid)
{
    /*
    if(start) {
        std::cout << "Start Room Internal RID: " << rid << std::endl;
    } else {
        std::cout << "Internal Room ID: " << rid << std::endl;
    }
    */
    this->rid = rid;
}

int Room::getID()
{
    return rid;
}

void Room::setNode(boost::shared_ptr<Node> node)
{
    this->node = node;
}

boost::shared_ptr<Node> Room::getNode()
{
    return node;
}

int Room::getNumConnected()
{
    return connectedRooms.size();
}

}
