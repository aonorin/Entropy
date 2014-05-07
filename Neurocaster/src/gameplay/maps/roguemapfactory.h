#ifndef ROGUEMAPFACTORY_H
#define ROGUEMAPFACTORY_H

#include "gameplay/map.h"
#include "gameplay/player.h"

namespace neuro
{

class RogueMapFactory : public MapFactory
{
public:
    RogueMapFactory();
    Map::Ptr createMap();

private:
    int gridSizeX;
    int gridSizeY;
    int numGone;

    void initTiles(Tile::PtrArray2D& tiles);
    void createSubSystems(SubSystem::PtrArray2D &subSystems);
    void initSubSystem(SubSystem::Ptr subSystem, int gridX, int gridY);
    void addSubSystemTilesToMap(SubSystem::PtrArray2D &subSystems,Tile::PtrArray2D& tiles);
    void setStartingSubSystem(SubSystem::PtrArray2D &subSystems,Tile::PtrArray2D& tiles,Player::Ptr player,NeuroAI::PtrArray& ai);
    void createPorts(SubSystem::Ptr subSystem);
};

}

#endif // ROGUEMAPFACTORY_H
