#ifndef ROOM_H
#define ROOM_H

#include <string.h>
#include <stdlib.h>
#include <vector>
#include "neuro/square.h"
#include "boost/shared_ptr.hpp"

class Terminal;

namespace neuro {

class Port;
class Node;

class Room
{
public:
    Room(int gridX, int gridY, int gridSizeX, int gridSizeY,int mapSizeX,int mapSizeY);
    ~Room();
    void initRoom();
    void createDoors();
    void setGone();
    void setStart(bool start);
    bool getStart();
    void setEnd(bool end);
    bool getEnd();
    bool isConnected();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    void setConnected(bool connected);
    void addConnectedRoom(Vec2i room);
    int getNumConnected();
    boost::shared_ptr<Square> getSquare(int w, int h);
    void createHallways();
    std::vector<Vec2i> getConnectedRooms();
    bool checkIfConnected(Vec2i room);
    bool isGone();
    int getMidX();
    int getMidY();
    STATUS status;
    void setStatus(STATUS status);
    STATUS getStatus();
    bool checkContains(Vec2i pos);
    bool statusChange(Vec2i heroPos);
    void setID(int id);
    int getID();
    int getDoorFlag();
    boost::shared_ptr<Port> getPort1();
    boost::shared_ptr<Port> getPort2();
    Vec2i getGridPos();
    void setNode(boost::shared_ptr<Node> node);
    boost::shared_ptr<Node> getNode();
    int doorFlag;

private:
    boost::shared_ptr<Port> port1;
    boost::shared_ptr<Port> port2;
    boost::shared_ptr<Node> node;
    int sideWall,topWall,corner,floor;
    int gridX,gridY,gridSizeX,gridSizeY,mapSizeX,mapSizeY;
    int x,y,width,height;
    bool connected,gone,start,end;
    int rid;
    std::vector<std::vector<boost::shared_ptr<Square> > > squares;
    std::vector<Vec2i> connectedRooms;
    std::vector<std::vector<int > > connectedMap;

};

}

#endif // ROOM_H
