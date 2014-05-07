#include "roguemapfactory.h"
#include "core/Global.h"
#include "gameplay/enemies/sentry.h"

using namespace neuro;

RogueMapFactory::RogueMapFactory() :
    MapFactory(60,60),
    gridSizeX(nrand(2,3)),
    gridSizeY(nrand(2,3)),
    numGone(0)
{
}

bool contains_pos(Square pos, Tile::PtrArray2D& tiles)
{
    if(pos.getX() >= 0 && pos.getX() < tiles.size())
    {
        if(pos.getY() >= 0 && pos.getY() < tiles.at(pos.getX()).size())
            return true;
    }

    return false;
}

Map::Ptr RogueMapFactory::createMap()
{
    Map::Ptr rogue_map(new Map());

    srand(time(NULL));

    //    TileMultiArray tiles(boost::extents[mapSizeX][mapSizeY]);

    Tile::PtrArray2D tiles;

    NeuroAI::PtrArray ai;
    Hackable::PtrArray hackables;

    SubSystem::PtrArray2D subSystems;
    Player::Ptr player(new Player(Square(0,0)));

    initTiles(tiles);
    createSubSystems(subSystems);
    setStartingSubSystem(subSystems,tiles,player,ai);
    addSubSystemTilesToMap(subSystems,tiles);

    rogue_map->setTiles(tiles);
    rogue_map->setHackables(hackables);
    rogue_map->setSubSystems(subSystems);
    rogue_map->setPlayer(player);
    rogue_map->setAI(ai);

    createSubSystemLayout(subSystems, 0.5);

    createHallways(subSystems, rogue_map);
    generateTileManuals(rogue_map);
    return rogue_map;
}

void RogueMapFactory::initTiles(Tile::PtrArray2D &tiles)
{
    for(int x=0;x<mapSizeX;x++) {
        Tile::PtrArray tile_vec;
        for(int y=0;y<mapSizeY;y++) {
            Tile::Ptr tile(new Tile(Tile::BLANK,Square(x,y)));
            tile_vec.push_back(tile);
        }
        tiles.push_back(tile_vec);
    }
}

void RogueMapFactory::createSubSystems(SubSystem::PtrArray2D &subSystems)
{
    for(int x = 0; x < gridSizeX; x++)
    {
        SubSystem::PtrArray vec;
        for(int y = 0; y < gridSizeY; y++)
        {
            SubSystem::Ptr subSystem(new SubSystem());

            if(!subSystem.get())
                continue; // NULL pointer

            if(nrand(0, 10) < 5)
            {
                subSystem->setGone(true);
                numGone++;
            }

            else
            {
                initSubSystem(subSystem,x,y);
                if(!subSystem->isGone())
                {
                    subSystem->async_init_manual();
                }
            }

            vec.push_back(subSystem);
        }

        subSystems.push_back(vec);
    }
}

void RogueMapFactory::initSubSystem(SubSystem::Ptr subSystem,int gridX,int gridY)
{
    if(!subSystem.get())
        return;

    int offsetX = neuro::nrand(0,(mapSizeX/gridSizeX)*0.25) - (mapSizeX/gridSizeX)*0.125;
    int offsetY = neuro::nrand(0,(mapSizeY/gridSizeY)*0.25) - (mapSizeY/gridSizeY)*0.125;

    //    offsetX = 0;
    //    offsetY = 0;

    int x = (mapSizeX/gridSizeX) * (gridX - 1) + offsetX + (mapSizeX*0.5);
    int y = (mapSizeY/gridSizeY) * (gridY - 1) + offsetY + (mapSizeY*0.5);

    subSystem->setPos(Square(x,y));

    for(unsigned int x = 0; x < subSystem->getWidth(); ++x)
    {
        for(unsigned int y = 0; y < subSystem->getHeight(); y++)
        {
            Tile::Ptr tile(new  Tile(Tile::BLANK,Square(x,y)));
            subSystem->setTile(x, y, tile);
        }
    }

    if(!subSystem->isGone()) {
        //X
        for(int ix=0;ix<subSystem->getWidth();ix++) {
            //Y
            for(int iy=0;iy<subSystem->getHeight();iy++) {
                if(ix==0&&iy==0) {
                    //Bottom Left Corner
                    Tile::Ptr tile(new Tile(Tile::TOPCORNER,Square(ix+x,iy+y)));
                    subSystem->setTile(ix,iy,tile);
                } else if(ix==0&&iy==(subSystem->getHeight()-1)) {
                    //Top Left Corner
                    Tile::Ptr tile(new Tile(Tile::CORNER,Square(ix+x,iy+y)));
                    subSystem->setTile(ix,iy,tile);
                } else if(ix==(subSystem->getWidth()-1)&&y==(subSystem->getHeight()-1)) {
                    //Top Right Corner
                    Tile::Ptr tile(new Tile(Tile::CORNER,Square(ix+x,iy+y)));
                    subSystem->setTile(ix,iy,tile);
                } else if(ix==(subSystem->getWidth()-1)&&y==0) {
                    //Bottom Right Corner
                    Tile::Ptr tile(new Tile(Tile::TOPCORNER,Square(ix+x,iy+y)));
                    subSystem->setTile(ix,iy,tile);
                } else if(ix==0) {
                    //Left Wall
                    Tile::Ptr tile(new Tile(Tile::SIDEWALL,Square(ix+x,iy+y)));
                    subSystem->setTile(ix,iy,tile);
                } else if(ix==(subSystem->getWidth()-1)) {
                    //Right Wall
                    Tile::Ptr tile(new Tile(Tile::SIDEWALL,Square(ix+x,iy+y)));
                    subSystem->setTile(ix,iy,tile);
                } else if(iy==(subSystem->getHeight()-1)) {
                    //Top Row
                    Tile::Ptr tile(new Tile(Tile::TOPWALL,Square(ix+x,iy+y)));
                    subSystem->setTile(ix,iy,tile);
                } else if(iy==0) {
                    //Bottom Row
                    Tile::Ptr tile(new Tile(Tile::TOPWALL,Square(ix+x,iy+y)));
                    subSystem->setTile(ix,iy,tile);
                } else {
                    //Inside Square
                    Tile::Ptr tile(new Tile(Tile::FLOOR,Square(ix+x,iy+y)));
                    subSystem->setTile(ix,iy,tile);
                }
            }//Y
        }//X
        createPorts(subSystem);
    } else {
        //Gone
        //X
        for(int ix=0;ix<subSystem->getWidth();ix++) {
            //Y
            for(int iy=0;iy<subSystem->getHeight();iy++) {
                Tile::Ptr tile(new Tile(Tile::BLANK,Square(ix+x,iy+y)));
                subSystem->setTile(ix,iy,tile);
            }//Y
        }//X
    }
}

void RogueMapFactory::createPorts(SubSystem::Ptr subSystem)
{
    int direction = nrand(0,3);
    int x = subSystem->getPos().getX() + 2;
    int y = subSystem->getPos().getY() + 2;
    Square pos1(0,0);
    Square pos2(0,0);
    //    int pos1x,pos2x,pos1y,pos2y;
    switch(direction) {
    case 0:
        pos1 = Square(x -2, y);
        pos2 = Square(x + 2, y);
        break;
    case 1:
        pos1 = Square(x, y+2);
        pos2 = Square(x, y-2);
        break;
    case 2:
        pos1 = Square(x+2, y);
        pos2 = Square(x-2, y);
        break;
    case 3:
    default:
        pos1 = Square(x, y-2);
        pos2 = Square(x, y+2);
        break;

    }
    int portFlags = Port::Closed | Port::EnemyOwned;
    Port::Ptr port1(new Port(pos1,portFlags));
    Port::Ptr port2(new Port(pos2,portFlags));

    subSystem->setPort1(port1);
    subSystem->setPort2(port2);

    Square nodePos(subSystem->getPos().getX() + 2, subSystem->getPos().getY() + 2);
    Node::Ptr node(new Node(nodePos,0));
    subSystem->setNode(node);
    //    node->async_init_manuals();
    //    boost::shared_ptr<Room> room(this);
    //    port1->setParent(room);
    //    port2->setParent(room);
}

void RogueMapFactory::addSubSystemTilesToMap(SubSystem::PtrArray2D &subSystems,Tile::PtrArray2D& tiles)
{
    for(unsigned int gx = 0; gx < subSystems.size(); gx++)
    {
        for(unsigned int gy = 0; gy < subSystems.at(gx).size(); gy++)
        {
            if(!subSystems[gx][gy]->isGone())
            {
                for(unsigned int x = 0; x < subSystems[gx][gy]->getWidth(); ++x)
                {
                    for(unsigned int y = 0; y < subSystems[gx][gy]->getHeight(); ++y)
                    {
                        Tile::Ptr tile = subSystems[gx][gy]->getTile(x,y);
                        if(!tile.get())
                        {
                            std::cout << "addSubSystemTilesToMap: NULL POINTER TILE WTF?!?!?" << std::endl;
                            continue; // Null Pointer
                        }

                        Square pos = tile->getPos();
                        if(contains_pos(pos, tiles))
                        {
                            if(tile->getChar() != Tile::BLANK)
                            {
                                tiles[pos.getX()][pos.getY()] = tile;
                            }
                        }
                    }
                }

                Port::Ptr port1 = subSystems[gx][gy]->getPort1();
                Port::Ptr port2 = subSystems[gx][gy]->getPort2();
                Node::Ptr node = subSystems[gx][gy]->getNode();

                if(contains_pos(port1->getPos(),tiles))
                    tiles[port1->getPos().getX()][port1->getPos().getY()] = port1;
                if(contains_pos(port2->getPos(),tiles))
                    tiles[port2->getPos().getX()][port2->getPos().getY()] = port2;
                if(contains_pos(node->getPos(),tiles))
                    tiles[node->getPos().getX()][node->getPos().getY()] = node;
            }
        }
    }
}

Square randomSystem(SubSystem::PtrArray2D& subSystems, Tile::PtrArray2D& tiles)
{
    bool selected = false;
    unsigned int i = 0;

    while(!selected)
    {
        for(int x = 0; x < subSystems.size(); ++x)
        {
            for(int y = 0; y < subSystems.at(x).size(); ++y)
            {
                if(neuro::nrand(0, 9) == 0)
                {
                    if(!subSystems[x][y]->isGone() || i > 1000)
                    {
                        return Square(x, y);
                        break;
                    }
                }

                ++i;
            }
        }
    }
}

void RogueMapFactory::setStartingSubSystem(SubSystem::PtrArray2D &subSystems, Tile::PtrArray2D &tiles, Player::Ptr player, NeuroAI::PtrArray &ai)
{
    bool selected = false;
    int sel_x = 0;
    int sel_y = 0;
    unsigned int i = 0;
    Square selectedPos = randomSystem(subSystems, tiles);

    SubSystem::Ptr s = subSystems[selectedPos.getX()][selectedPos.getY()];
    s->set_starting();
    Port::Ptr p1 = s->getPort1();
    Port::Ptr p2 = s->getPort2();
    Node::Ptr n = s->getNode();

    int portFlags = Port::Open | Port::UserOwned;
    p1->setPortFlags(portFlags);
    p2->setPortFlags(portFlags);

    int nodeFlags = Node::UserOwned;
    n->setPortFlags(nodeFlags);

    Square playerPos = n->getPos();
    playerPos.setX(playerPos.getX() -1 );
    player->setPos(playerPos);
    player->init_manual();

    for(int x = 0; x < subSystems.size(); ++x)
    {
        for(int y = 0; y < subSystems.at(x).size(); ++y)
        {
            SubSystem::Ptr subSystem = subSystems.at(x).at(y);
            if(!subSystem.get())
            {
                std::cout << "SubSystem::Ptr IS EMPTY WTF MANG!?!?!" << std::endl;
                continue;
            }

            if(!subSystems[x][y]->isGone())
            {
                if(!subSystems[x][y]->isStarting())
                {
                    Square enemyPos = subSystems[x][y]->getPos();
                    enemyPos.setX(enemyPos.getX() +1 );
                    enemyPos.setY(enemyPos.getY() +2 );
                    Sentry::Ptr neuro_ai(new Sentry(enemyPos));
                    ai.push_back(neuro_ai);
                    neuro_ai->init_manual();
                }
            }
        }
    }
}
