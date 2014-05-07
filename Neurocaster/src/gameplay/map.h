#ifndef MAP_H
#define MAP_H

//system
#include <vector>

//boost
//#include "boost/pointer_cast.hpp"
//#include "boost/multi_array.hpp"
//#include "boost/enable_shared_from_this.hpp"

// Ogre
#include "OGRE/OgreVector2.h"

//neuro
#include "square.h"
#include "neuroai.h"
#include "structures/hackable.h"
#include "structures/subsystem.h"
#include "core/scene.h"
#include "grid.h"
#include "player.h"
#include "tilehilight.h"

namespace neuro
{

//typedef std::vector< std::vector< Tile::Ptr > > TileMultiArray;
//typedef std::vector< std::vector< SubSystem::Ptr > > SubSystemMultiArray;

class Map : public Scene
{
public:

    typedef boost::shared_ptr<neuro::Map> Ptr;
    typedef std::vector<Ptr> PtrArray;

    Map();

    void continuous_update();
    void update_turn(double turn_number);
    void update_action() {}
    void complete_action() {}
    void new_round();

    void init();

    //Map creation methods
    void setTiles(Tile::PtrArray2D tiles);
    void setTile(const Ogre::Vector2& coord, Tile::Type type);
    void setAI(NeuroAI::PtrArray ai);
    void setHackables(Hackable::PtrArray hackables);
    void setSubSystems(SubSystem::PtrArray2D subSystems);
    void addTileManual(NeuroManualObject::Ptr manualObject);
    void setPlayer(Player::Ptr player);
    Player::Ptr getPlayer() const;

    // Access methods
    Tile::Ptr getTile(const Ogre::Vector2& coord) const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getSSHeight();
    unsigned int getSSWidth();
    SubSystem::Ptr getSS(int x, int y);

    void print() const; // Prints a text only version to the std out stream

    TileHilight::Ptr tile_hilight;

    Tile::Ptr getSelectedTile();

protected:
    Tile::PtrArray2D tiles;
    NeuroManualObject::PtrArray tileManuals; // Manual objects for the specific tiles
    NeuroAI::PtrArray ai;
    Hackable::PtrArray hackables;
    SubSystem::PtrArray2D subSystems;
    Player::Ptr player;
    Grid::Ptr grid;
    unsigned int width, height;
};

class MapFactory
{
public:
    MapFactory(int mapSizeX, int mapSizeY);
    int mapSizeX;
    int mapSizeY;
    virtual Map::Ptr createMap() = 0;

protected:

    // Subclass calls this to reate the subsystem connections, stored inside each subsystem
    void createSubSystemLayout(SubSystem::PtrArray2D& subSystems, double roomConnectionProbability = 0.05);
    // create the hallways in the tile map between the rooms
    void createHallways(SubSystem::PtrArray2D& subSystems, Map::Ptr tileMap);
    void generateTileManuals(Map::Ptr tileMap); // Reads through the map and populates the tileManuals array with instances of the NeuroManualObject subclasses

private:

    // internal helper function for createSubSystemLayout
    void createSubSystemConnections(SubSystem::PtrArray2D& subSystems, double roomConnectionProbability, Square subSystemCoord);
    // internal helper function for createHallways
    void createSubSystemPaths(SubSystem::PtrArray2D& subSystems, Map::Ptr tileMap, Square subSystemCoord);
    Ogre::Vector2 generatePosition(SubSystem::Ptr subSystem);
};

}

#endif // MAP_H
