#pragma once

#include "neuro/square.h"
#include <vector>
#include "boost/pointer_cast.hpp"
#include "boost/multi_array.hpp"
#include "neuro/program.h"
#include "boost/enable_shared_from_this.hpp"

namespace Ogre {
    class TextAreaOverlayElement;
}

namespace neuro {
typedef boost::multi_array<int, 2> WalkArray;
class Room;
class Hero;
class AStar;
class Phantom;
class BackTracer;

typedef boost::multi_array<boost::shared_ptr<Square>, 2> SquareArray;

class Map : public boost::enable_shared_from_this<Map>
{
public:
    Map(int gsX,int gsY, int msX, int msY);
    ~Map();
    void moveHero(int ch);
    boost::shared_ptr<Room> getRoom(int x,int y) ;
    boost::multi_array<boost::shared_ptr<Square>, 2> getTerrainMap();
    Vec2f getHeroPos();
    Vec2i getVirusPos();
    int getGridSizeX();
    int getGridSizeY();
    void setSquareID(int x,int y,int id);
    boost::shared_ptr<Square> getSquare(int x,int y);
    int getMapSizeX();
    int getMapSizeY();
    boost::shared_ptr<Hero> getHero();
    void initProgram(Program::ProgramType program);
    void initProgram1();
    void initProgram2();
    void initProgram3();
    void initProgram4();
    int testProgram(int num);
    boost::shared_ptr<Square> getProgramPos();
    void programCompleted(int id);
    void updateAI();
    void deletePhantom(int id);
    void checkGameStatus();
    void gameOver();
    void gameWon();
    int getGameState();
    void respawnPhantom(int oneDPos);
    bool honeyPotActive;
    void unHoneyPot();
    boost::shared_ptr<AStar> getAStar();
    boost::shared_ptr<BackTracer> getBackTracer();
    void deleteResources();

private:
    Ogre::TextAreaOverlayElement* gameEnd;
    bool resourcesDeleted;
    int gridSizeX,gridSizeY,mapSizeX,mapSizeY;
    int sx,sy;
    boost::multi_array<boost::shared_ptr<Room>, 2> rooms;
    SquareArray terrainMap;
    int gameState;
    boost::shared_ptr<Hero> hero;
    std::vector<Square> enemies;
    std::vector<Square> objects;
    void createRooms();
    void connectRooms();
    void connectRooms2();
    void createHallways();
    void testConnectedness();
    void updateMap();
    int pathCount;
    int pathIter;
    int roomID;
    int numGone;
    std::vector<boost::shared_ptr<Phantom> > phantoms;
    WalkArray hallWalkability;
    WalkArray enemyWalkability;
    void convertToHallWalkMap();
    void convertToEnemyWalkMap();
    bool setup;
    boost::shared_ptr<AStar> aStar;
    boost::shared_ptr<BackTracer> backTracer;
};

}
