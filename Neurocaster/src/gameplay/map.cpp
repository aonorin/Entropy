#include "map.h"
#include "core/neurocaster.h"
#include "utility/util.h"
#include "src/gameplay/astar.h"
#include "src/gameplay/structures/hallway.h"

using namespace neuro;

Map::Map() :
    Scene(),
    width(0),
    height(0),
    tile_hilight(new TileHilight())
{
    neuro::util::seedRandomness();
    tile_hilight->init_manual();
//    grid = Grid::Ptr(new Grid());
//    grid->async_init_manual();
}

void Map::init()
{
    //Init AI
    for(int i = 0; i<ai.size(); ++i)
    {
        ai[i]->init_ai();
    }
}

void Map::continuous_update()
{
    for(int i=0;i<ai.size();++i)
    {
        ai.at(i)->continuous_update();
    }

    for(int i=0;i<hackables.size();++i)
    {
        hackables.at(i)->continuous_update();
    }

    player->continuous_update();
}

void Map::update_turn(double turn_number)
{
    //    std::cout << "turn: " << turn_number << std::endl;
    for(int i=0;i<ai.size();++i)
    {
        ai.at(i)->update_turn(turn_number);
    }

    for(int i=0;i<hackables.size();++i)
    {
        hackables.at(i)->update_turn(turn_number);
    }

    player->update_turn(turn_number);
}

void Map::new_round()
{
    for(int i=0;i<ai.size();++i)
    {
        ai.at(i)->update_ai();
    }
}

//Map creation methods
void Map::setTiles(Tile::PtrArray2D tiles)
{
    this->tiles = tiles;
    width = tiles.size();

    if(tiles.size())
        height = tiles.at(0).size();
    else
        height = 0;
}

void Map::setTile(const Ogre::Vector2& coord, Tile::Type type)
{
    tiles[coord.x][coord.y]->setChar(type);
}

void Map::setAI(NeuroAI::PtrArray ai)
{
    this->ai = ai;
}

void Map::setHackables(Hackable::PtrArray hackables)
{
    this->hackables = hackables;
}

void Map::setSubSystems(SubSystem::PtrArray2D subSystems)
{
    this->subSystems = subSystems;

    //Update map with tiles from SubSystems
    //    for(int x=0; x<subSystems.size(); ++x)
    //    {
    //        for(int y=0; y<subSystems.size(); ++y)
    //        {
    //            SubSystem::Ptr ss = subSystems.at(x).at(y);
    //            for(Tile::PtrArray2D::iterator it = ss->getTilesBegin(); it != ss->getTilesEnd(); ++it)
    //            {
    //                for(Tile::PtrArray::iterator it2 = it->begin(); it2 != it->end(); ++it2)
    //                {
    //                    Tile::Ptr tile = *it2;
    //                    Square pos = tile->getPos();
    //                    pos.setX(pos.getX() + ss->getPos().x);
    //                    pos.setY(pos.getY() + ss->getPos().y);
    //                    if(tiles.size() > pos.getX())
    //                    {
    //                        if(tiles.at(pos.x).size() >  pos.getY())
    //                        {
    //                            if(tile->getChar() != Tile::BLANK)
    //                            {
    //                                std::cout << "setSbutSystems tiles[" << pos.x << ", " << pos.y << "] = " << tile->getChar() << std::endl;
    //                                tiles[pos.x][pos.y] = tile;
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
}

Tile::Ptr Map::getTile(const Ogre::Vector2 &coord) const
{
    //    std::cout << "getTile: [" << coord.x << "," << coord.y << "]" << std::endl;

    if(coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height)
        return tiles.at(coord.x).at(coord.y);
    else
        return Tile::Ptr(new Tile(Tile::BLANK,Square(coord.x,coord.y)));
}

unsigned int Map::getWidth() const
{
    return width;
}

unsigned int Map::getHeight() const
{
    return height;
}

void Map::print() const
{
    std::cout << "////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////"
              << std::endl;

    for(unsigned int x = 0; x < tiles.size(); ++x)
    {
        for(unsigned int y = 0; y < tiles.at(x).size(); ++y)
        {
            Tile::Ptr tile =  tiles.at(x).at(y);

            if(tile.get())
            {
                unsigned char tileChar = tile->getChar();
                std::cout << tileChar;
            }

            else
            {
                std::cout << " ";
            }
        }

        std::cout << std::endl;
    }

    std::cout << std::endl << std::endl;
}

void Map::addTileManual(NeuroManualObject::Ptr manualObject)
{
    tileManuals.push_back(manualObject);
}

void Map::setPlayer(Player::Ptr player)
{
    this->player = player;
}

Player::Ptr Map::getPlayer() const
{
    return player;
}

Tile::Ptr Map::getSelectedTile()
{
    return getTile(tile_hilight->getPos().toOgre());
}

MapFactory::MapFactory(int mapSizeX, int mapSizeY) :
    mapSizeX(mapSizeX),
    mapSizeY(mapSizeY)
{
    Neurocaster::getSingletonPtr()->current_map_x = mapSizeX;
    Neurocaster::getSingletonPtr()->current_map_y = mapSizeY;
}

void MapFactory::createSubSystemConnections(SubSystem::PtrArray2D& subSystems, double roomConnectionProbability, Square subSystemCoord)
{
    if(subSystems.size() == 0)
        return;

    SubSystem::Ptr subSystem = subSystems.at(subSystemCoord.getX()).at(subSystemCoord.getY());

    for(unsigned int x = 0; x < subSystems.size(); ++x)
    {
        for(unsigned int y = 0; y < subSystems.at(x).size(); ++y)
        {
            if(subSystems.at(x).at(y)->isGone())
                continue;

            if(x == subSystemCoord.getX() && y == subSystemCoord.getY())
                continue; // Dont try to connect rooms to themselves

            else if(neuro::util::rand_double() < roomConnectionProbability)
                subSystem->addConnectedSubSystem(Square(x, y));
        }
    }

    // If no connections were made, just choose a random connection
    while(subSystems.at(0).size() > 0 && subSystem->getConnectedSubsystems().size() == 0)
    {
        unsigned int x = neuro::util::rand_int(0, subSystems.size());
        unsigned int y = neuro::util::rand_int(0, subSystems.at(x).size());

        if(x != subSystemCoord.getX() || y != subSystemCoord.getY())
            subSystem->addConnectedSubSystem(subSystems.at(x).at(y)->getPos());
    }
}

void MapFactory::createSubSystemLayout(SubSystem::PtrArray2D& subSystems, double roomConnectionProbability)
{
    for(unsigned int x = 0; x < subSystems.size(); ++x)
    {
        for(unsigned int y = 0; y < subSystems.at(x).size(); ++y)
        {
            if(!subSystems.at(x).at(y)->isGone())
                createSubSystemConnections(subSystems, roomConnectionProbability, Square(x, y));
        }
    }
}

void MapFactory::createSubSystemPaths(SubSystem::PtrArray2D &subSystems, Map::Ptr tileMap, Square subSystemCoord)
{
    SubSystem::Ptr subSystem(subSystems.at(subSystemCoord.getX()).at(subSystemCoord.getY()));
    const std::vector<Square>& connectedSubSystems = subSystem->getConnectedSubsystems();

    for(unsigned int i = 0; i < connectedSubSystems.size(); ++i)
    {
        Square targetSubSystemCoord = connectedSubSystems.at(i);
        targetSubSystemCoord.setX((int) targetSubSystemCoord.getX() % subSystems.size());
        targetSubSystemCoord.setY((int) targetSubSystemCoord.getY() % subSystems.at(targetSubSystemCoord.getX()).size()); // confine to bounds
        SubSystem::Ptr targetSubSystem(subSystems.at(targetSubSystemCoord.getX()).at(targetSubSystemCoord.getY()));
        Ogre::Vector2 originPosition;
        Ogre::Vector2 targetPosition;

        if(subSystem->isGone())
            originPosition = subSystem->getPos().toOgre();
        else
            originPosition = subSystem->getPort1()->getPos().toOgre();

        if(targetSubSystem->isGone())
            targetPosition = targetSubSystem->getPos().toOgre();
        else
            targetPosition = targetSubSystem->getPort2()->getPos().toOgre();

        // Path find between ports
        neuro::astar::path_t path;

        if(neuro::astar::pathFind(tileMap, originPosition, targetPosition, Tile::MAPGEN, &path, neuro::astar::DiagonalHate, false, true))
        {

            // Set tiles to HALLWAY status
            for(unsigned int i = 0; i < path.size(); ++i)
            {
                // std::cout << "Hallway: (" << path.at(i).x << ", " << path.at(i).y << ")" << std::endl;
                Tile::Ptr tile = tileMap->getTile(path.at(i));
                if(tile.get())
                {
                    if(tile->getChar() == Tile::BLANK)
                        tileMap->setTile(path.at(i), Tile::HALLWAY);
                }
            }
        }

        if(subSystem->isGone())
            originPosition = subSystem->getPos().toOgre();
        else
            originPosition = subSystem->getPort2()->getPos().toOgre();

        if(targetSubSystem->isGone())
            targetPosition = targetSubSystem->getPos().toOgre();
        else
            targetPosition = targetSubSystem->getPort1()->getPos().toOgre();

        // Path find between ports
        path.clear();

        if(neuro::astar::pathFind(tileMap, originPosition, targetPosition, Tile::MAPGEN, &path, neuro::astar::DiagonalHate, false, true))
        {
            // Set tiles to HALLWAY status
            for(unsigned int i = 0; i < path.size(); ++i)
            {
                // std::cout << "Hallway: (" << path.at(i).x << ", " << path.at(i).y << ")" << std::endl;
                Tile::Ptr tile = tileMap->getTile(path.at(i));

                if(tile.get())
                {
                    if(tile->getChar() == Tile::BLANK)
                        tileMap->setTile(path.at(i), Tile::HALLWAY);
                }
            }
        }
    }
}

void MapFactory::createHallways(SubSystem::PtrArray2D& subSystems, Map::Ptr tileMap)
{
    tileMap->print();

    for(unsigned int x = 0; x < subSystems.size(); ++x)
    {
        for(unsigned int y = 0; y < subSystems.at(x).size(); ++y)
        {
            createSubSystemPaths(subSystems, tileMap, Square(x, y));
        }
    }

    tileMap->print();
}

Ogre::Vector2 MapFactory::generatePosition(SubSystem::Ptr subSystem)
{
    if(subSystem->isGone())
    {
        return Ogre::Vector2(subSystem->getPos().getX(), subSystem->getPos().getY());
    }

    else
    {
        Port::Ptr originPort;

        // Pick the origin port
        if(neuro::util::rand_double() > 0.5)
            originPort = subSystem->getPort1();
        else
            originPort = subSystem->getPort2();

        return Ogre::Vector2(originPort->getX(), originPort->getY());
    }
}

void MapFactory::generateTileManuals(Map::Ptr tileMap)
{
    for(unsigned int x = 0; x < tileMap->getWidth(); ++x)
    {
        for(unsigned int y = 0; y < tileMap->getHeight(); ++y)
        {
            Tile::Ptr tile = tileMap->getTile(Ogre::Vector2(x, y));

            switch((Tile::Type) tile->getChar())
            {
            case Tile::HALLWAY:
                tileMap->addTileManual(NeuroManualObject::Ptr(new neuro::Hallway(Square(x, y))));
                break;

            default:
                break;
            }
        }
    }
}

unsigned int Map::getSSWidth()
{
    return subSystems.size();
}

unsigned int Map::getSSHeight()
{
    return subSystems[0].size();
}

SubSystem::Ptr Map::getSS(int x, int y)
{
    return subSystems[x][y];
}
