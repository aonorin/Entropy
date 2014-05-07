#include "neuro/map.h"
#include "stdio.h"
#include "stdlib.h"
#include "neuro/virus.h"
#include "Global.h"
#include "src/Core/update.h"
#include "boost/shared_ptr.hpp"
#include "src/Core/terminal.h"
#include "src/Utility/WeakFunc.h"
#include "src/Core/Entropy.h"
#include "neuro/hero.h"
#include "neuro/astar.h"
#include "neuro/phantom.h"
#include "neuro/backtracer.h"
#include "neuro/room.h"
#include "OGRE/OgreTextAreaOverlayElement.h"
#include "neuro/port.h"
#include "neuro/node.h"
#include "OGRE/OgreOverlayManager.h"
#include "OGRE/OgreOverlayContainer.h"
/*
  TODO:
    Features:

    Bugs:
    Fix Edge Squares not showing or being traversable

    optional:
    -Tweak player collision detection to make the player "smaller"?

    done?:
    -Crashing when phantom scans a port with a player program running on it?mm
    -Implement Starting Room being in different places in map
    -Implement Clean up and restart
    -Fix Respawn Program not going away
    -Fix Bounds crash hero movement
    -Fix Phantoms following you even though you are invisible
    -Fix not being able to walk to lower edge
    -Fixed ID's
    -Fix floating phantoms
    -Fix HoneyPot (Related to Phantoms freaking out?)
    -Fix Phantoms freezing and flipping out
    -Implement Phantom attacks
    -Implement Something with nodes!?
    -Fix Unlimited Encryptions
    -Implement Backtracer
    -Implement Activated alarms(both player and enemy)
    -Fix Running programs crashing neurocaster on log out
    -Fix Unconnected rooms
  */

namespace neuro {

Map::Map(int gsX, int gsY,int msX,int msY) :
    gridSizeX(gsX),
    gridSizeY(gsY),
    mapSizeX(msX),
    mapSizeY(msY),
    pathCount(0),
    hallWalkability(boost::extents[mapSizeX][mapSizeY]),
    enemyWalkability(boost::extents[mapSizeX][mapSizeY]),
    terrainMap(boost::extents[mapSizeX][mapSizeY]),
    rooms(boost::extents[gridSizeX][gridSizeY]),
    pathIter(0),
    roomID(0),
    numGone(0),
    hero(new Hero(HERO,10,10)),
    gameState(InProgress),
    honeyPotActive(false),
    setup(false),
    resourcesDeleted(false)
{
    MAPSIZEX = mapSizeX;
    MAPSIZEY = mapSizeY;
    for(int x=0;x<mapSizeX;x++) {
        for(int y=0;y<mapSizeY;y++) {
            boost::shared_ptr<Square> square(new Square(BLANK,x,y));
            terrainMap[x][y] = square;
        }
    }

    //    enemies.push_back(Virus(VIRUS,20,20));
    createRooms();
    //Input Room data into the charMap
    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            for(int w=0;w<rooms[x][y]->getWidth();w++) {
                for(int h=0;h<rooms[x][y]->getHeight();h++) {
                    boost::shared_ptr<Square> square = rooms[x][y]->getSquare(w,h);
                    if(square->getX()>=0&&square->getX()<mapSizeX) {
                        if(square->getY()>=0&&square->getY()<mapSizeY) {
                            //                            delete terrainMap[square->getX()][square->getY()];
                            terrainMap[square->getX()][square->getY()] = square;
                        }
                    }
                }
            }
        }
    }
    connectRooms2();
    createHallways();
    testConnectedness();
    updateMap();
    int numNotGone = 0;
    int numEnemies = 0;
    //Create Ports and Nodes
    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            if(!rooms[x][y]->isGone()) {
                numNotGone++;
                terrainMap[rooms[x][y]->getPort1()->getX()][rooms[x][y]->getPort1()->getY()] = rooms[x][y]->getPort1();
                terrainMap[rooms[x][y]->getPort2()->getX()][rooms[x][y]->getPort2()->getY()] = rooms[x][y]->getPort2();

                if(rooms[x][y]->getPort1()->getPortFlags() & Port::EnemyOwned) {
                    if(nrand(0,3)>=3) {
                        rooms[x][y]->getPort1()->addPortFlag(Port::Daemoned);
                    }
                    if(nrand(0,3)>=3) {
                        rooms[x][y]->getPort1()->addPortFlag(Port::Alarmed);
                    }
                }
                if(rooms[x][y]->getPort2()->getPortFlags() & Port::EnemyOwned) {
                    if(nrand(0,3)>=3) {
                        rooms[x][y]->getPort2()->addPortFlag(Port::Daemoned);
                    }
                    if(nrand(0,3)>=3) {
                        rooms[x][y]->getPort2()->addPortFlag(Port::Alarmed);
                    }
                }

                int nodeFlags = 0;
                if(nrand(0,3)>=2) {
                    nodeFlags |= Node::Encrypted;
                }
                //                switch(nrand(0,5)) {
                switch(1) {
                case 0:
                    nodeFlags |= Node::Scanner;
                    break;
                case 1:
                    nodeFlags |= Node::Data;
                    break;
                case 2:
                    nodeFlags |= Node::Control;
                    break;
                case 3:
                    nodeFlags |= Node::Spawner;
                    break;
                case 4:
                    nodeFlags |= Node::Wetware;
                    break;
                }
                if(rooms[x][y]->getStart()) {
                    nodeFlags = 0;
                    nodeFlags |= Node::UserOwned;
                    nodeFlags |= Node::IO;
                    nodeFlags |= Node::Infected;
                }
                Vec2i nPos(rooms[x][y]->getMidX(),rooms[x][y]->getMidY());
                boost::shared_ptr<Node> node(new Node(nPos,nodeFlags));
                if(rooms[x][y]->getStart()) {
                    node->setChar(NODE);
                }
                terrainMap[nPos.x][nPos.y] = node;
                rooms[x][y]->setNode(node);
                node->setParent(rooms[x][y]);
            }
        }
    }
    convertToEnemyWalkMap();
    updateMap();
    aStar = boost::shared_ptr<AStar>(new AStar(mapSizeX,mapSizeY,1,30,enemyWalkability));
    aStar->setDiaganolMovement(false);
    hero->id = aStar->registerMover(hero->getPos(),1);
    int startID;
    //Place Enemies down
    //    while(numEnemies*2<numNotGone) {
    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            if(!rooms[x][y]->isGone()) {
                int rid = aStar->registerMover(Vec2i(rooms[x][y]->getMidX()-1,rooms[x][y]->getMidY()),1);
                rooms[x][y]->setID(rid);
                aStar->setPos(rid,Vec2i(rooms[x][y]->getMidX()-1,rooms[x][y]->getMidY()));
                if(rooms[x][y]->getStart()) {
                    startID = rid;
                    //                    std::cout << "STARTING POS: "; Vec2i(rooms[x][y]->getMidX()-1,rooms[x][y]->getMidY()).print(); std::cout << std::endl;
                    hero->setPos(Vec2i(rooms[x][y]->getMidX()-1,rooms[x][y]->getMidY()));
                    rooms[x][y]->getNode()->captured();
                } else {
                    Vec2i nPos(rooms[x][y]->getMidX(),rooms[x][y]->getMidY());
                    int pid = aStar->registerMover(Vec2i(nPos.x,nPos.y+1),1);
                    //                    boost::shared_ptr<Map> map(this);
                    boost::shared_ptr<Phantom> phantom(new Phantom(Vec2f(nPos.x,nPos.y+1),pid,NeuroAI::Idle,NeuroAI::Patrolling,this));
                    phantoms.push_back(phantom);
                    //                    std::cout << "Room ID: " << rid << std::endl;
                    //                    std::cout << "PHANTOM ID: " << phantoms.back()->getID() << std::endl;
                    numEnemies++;
                }
            }
        }
    }
    for(int i=0;i<phantoms.size();i++) {
        phantoms.at(i)->chooseTarget();
    }

    //Place BackTracer
    while(true) {
        int x = nrand(0,gridSizeX-1);
        int y = nrand(0,gridSizeY-1);
        if(!rooms[x][y]->isGone()) {
            if(!rooms[x][y]->isGone()) {
                if(!rooms[x][y]->getStart()) {
                    //                    boost::shared_ptr<Map> map(this);
                    backTracer = boost::shared_ptr<BackTracer>(new BackTracer(Vec2i(rooms[x][y]->getMidX(),rooms[x][y]->getMidY()),this,startID));
                    rooms[x][y]->getNode()->changeType(Node::Scanner);
                    rooms[x][y]->setEnd(true);
                    rooms[x][y]->getNode()->reveal();
                    rooms[x][y]->getNode()->enemyInfect();
                    rooms[x][y]->getPort1()->addPortFlag(Port::Daemoned);
                    rooms[x][y]->getPort2()->addPortFlag(Port::Daemoned);
                    rooms[x][y]->getPort2()->addPortFlag(Port::Alarmed);
                    rooms[x][y]->getPort2()->addPortFlag(Port::Alarmed);
                    rooms[x][y]->getPort1()->activatePrograms();
                    rooms[x][y]->getPort2()->activatePrograms();

                    //Add Extra  Port Protection Around Scanner
                    /*
                    w w w w w
                    w 4 5 6 w
                    1 3 N 7 2
                    w 10 9 8w
                    w w w w w
                      */
                    Vec2i pos = Vec2i(rooms[x][y]->getMidX(),rooms[x][y]->getMidY());
                    int portFlags = 0;
                    portFlags |= Port::Daemoned;
                    portFlags |= Port::EnemyOwned;
                    portFlags |= Port::Closed;
                    boost::shared_ptr<Port> port3(new Port(Vec2i(pos.x-1,pos.y+0),portFlags));
                    boost::shared_ptr<Port> port4(new Port(Vec2i(pos.x-1,pos.y+1),portFlags));
                    boost::shared_ptr<Port> port5(new Port(Vec2i(pos.x+0,pos.y+1),portFlags));
                    boost::shared_ptr<Port> port6(new Port(Vec2i(pos.x+1,pos.y+1),portFlags));
                    boost::shared_ptr<Port> port7(new Port(Vec2i(pos.x+1,pos.y+0),portFlags));
                    boost::shared_ptr<Port> port8(new Port(Vec2i(pos.x+1,pos.y-1),portFlags));
                    boost::shared_ptr<Port> port9(new Port(Vec2i(pos.x+0,pos.y-1),portFlags));
                    boost::shared_ptr<Port> port10(new Port(Vec2i(pos.x-1,pos.y-1),portFlags));
                    port3->activatePrograms();
                    port4->activatePrograms();
                    port5->activatePrograms();
                    port6->activatePrograms();
                    port7->activatePrograms();
                    port8->activatePrograms();
                    port9->activatePrograms();
                    port10->activatePrograms();
                    port3->setParent(rooms[x][y]);
                    port4->setParent(rooms[x][y]);
                    port5->setParent(rooms[x][y]);
                    port6->setParent(rooms[x][y]);
                    port7->setParent(rooms[x][y]);
                    port8->setParent(rooms[x][y]);
                    port9->setParent(rooms[x][y]);
                    port10->setParent(rooms[x][y]);
                    terrainMap[pos.x-1][pos.y+0] = port3;
                    terrainMap[pos.x-1][pos.y+1] = port4;
                    terrainMap[pos.x-0][pos.y+1] = port5;
                    terrainMap[pos.x+1][pos.y+1] = port6;
                    terrainMap[pos.x+1][pos.y+0] = port7;
                    terrainMap[pos.x+1][pos.y-1] = port8;
                    terrainMap[pos.x+0][pos.y-1] = port9;
                    terrainMap[pos.x-1][pos.y-1] = port10;
                    break;
                }
            }
        }
    }
    setup = true;
}

Map::~Map()
{
    deleteResources();
}

void Map::deleteResources()
{
    if(!resourcesDeleted) {
        resourcesDeleted = true;
        for(int x=0;x<mapSizeX;x++) {
            for(int y=0;y<mapSizeY;y++) {
                if(terrainMap[x][y]!=0) {
                    if(terrainMap[x][y]->getChar()==NODE||terrainMap[x][y]->getChar()==ENEMYNODE) {
                        //                    delete ((Node*)terrainMap[x][y]);
                        boost::static_pointer_cast<Node>(terrainMap[x][y])->deleteResources();
                        boost::static_pointer_cast<Node>(terrainMap[x][y]).reset();
                    } else if(terrainMap[x][y]->getChar()==PORT||terrainMap[x][y]->getChar()==ENEMYPORT) {
                        //                    delete ((Port*)terrainMap[x][y]);
                        boost::static_pointer_cast<Port>(terrainMap[x][y])->deleteResources();
                        boost::static_pointer_cast<Port>(terrainMap[x][y]).reset();
                    } else {
                        //                    delete terrainMap[x][y];
                        terrainMap[x][y].reset();
                    }
                }
            }
        }
        //Delete rooms;
        for(int x=0;x<rooms.size();x++) {
            for(int y=0;y<rooms[x].size();y++) {
                //            delete rooms[x][y];
                rooms[x][y].reset();
            }
        }
        //Delete Phantoms
        for(int i=0;i<phantoms.size();i++) {
            //        delete phantoms.at(i);
            phantoms.at(i).reset();
        }
        //    delete aStar;
        hero.reset();
        backTracer.reset();
        if(gameState==Lost || gameState==Won) {
            Entropy::getSingletonPtr()->addQeueudDeleteOverlays(gameEnd);
        }
    }
}

//=============================================================================================
//Map Generation
//=============================================================================================
void Map::createRooms()
{
    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            rooms[x][y] = boost::shared_ptr<Room>(new Room(x,y,gridSizeX,gridSizeY,mapSizeX,mapSizeY));
            if(nrand(0,10)<5) {
                rooms[x][y]->setGone();
                numGone++;
            }
            rooms[x][y]->initRoom();
        }
    }
}

void Map::connectRooms2()
{
    std::vector<boost::shared_ptr<Room> > connectedRooms;
    int algo = nrand(2,3);
    int direction = nrand(0,1);
    int dx=0;
    int dy=0;
    switch(algo) {

    case 0:
        //down to up
        for(int x=0;x<gridSizeX;x++) {
            for(int y=0;y<gridSizeY;y++) {
                if(!rooms[x][y]->isGone()) {
                    rooms[x][y]->setConnected(true);
                    connectedRooms.push_back(rooms[x][y]);
                }
            }
        }
        break;

    case 1:
        //left to right
        for(int y=0;y<gridSizeY;y++) {
            for(int x=0;x<gridSizeX;x++) {
                if(!rooms[x][y]->isGone()) {
                    rooms[x][y]->setConnected(true);
                    connectedRooms.push_back(rooms[x][y]);
                }
            }
        }
        break;

    case 2:
        //up and down back and forth
        direction = nrand(0,1);

        while(dx<gridSizeX) {
            if(direction==0) {
                direction=1;
                for(int y=0;y<gridSizeY;y++) {
                    if(!rooms[dx][y]->isGone()) {
                        rooms[dx][y]->setConnected(true);
                        connectedRooms.push_back(rooms[dx][y]);
                    }
                }
            } else {
                direction=0;
                for(int y=gridSizeY-1;y>=0;y-=1) {
                    if(!rooms[dx][y]->isGone()) {
                        rooms[dx][y]->setConnected(true);
                        connectedRooms.push_back(rooms[dx][y]);
                    }
                }
            }
            dx++;
        }
        break;

    case 3:
        //left and right back and forth
        direction = nrand(0,1);

        while(dy<gridSizeY) {
            if(direction==0) {
                direction=1;
                for(int x=0;x<gridSizeX;x++) {
                    if(!rooms[x][dy]->isGone()) {
                        rooms[x][dy]->setConnected(true);
                        connectedRooms.push_back(rooms[x][dy]);
                    }
                }
            } else {
                direction=0;
                for(int x=gridSizeX-1;x>=0;x-=1) {
                    if(!rooms[x][dy]->isGone()) {
                        rooms[x][dy]->setConnected(true);
                        connectedRooms.push_back(rooms[x][dy]);
                    }
                }
            }
            dy++;
        }
        break;
    }

    //Unconnected Rooms cleanup
    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            if(!rooms[x][y]->isGone()) {
                if(!rooms[x][y]->isConnected()) {
                    rooms[x][y]->setConnected(true);
                    connectedRooms.push_back(rooms[x][y]);
                }
            }
        }
    }

    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            rooms[x][y]->setConnected(false);
        }
    }

    int currentRoom = nrand(0,connectedRooms.size()-1);
    currentRoom = 0;

    int connected;
    //    if(nrand(0,3)>=2) {
    if(false) {
        //Random Neighbor Connection
        while(connectedRooms.size()>1) {
            connected = nrand(0,connectedRooms.size()-1);
            if(connected!=currentRoom) {
                Vec2i pos = connectedRooms.at(connected)->getGridPos();
                connectedRooms.at(currentRoom)->setConnected(true);
                connectedRooms.at(currentRoom)->addConnectedRoom(pos);
                connectedRooms.erase(connectedRooms.begin()+currentRoom);
                if(currentRoom<connected) {
                    currentRoom = connected -1;
                } else {
                    currentRoom = connected;
                }

            }
        }
    } else {

        //Sequential Neighbor Connection
        int i = 0;
        while(i<connectedRooms.size()) {
            connected = (currentRoom+1)%connectedRooms.size();
            Vec2i pos = connectedRooms.at(connected)->getGridPos();
            if(
                    connectedRooms.at(currentRoom)->getGridPos().x!=connectedRooms.at(connected)->getGridPos().x||
                    connectedRooms.at(currentRoom)->getGridPos().y!=connectedRooms.at(connected)->getGridPos().y
                    ) {
                connectedRooms.at(currentRoom)->setConnected(true);
                connectedRooms.at(currentRoom)->addConnectedRoom(pos);
            } else {
                std::cout << "SAME ROOM!?!?" << std::endl;
            }
            if(i==connectedRooms.size()-1) {
                connectedRooms.at(connected)->setConnected(true);
            }
            currentRoom = connected;
            i++;
        }
    }

    //Unconnected Room Nuke
    //    int ci = connectedRooms.size()-1;

    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            if(!rooms[x][y]->isGone()) {
                if(!rooms[x][y]->isConnected()) {
                    Vec2i pos = rooms[x][y]->getGridPos();
                    std::cout << "ROOM NUKE: " <<x<<","<<y<< std::endl;
                    for(int i=0;i<3;i++) {
                        connected = nrand(0,connectedRooms.size()-1);
                        connectedRooms.at(connected)->addConnectedRoom(pos);
                        connectedRooms.at(connected)->setConnected(true);
                        connectedRooms.push_back(rooms[x][y]);
                        rooms[x][y]->setConnected(true);
                    }
                }
            }
        }
    }

    sx = 0;
    sy = 0;

    while(true) {
        int x = nrand(0,gridSizeX-1);
        int y = nrand(0,gridSizeY-1);
        if(!rooms[x][y]->isGone()) {
            //Set initial room to start
            connectedRooms.at(currentRoom)->setStart(true);
            connectedRooms.at(currentRoom)->setStatus(OCCUPIED);
            connectedRooms.at(currentRoom)->setConnected(true);
            connectedRooms.at(currentRoom)->getPort1()->enemyPortToPort();
            connectedRooms.at(currentRoom)->getPort2()->enemyPortToPort();
            sx = x;
            sy = y;
            break;
        }
    }
}

void Map::createHallways()
{
    //For each room in the grid
    //X
    for(int x=0;x<gridSizeX;x++) {
        //Y
        for(int y=0;y<gridSizeY;y++) {
            //for each room in that room's connected list
            for(int i=0;i<rooms[x][y]->getConnectedRooms().size();i++) {
                //Get connected door
                //                Vec2i target = rooms[x][y]->getConnectedRooms().at(i);
                boost::shared_ptr<Port> hDoor;
                boost::shared_ptr<Port> tDoor;


                boost::shared_ptr<Room> connectedRoom = rooms[rooms[x][y]->getConnectedRooms().at(i).x][rooms[x][y]->getConnectedRooms().at(i).y];
                int doorNum = rooms[x][y]->getDoorFlag();
                if(doorNum==0) {
                    hDoor = rooms[x][y]->getPort1();
                } else {
                    hDoor = rooms[x][y]->getPort2();
                }
                doorNum = connectedRoom->getDoorFlag();
                if(doorNum==0) {
                    tDoor = connectedRoom->getPort1();
                } else {
                    tDoor = connectedRoom->getPort2();
                }
                //                delete terrainMap[hDoor->getX()][hDoor->getY()];
                //                delete terrainMap[tDoor->getX()][tDoor->getY()];
                terrainMap[hDoor->getX()][hDoor->getY()] = boost::shared_ptr<Square>(new Square(HALLWAY,hDoor->getX(),hDoor->getY()));
                terrainMap[tDoor->getX()][tDoor->getY()] = boost::shared_ptr<Square>(new Square(HALLWAY,tDoor->getX(),tDoor->getY()));
                convertToHallWalkMap();
                AStar astar(mapSizeX,mapSizeY,1,4,hallWalkability);
                astar.setDiaganolMovement(false);
                int home = astar.registerMover(hDoor->getPos(),1);
                int target = astar.registerMover(tDoor->getPos(),1);
                std::vector<Vec2i> path;
                int count = 0;
                while(!astar.closedOnTarget(home,target)) {
                    count++;
                    if(count >600) {
                        std::cout << "COULDN'T CONNECT ROOMS!";
                        break;
                    }
                    path.push_back(astar.move(home,target,1));
                }
                for(int i=0;i<path.size();i++) {
                    //                    delete terrainMap[path.at(i).x][path.at(i).y];
                    terrainMap[path.at(i).x][path.at(i).y] = boost::shared_ptr<Square>(new Square(HALLWAY,path.at(i).x,path.at(i).y));
                }
            }//ConectedList
        }//Y
    }//X
}

void Map::testConnectedness()
{
    std::vector<Vec2i> unconnectedList;

    //Find Unreachable Rooms
    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            if(!rooms[x][y]->isGone()) {
                boost::shared_ptr<Port> hDoor;
                boost::shared_ptr<Port> tDoor;
                int doorNum = rooms[x][y]->getDoorFlag();
                if(doorNum==0) {
                    hDoor = rooms[x][y]->getPort1();
                } else {
                    hDoor = rooms[x][y]->getPort2();
                }
                doorNum = rooms[sx][sy]->getDoorFlag();
                if(doorNum==0) {
                    tDoor = rooms[sx][sy]->getPort1();
                } else {
                    tDoor = rooms[sx][sy]->getPort2();
                }
                //                delete terrainMap[hDoor->getX()][hDoor->getY()];
                //                delete terrainMap[tDoor->getX()][tDoor->getY()];
                terrainMap[hDoor->getX()][hDoor->getY()] = boost::shared_ptr<Square>(new Square(HALLWAY,hDoor->getX(),hDoor->getY()));
                terrainMap[tDoor->getX()][tDoor->getY()] = boost::shared_ptr<Square>(new Square(HALLWAY,tDoor->getX(),tDoor->getY()));
                convertToEnemyWalkMap();
                AStar astar(mapSizeX,mapSizeY,1,4,enemyWalkability);
                astar.setDiaganolMovement(false);
                int home = astar.registerMover(hDoor->getPos(),1);
                int target = astar.registerMover(tDoor->getPos(),1);
                Vec2i pos;
                int count = 0;
                while(!astar.closedOnTarget(home,target)) {
                    count++;
                    if(count >600) {
                        std::cout << "COULDN'T CONNECT ROOMS!" << std::endl;
                        unconnectedList.push_back(Vec2i(x,y));
                        break;
                    }
                    pos = astar.move(home,target,1);
                    if(pos.x==0&&pos.y==0) {
                        std::cout << "UNREACHABLE ROOM FOUND!" << std::endl;
                        unconnectedList.push_back(Vec2i(x,y));
                        break;
                    }
                }
            }
        }
    }

    for(int i=0;i<unconnectedList.size();i++) {
        int x = unconnectedList.at(i).x;
        int y = unconnectedList.at(i).y;
        boost::shared_ptr<Port> hDoor;
        boost::shared_ptr<Port> tDoor;

        int doorNum = rooms[x][y]->getDoorFlag();
        if(doorNum==0) {
            hDoor = rooms[x][y]->getPort1();
        } else {
            hDoor = rooms[x][y]->getPort2();
        }
        doorNum = rooms[sx][sy]->getDoorFlag();
        if(doorNum==0) {
            tDoor = rooms[sx][sy]->getPort1();
        } else {
            tDoor = rooms[sx][sy]->getPort2();
        }
        //        delete terrainMap[hDoor->getX()][hDoor->getY()];
        //        delete terrainMap[tDoor->getX()][tDoor->getY()];
        terrainMap[hDoor->getX()][hDoor->getY()] = boost::shared_ptr<Square>(new Square(HALLWAY,hDoor->getX(),hDoor->getY()));
        terrainMap[tDoor->getX()][tDoor->getY()] = boost::shared_ptr<Square>(new Square(HALLWAY,tDoor->getX(),tDoor->getY()));
        convertToHallWalkMap();
        AStar astar(mapSizeX,mapSizeY,1,4,hallWalkability);
        astar.setDiaganolMovement(false);
        int home = astar.registerMover(hDoor->getPos(),1);
        int target = astar.registerMover(tDoor->getPos(),1);
        std::vector<Vec2i> path;
        int count = 0;
        while(!astar.closedOnTarget(home,target)) {
            count++;
            if(count >600) {
                std::cout << "*FINAL* COULDN'T CONNECT ROOMS!";
                break;
            }
            path.push_back(astar.move(home,target,1));
        }
        for(int i=0;i<path.size();i++) {
            //            delete terrainMap[path.at(i).x][path.at(i).y];
            terrainMap[path.at(i).x][path.at(i).y] = boost::shared_ptr<Square>(new Square(HALLWAY,path.at(i).x,path.at(i).y));
        }
    }
}

//=============================================================================================
//Player/AI Movement
//=============================================================================================

void Map::moveHero(int ch)
{
    //Left of target square,target square, right of target square
    int l,r;
    bool lCheck=false;
    bool rCheck=false;
    if(hero.use_count()>0) {
        //Hero bounding box
        float hl = hero->getFX()-0.5;
        float hr = hero->getFX()+0.5;
        float ht = hero->getFY()+0.5;
        float hb = hero->getFY()-0.5;

        switch(ch)
        {
        case 258:
            //UP
            hero->setFacing(UP);
            if(hero->getY()<mapSizeY) {
                if(checkMapBounds(Vec2i((int)(hl+0.5),ht+hero->getSpeed()*2))) {
                    l = terrainMap[(int)(hl+0.5)][ht+hero->getSpeed()*2]->getChar();
                    if(l==FLOOR||l==HALLWAY||l==PORT||l==ENEMYNODE||l==NODE) {
                        lCheck = true;
                    }
                } else {
                    lCheck = false;
                }
                if(checkMapBounds(Vec2i(hr,ht+hero->getSpeed()))) {
                    r = terrainMap[hr][ht+hero->getSpeed()]->getChar();
                    if(r==FLOOR||r==HALLWAY||r==PORT||r==ENEMYNODE||r==NODE) {
                        rCheck = true;
                    }
                } else {
                    rCheck = false;
                }


                if(lCheck&&rCheck) {
                    hero->moveUp();
                    aStar->setPos(hero->id,hero->getPos());
                }
            }
            break;
        case 259:
            //Down
            hero->setFacing(DOWN);
            if(hero->getY()>=0) {
                if(checkMapBounds(Vec2i((int)(hl+0.5),hb-hero->getSpeed()+0.5))) {
                    l = terrainMap[(int)(hl+0.5)][hb-hero->getSpeed()+0.5]->getChar();
                    if(l==FLOOR||l==HALLWAY||l==PORT||l==ENEMYNODE||l==NODE) {
                        lCheck = true;
                    }
                }else {
                    lCheck = false;
                }
                if(checkMapBounds(Vec2i(hr,hb-hero->getSpeed()+0.5))) {
                    r = terrainMap[hr][hb-hero->getSpeed()+0.5]->getChar();
                    if(r==FLOOR||r==HALLWAY||r==PORT||r==ENEMYNODE||r==NODE) {
                        rCheck = true;
                    }
                } else {
                    rCheck = false;
                }

                if(lCheck&&rCheck) {
                    hero->moveDown();
                    aStar->setPos(hero->id,hero->getPos());
                }
            }
            break;
        case 261:
            //Right
            hero->setFacing(RIGHT);
            if(hero->getX()<mapSizeX) {
                if(checkMapBounds(Vec2i(hr+hero->getSpeed(),ht))) {
                    l = terrainMap[hr+hero->getSpeed()][ht]->getChar();
                    if(l==FLOOR||l==HALLWAY||l==PORT||l==ENEMYNODE||l==NODE) {
                        lCheck = true;
                    }
                } else {
                    lCheck = false;
                }
                if(checkMapBounds(Vec2i(hr+hero->getSpeed(),(int)(hb+0.5)))) {
                    r = terrainMap[hr+hero->getSpeed()][(int)(hb+0.5)]->getChar();
                    if(r==FLOOR||r==HALLWAY||r==PORT||r==ENEMYNODE||r==NODE) {
                        rCheck = true;
                    }
                } else {
                    rCheck = false;
                }

                if(lCheck&&rCheck) {
                    hero->moveRight();
                    aStar->setPos(hero->id,hero->getPos());
                }
            }
            break;
        case 260:
            //Left
            hero->setFacing(LEFT);
            if(hero->getX()-1>=0) {
                if(checkMapBounds(Vec2i(hl-hero->getSpeed()+0.5,ht))) {
                    l = terrainMap[hl-hero->getSpeed()+0.5][ht]->getChar();
                    if(l==FLOOR||l==HALLWAY||l==PORT||l==ENEMYNODE||l==NODE) {
                        lCheck = true;
                    }
                } else {
                    lCheck = false;
                }
                if(checkMapBounds(Vec2i(hl-hero->getSpeed()+0.5,(int)(hb+0.5)))) {
                    r = terrainMap[hl-hero->getSpeed()+0.5][(int)(hb+0.5)]->getChar();
                    if(r==FLOOR||r==HALLWAY||r==PORT||r==ENEMYNODE||r==NODE) {
                        rCheck = true;
                    }
                } else {
                    rCheck = false;
                }

                if(lCheck&&rCheck) {
                    hero->moveLeft();
                    aStar->setPos(hero->id,hero->getPos());
                }
            }
            break;
        }
        updateMap();
    }
}

void Map::updateMap()
{
    //Input Room data into the charMap
    //X
    for(int x=0;x<gridSizeX;x++) {
        //Y
        for(int y=0;y<gridSizeY;y++) {
            //Check to see if anything has changed
            if(rooms[x][y]->statusChange(hero->getPos())) {
                //if so, update that room
                for(int w=0;w<rooms[x][y]->getWidth();w++) {
                    for(int h=0;h<rooms[x][y]->getHeight();h++) {
                        terrainMap[w+rooms[x][y]->getX()][h+rooms[x][y]->getY()]->setStatus(rooms[x][y]->getStatus());
                    }
                }//Room Update
            }//Changed?
        }//Y
    }//X

    //Check areas around hero for Hallways and reveal
    //X
    for(int hx= -2;hx<4;hx++) {
        //Y
        for(int hy= -2;hy<4;hy++) {
            if(
                    (hero->getX()+hx>0&&hero->getX()+hx<(mapSizeX-1))&&
                    (hero->getY()+hy>0&&hero->getY()+hy<(mapSizeY-1))
                    ) {
                //Is it a Hallway?
                if(terrainMap[hero->getX()+hx][hero->getY()+hy]->getChar()==HALLWAY) {
                    //If so Reveal
                    terrainMap[hero->getX()+hx][hero->getY()+hy]->setStatus(EXPLORED);
                }//Hallway?
            }
        }//Y
    }//X
}

void Map::updateAI()
{
    for(int i=0;i<phantoms.size();i++) {
        boost::shared_ptr<Phantom> phantom = phantoms.at(i);
        if(phantom->checkForUpdate()) {
            aStar->setPos(phantoms.at(i)->getID(),phantoms.at(i)->getIPos());
            Vec2i pos = aStar->move(phantom->getID(),phantom->getTargetID(),1);
            if(!(pos.x==0&&pos.y==0)) {
                if(checkMapBounds(pos)) {
                    if(enemyWalkability[pos.x][pos.y]==Walkable) {
                        phantom->setNextPoint(pos);
                    } else {
                        //                        std::cout << "Phantom Unwalkable!" << std::endl;
                        phantom->setStatus(NeuroAI::Idle);
                        phantom->chooseTarget();
                    }
                } else {
                    //                    std::cout << "Phantom Out of Bounds!" << std::endl;
                    phantom->setStatus(NeuroAI::Idle);
                    phantom->chooseTarget();
                }
            } else {
                //                std::cout << "Phantom Target Unreachable!" << std::endl;
                phantom->setStatus(NeuroAI::Idle);
                phantom->chooseTarget();
            }
        }
        phantom->update();
    }

    if(setup) {
        if(hero->getSuspicion()>=1.0) {
            backTracer->activate();
        }
        if(backTracer->update()) {
            boost::function<void (void)> func = boost::bind(boost::weak_fn(&Map::gameOver,shared_from_this()));
            Entropy::getSingletonPtr()->addQeueudFunction(func);
        }
    }
}

void Map::convertToHallWalkMap()
{
    for(int x=0;x<mapSizeX;x++) {
        for(int y=0;y<mapSizeY;y++) {
            if(terrainMap[x][y]->getChar()==BLANK) {
                hallWalkability[x][y] = Walkable;
            } else if(terrainMap[x][y]->getChar()==HALLWAY) {
                //                hallWalkability[x][y] = 14;
                hallWalkability[x][y] = Walkable;
            } else {
                hallWalkability[x][y] = Unwalkable;
            }
        }
    }
}

void Map::convertToEnemyWalkMap()
{
    for(int x=0;x<mapSizeX;x++) {
        for(int y=0;y<mapSizeY;y++) {
            if(
                    terrainMap[x][y]->getChar()==HALLWAY||
                    terrainMap[x][y]->getChar()==PORT||
                    terrainMap[x][y]->getChar()==FLOOR||
                    terrainMap[x][y]->getChar()==ENEMYPORT||
                    terrainMap[x][y]->getChar()==NODE||
                    terrainMap[x][y]->getChar()==ENEMYNODE
                    ) {
                enemyWalkability[x][y] = Walkable;
            } else {
                enemyWalkability[x][y] = Unwalkable;
            }
        }
    }
}

//=============================================================================================
//Programs
//=============================================================================================
void Map::initProgram(Program::ProgramType program)
{
    boost::shared_ptr<Square> activeSquare = getProgramPos();
    boost::shared_ptr<Program> newProgram;
    if(program==Program::Encrypt) {
        if(boost::static_pointer_cast<Node>(activeSquare)->containsFlag(Node::Encrypted)) {
            newProgram = hero->initProgram(Program::Errorz,activeSquare);
        } else {
            newProgram = hero->initProgram(program,activeSquare);
        }
    } else {
        newProgram = hero->initProgram(program,activeSquare);
    }
    if(newProgram.use_count()!=0) {
        boost::function<void (int)> func = boost::bind(boost::weak_fn(&Map::programCompleted,shared_from_this()),_1);
        boost::function<void (float)> update = boost::bind( boost::weak_fn(&Program::update,newProgram),_1);
        int eid = Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(newProgram->getTime()),func,update,newProgram->getID());
        newProgram->setTimedEventID(eid);
    }
}

void Map::programCompleted(int id)
{
    if(hero.use_count()>0) {
        boost::shared_ptr<Program> program = hero->getProgram(id);
        if(program.use_count()>0) {
            boost::shared_ptr<Port> port = boost::static_pointer_cast<Port>(terrainMap[program->getPos().x][program->getPos().y]);
            boost::shared_ptr<Node> node;



            switch(program->getProgamType()) {
            case Program::Scan:
                if(port->getChar()==ENEMYPORT) {
                    if(port->containsFlag(Port::Alarmed)||port->containsFlag(Port::Daemoned)) {
                        port->activatePrograms();
                    }
                    port->getParent()->getNode()->reveal();
                }
                hero->endProgram(id);
                break;
            case Program::Glitch:
                if(port->getChar()==ENEMYPORT) {
                    if(port->containsFlag(Port::Alarmed)&&port->containsFlag(Port::Daemoned)) {
                        boost::function<void (void)> func = boost::bind( boost::weak_fn(&Port::portToEnemyPort,port));
                        Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(250),func);
                        hero->damageHealth(0.333);
                        port->activatePrograms();
                        hero->addSuspicion(1.0);
                        port->removePortFlag(Port::Daemoned);
                        port->removePortFlag(Port::Alarmed);
                        hero->endProgram(id);
                    } else if(port->containsFlag(Port::Daemoned)) {
                        boost::function<void (void)> func = boost::bind( boost::weak_fn(&Port::portToEnemyPort,port));
                        Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(250),func);
                        hero->damageHealth(0.333);
                        port->activatePrograms();
                        port->removePortFlag(Port::Daemoned);
                        hero->endProgram(id);
                    } else if(port->containsFlag(Port::Alarmed)) {
                        boost::function<void (void)> func = boost::bind( boost::weak_fn(&Port::portToEnemyPort,port));
                        Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(250),func);
                        hero->addSuspicion(1.0);
                        port->activatePrograms();
                        port->removePortFlag(Port::Alarmed);
                        hero->endProgram(id);
                    } else {
                        port->enemyPortToPort();
                        boost::function<void (void)> func = boost::bind( boost::weak_fn(&Port::portToEnemyPort,port));
                        Update::getSingletonPtr()->registerEvent(boost::posix_time::seconds(3),func);
                        boost::function<void (int)> func2 = boost::bind( boost::weak_fn(&Hero::endProgram,hero),_1);
                        Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(3100),func2,id);
                    }
                    port->getParent()->getNode()->reveal();
                }
                break;
            case Program::Crack:
                if(port->getChar()==ENEMYPORT) {
                    port->enemyPortToPort();
                    port->getParent()->getNode()->reveal();
                    port->removePortFlag(Port::Daemoned);
                    port->removePortFlag(Port::Alarmed);
                }
                hero->endProgram(id);
                break;
                //Enemy Node Programs
            case Program::Infect:
                node =  boost::static_pointer_cast<Node>(terrainMap[program->getPos().x][program->getPos().y]);
                node->addPortFlag(Node::UserOwned);
                node->addPortFlag(Node::Infected);
                node->captured();
                node->setChar(NODE);
                hero->endProgram(id);
                if(node->getParent()->getEnd()) {
                    backTracer->deactivate();
                }
                checkGameStatus();
                break;
            case Program::Decrypt:
                node =  boost::static_pointer_cast<Node>(terrainMap[program->getPos().x][program->getPos().y]);
                if(node->getChar()==ENEMYNODE) {
                    if(!node->containsFlag(Node::Encrypted)) {
                        node->addPortFlag(Node::UserOwned);
                        node->captured();
                        node->setChar(NODE);
                        if(node->getParent()->getEnd()) {
                            backTracer->deactivate();
                        }
                    }
                }
                checkGameStatus();
                hero->endProgram(id);
                break;
            case Program::Crash:
                node =  boost::static_pointer_cast<Node>(terrainMap[program->getPos().x][program->getPos().y]);
                if(node->getChar()==ENEMYNODE) {
                    if(!node->containsFlag(Node::Encrypted)) {
                        //                std::cout << "CRASHING!" << std::endl;
                        node->crash();
                    }
                }
            case Program::Errorz:
                hero->endProgram(id);
                break;
            case Program::Encrypt:
                node =  boost::static_pointer_cast<Node>(terrainMap[program->getPos().x][program->getPos().y]);
                if(node->getChar()==NODE&&node->containsFlag(Node::Infected)) {
                    node->encrypt();
                    hero->healHealth(0.1);
                }
                hero->endProgram(id);
                break;
            case Program::HoneyPot:
                node =  boost::static_pointer_cast<Node>(terrainMap[program->getPos().x][program->getPos().y]);
                if(node->getChar()==NODE&&node->containsFlag(Node::Infected)) {
                    node->honeyPot();
                    for(int i=0;i<phantoms.size();i++) {
                        phantoms.at(i)->setStatus(NeuroAI::Walking);
                        phantoms.at(i)->setTargetID(node->getParent()->getID());
                    }
                    boost::function<void (void)> func = boost::bind(boost::weak_fn(&Map::unHoneyPot,shared_from_this()));
                    Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(50000),func);
                    honeyPotActive = true;
                }
                hero->endProgram(id);
                break;
                //These Require Node Infection to run
            case Program::Daemon:
                if(port->getChar()==PORT) {
                    port->addPortFlag(Port::Daemoned);
                    port->activatePrograms();
                }
                hero->endProgram(id);
                break;
            case Program::Alarm:
                if(port->getChar()==PORT) {
                    port->addPortFlag(Port::Alarmed);
                    port->activatePrograms();
                }
                hero->endProgram(id);
                break;
            case Program::Trojan:
                if(port->getChar()==PORT) {
                    port->addPortFlag(Port::Trojaned);
                    port->activatePrograms();
                }
                hero->endProgram(id);
                break;
            case Program::BlackICE:
                if(port->getChar()==PORT) {
                    port->addPortFlag(Port::BlackICED);
                    port->activatePrograms();
                }
                hero->endProgram(id);
                break;
            }
        }
    }
}

void Map::unHoneyPot()
{
    honeyPotActive = false;
    for(int i=0;i<phantoms.size();i++) {
        if(phantoms.at(i).use_count()>0) {
            phantoms.at(i)->setStatus(NeuroAI::Walking);
            phantoms.at(i)->chooseTarget();
        }
    }
    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            if(!rooms[x][y]->isGone()) {
                if(rooms[x][y].use_count()>0) {
                    if(rooms[x][y]->getNode()->containsFlag(Node::HoneyPot)) {
                        rooms[x][y]->getNode()->unHoneyPot();
                    }
                }
            }
        }
    }
}

void Map::initProgram1()
{
    switch(testProgram(1)) {
    case PORT:
        initProgram(neuro::Program::Alarm);
        break;
    case ENEMYPORT:
        initProgram(neuro::Program::Glitch);
        break;
    case NODE:
        initProgram(neuro::Program::Encrypt);
        break;
    case ENEMYNODE:
        initProgram(neuro::Program::Decrypt);
        break;
    case Program::Errorz:
        initProgram(neuro::Program::Errorz);
        break;
    case BLANK:
        break;
    case Program::BlackICE:
        initProgram(Program::BlackICE);
        break;
    default:
        initProgram(Program::Errorz);
    }
}

void Map::initProgram2()
{
    switch(testProgram(2)) {
    case PORT:
        initProgram(neuro::Program::Daemon);
        break;
    case ENEMYPORT:
        initProgram(neuro::Program::Crack);
        break;
    case ENEMYNODE:
        initProgram(neuro::Program::Infect);
        break;
    case Program::Errorz:
        initProgram(neuro::Program::Errorz);
        break;
    case BLANK:
        break;
    case Program::BlackICE:
        initProgram(Program::BlackICE);
        break;
    default:
        initProgram(Program::Errorz);
    }
}

void Map::initProgram3()
{
    switch(testProgram(3)) {
    case PORT:
        initProgram(neuro::Program::Trojan);
        break;
    case ENEMYPORT:
        initProgram(neuro::Program::Scan);
        break;
    case ENEMYNODE:
        initProgram(neuro::Program::Crash);
        break;
    case NODE:
        if(!honeyPotActive) {
            initProgram(neuro::Program::HoneyPot);
        } else {
            initProgram(neuro::Program::Errorz);
        }
        break;
    case Program::Errorz:
        initProgram(neuro::Program::Errorz);
        break;
    case BLANK:
        break;
    case Program::BlackICE:
        initProgram(Program::BlackICE);
        break;
    default:
        initProgram(Program::Errorz);
    }
}

void Map::initProgram4()
{
    hero->setInvisible();
    boost::function<void (void)> func = boost::weak_fn(&Hero::setVisible,hero);
    boost::function<void (float)> update = boost::bind(boost::weak_fn(&Hero::invisDamage,hero),_1);
    Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(2000),func,update);
}

int Map::testProgram(int num)
{
    boost::shared_ptr<Square> activeSquare = getProgramPos();

    if(activeSquare->getChar() == PORT) {
        boost::shared_ptr<Port> port = boost::static_pointer_cast<Port>(activeSquare);
        boost::shared_ptr<Node> node = port->getParent()->getNode();
        if(node->containsFlag(Node::Infected)) {
            if(port->containsFlag(Port::Alarmed)&&port->containsFlag(Port::Trojaned)&&port->containsFlag(Port::Daemoned)) {
                return Program::BlackICE;
            } else {
                return PORT;
            }
        } else {
            return Program::Errorz;
        }
    } else if(activeSquare->getChar() ==ENEMYPORT) {
        return ENEMYPORT;
    } else if(activeSquare->getChar() ==NODE) {
        boost::shared_ptr<Node> node = boost::static_pointer_cast<Node>(activeSquare);
        if(node->containsFlag(Node::Infected)) {
            return NODE;
        } else {
            if(num==1) {
                return Program::Errorz;
            } else if(num!=3) {
                return ENEMYNODE;
            }
        }
    } else if(activeSquare->getChar() ==ENEMYNODE) {
        boost::shared_ptr<Node> node = boost::static_pointer_cast<Node>(activeSquare);
        if(num==1&&node->containsFlag(Node::Encrypted)) {
            return Program::Errorz;
        } else if(num==3&&node->containsFlag(Node::Encrypted)){
            return Program::Errorz;
        } else {
            return ENEMYNODE;
        }
    } else {
        return BLANK;
    }
}

boost::shared_ptr<Square> Map::getProgramPos()
{
    int hx=0;
    int hy=0;
    int lx=0;
    int rx=0;
    int uy=0;
    int dy=0;
    switch(hero->getFacing()){
    case UP:
        hy=1;
        lx=-1;
        rx=1;
        break;
    case DOWN:
        hy=-1;
        lx=-1;
        rx=1;
        break;
    case LEFT:
        hx=-1;
        uy=-1;
        dy=1;
        break;
    case RIGHT:
        hx=1;
        uy=-1;
        dy=1;
        break;
    }
    boost::shared_ptr<Square> activeSquare;
    if(
            terrainMap[hero->getX()+hx][hero->getY()+hy]->getChar() == PORT ||
            terrainMap[hero->getX()+hx][hero->getY()+hy]->getChar() == ENEMYPORT ||
            terrainMap[hero->getX()+hx][hero->getY()+hy]->getChar() == NODE ||
            terrainMap[hero->getX()+hx][hero->getY()+hy]->getChar() == ENEMYNODE
            ) {
        activeSquare = terrainMap[hero->getX()+hx][hero->getY()+hy];
    } else if(
              terrainMap[hero->getX()][hero->getY()]->getChar() == PORT ||
              terrainMap[hero->getX()][hero->getY()]->getChar() == ENEMYPORT ||
              terrainMap[hero->getX()][hero->getY()]->getChar() == NODE ||
              terrainMap[hero->getX()][hero->getY()]->getChar() == ENEMYNODE
              ) {
        activeSquare = terrainMap[hero->getX()][hero->getY()];
    } else if(
              terrainMap[hero->getX()+hx+lx][hero->getY()+hy+uy]->getChar() == PORT ||
              terrainMap[hero->getX()+hx+lx][hero->getY()+hy+uy]->getChar() == ENEMYPORT ||
              terrainMap[hero->getX()+hx+lx][hero->getY()+hy+uy]->getChar() == NODE ||
              terrainMap[hero->getX()+hx+lx][hero->getY()+hy+uy]->getChar() == ENEMYNODE
              ) {
        activeSquare = terrainMap[hero->getX()+hx+lx][hero->getY()+hy+uy];
    } else if(
              terrainMap[hero->getX()+hx+rx][hero->getY()+hy+dy]->getChar() == PORT ||
              terrainMap[hero->getX()+hx+rx][hero->getY()+hy+dy]->getChar() == ENEMYPORT ||
              terrainMap[hero->getX()+hx+rx][hero->getY()+hy+dy]->getChar() == NODE ||
              terrainMap[hero->getX()+hx+rx][hero->getY()+hy+dy]->getChar() == ENEMYNODE
              ) {
        activeSquare = terrainMap[hero->getX()+hx+rx][hero->getY()+hy+dy];
    } else {
        activeSquare = terrainMap[hero->getX()][hero->getY()];
    }

    return activeSquare;
}

//=============================================================================================
//Getters/Setters
//=============================================================================================

Vec2f Map::getHeroPos()
{
    Vec2f flipPos;
    flipPos.x = hero->getFPos().x;
    flipPos.y = (mapSizeY-1)-hero->getFPos().y;
    return hero->getFPos();
}

Vec2i Map::getVirusPos()
{
    Vec2i flipPos;
    flipPos.x = enemies.at(0).getPos().x;
    flipPos.y = (mapSizeY-1)-enemies.at(0).getPos().y;
    return enemies.at(0).getPos();
}

boost::shared_ptr<Room> Map::getRoom(int x,int y) {
    return rooms[x][y];
}

int Map::getGridSizeX()
{
    return gridSizeX;
}

int Map::getGridSizeY()
{
    return gridSizeY;
}

boost::multi_array<boost::shared_ptr<Square>, 2> Map::getTerrainMap()
{
    return terrainMap;
}

void Map::setSquareID(int x,int y,int id)
{
    terrainMap[x][y]->id = id;
}

boost::shared_ptr<Square> Map::getSquare(int x, int y)
{
    if(checkMapBounds(Vec2i(x,y))) {
        return terrainMap[x][y];
    } else {
        return  terrainMap[0][0];
    }
}

int Map::getMapSizeX()
{
    return mapSizeX;
}

int Map::getMapSizeY()
{
    return mapSizeY;
}

/*
void Map::setRoomID(int x, int y, int id)
{
    rooms[x][y]->setID(id);
}
*/

boost::shared_ptr<Hero> Map::getHero()
{
    return hero;
}

void Map::deletePhantom(int id)
{
    for(int i=0;i<phantoms.size();i++) {
        if(phantoms.at(i)->getID()==id) {
            aStar->removeMover(phantoms.at(i)->getID());
            boost::shared_ptr<Node> node = boost::static_pointer_cast<Node>(terrainMap[phantoms.at(i)->getNodePos().x][phantoms.at(i)->getNodePos().y]);
            if( !(node->containsFlag(Node::UserOwned)) && !(node->containsFlag(Node::Crashed)) ) {
                //                Program* program = new Program(Program::Respawn,phantoms.at(i)->getNodePos(),Program::getProgramID());
                boost::shared_ptr<Program> program(new Program(Program::Respawn,phantoms.at(i)->getNodePos(),Program::getProgramID()));

                node->startRespawner(program);
                boost::function<void (int)> func = boost::bind(boost::weak_fn(&Map::respawnPhantom,shared_from_this()),_1);
                boost::function<void (float)> update = boost::bind( boost::weak_fn(&Node::updateRespawner,node),_1);
                int oneDPos = (phantoms.at(i)->getNodePos().y*mapSizeX)+phantoms.at(i)->getNodePos().x;
                //                boost::shared_ptr<Map> map(this);
                Update::getSingletonPtr()->registerEvent(boost::posix_time::milliseconds(program->getTime()),func,update,oneDPos);
            }

            //            delete phantoms.at(i);
            phantoms.erase(phantoms.begin()+i);
            break;
        }
    }
}

void Map::respawnPhantom(int oneDPos)
{
    Vec2i nodePos(0,0);
    nodePos.x = oneDPos%mapSizeX;
    nodePos.y = (oneDPos-nodePos.x)/mapSizeX;
    if( (boost::static_pointer_cast<Node>(terrainMap[nodePos.x][nodePos.y])->isRespawnActive()) ) {
        boost::static_pointer_cast<Node>(terrainMap[nodePos.x][nodePos.y])->stopRespawner();
        int pid = aStar->registerMover(Vec2i(nodePos.x,nodePos.y+1),1.0);
        //        boost::shared_ptr<Map> map(this);
        boost::shared_ptr<Phantom> phantom(new Phantom(Vec2f(nodePos.x,nodePos.y+1),pid,NeuroAI::Idle,NeuroAI::Patrolling,this));
        phantom->chooseTarget();
        phantoms.push_back(phantom);
    }
}

void Map::checkGameStatus()
{
    int totalNodes = 0;
    int capturedNodes = 0;
    for(int x=0;x<gridSizeX;x++) {
        for(int y=0;y<gridSizeY;y++) {
            if(!rooms[x][y]->isGone()) {
                totalNodes++;
                if(rooms[x][y]->getNode()->containsFlag(Node::UserOwned)) {
                    capturedNodes++;
                }
                if(rooms[x][y]->getNode()->containsFlag(Node::Crashed)) {
                    totalNodes-=1;
                }
            }
        }
    }
    if(totalNodes<=capturedNodes) {
        boost::function<void (void)> func = boost::bind(&Map::gameWon,this);
        Entropy::getSingletonPtr()->addQeueudFunction(func);
    } else if(hero->getHealth() <= 0) {
        boost::function<void (void)> func = boost::bind(&Map::gameOver,this);
        Entropy::getSingletonPtr()->addQeueudFunction(func);
    }

}

void Map::gameOver()
{
    if(gameState==InProgress) {
        gameEnd = (Ogre::TextAreaOverlayElement*)Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("TextArea","GameEnd");
        gameEnd->setMetricsMode(Ogre::GMM_RELATIVE);
        gameEnd->setDimensions(0.5,0.5);
        //        if(hero.use_count()>0) {
        //            gameEnd->setPosition(hero->getPos().x*Terminal::getSingletonPtr()->pixelSize,hero->getPos().y*Terminal::getSingletonPtr()->pixelSize);
        //        }
        gameEnd->setPosition(0.5,0.5);
        gameEnd->setFontName("bluehigh");
        gameEnd->setCharHeight(0.2);
        gameEnd->setAlignment(Ogre::TextAreaOverlayElement::Center);
        gameEnd->setColour(Ogre::ColourValue(1,0.125,0.125,1.0));
        gameEnd->setCaption("GAME OVER");
        //        Terminal::getSingletonPtr()->_overlay4->add2D((Ogre::OverlayContainer*)textArea);
        Terminal::getSingletonPtr()->relativePanel->addChild(gameEnd);
        gameState = Lost;
        //        std::cout << "GAME OVER!" << std::endl;
    }
}

void Map::gameWon()
{
    if(gameState==InProgress) {
        gameEnd = (Ogre::TextAreaOverlayElement*)Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("TextArea","GameEnd");
        gameEnd->setMetricsMode(Ogre::GMM_RELATIVE);
        gameEnd->setDimensions(0.5,0.5);
        //        if(hero.use_count()>0) {
        //            gameEnd->setPosition(hero->getPos().x*Terminal::getSingletonPtr()->pixelSize,hero->getPos().y*Terminal::getSingletonPtr()->pixelSize);
        //        }
        gameEnd->setPosition(0.5,0.5);
        gameEnd->setFontName("bluehigh");
        gameEnd->setCharHeight(0.2);
        gameEnd->setAlignment(Ogre::TextAreaOverlayElement::Center);
        gameEnd->setColour(Ogre::ColourValue(0.125,1.0,0.125,1.0));
        gameEnd->setCaption("YOU WIN");
        Terminal::getSingletonPtr()->relativePanel->addChild(gameEnd);
        //        std::cout << "You Win!" << std::endl;
        gameState = Won;
    }
}

int Map::getGameState()
{
    return gameState;
}

boost::shared_ptr<AStar> Map::getAStar()
{
    return aStar;
}

boost::shared_ptr<BackTracer> Map::getBackTracer()
{
    return backTracer;
}

}
