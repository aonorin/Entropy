#ifndef TILE_H
#define TILE_H

#include "square.h"
#include "src/gameplay/entity.h"

namespace neuro
{

class Tile : public Entity
{
public:
    enum Loc_Status{UNEXPLORED=100,EXPLORED=101,OCCUPIED=102};
    enum Type {
        BLANK=32,FLOOR='-',HALLWAY='o',CORNER=',',TOPCORNER=',',SIDEWALL=':',TOPWALL='.',HERO='@',VIRUS='*',
        IO = 0,SCANNER=1,DATA=2,CONTROL=3,SPAWNER=4,WETWARE=5,ENEMYPORT='p',PORT='P',NODE='N',ENEMYNODE='n', MAPGEN, /* Used for map generation */

        //Enemies
        SENTRY = 1000
    };

    typedef boost::shared_ptr<Tile> Ptr;
    typedef std::vector<Ptr> PtrArray;
    typedef std::vector< PtrArray > PtrArray2D;
    Tile(int character,Square pos);

    int getChar();
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    void setPos(Square pos);
    Square getPos();
    void setStatus(Loc_Status status);
    Loc_Status getStatus();
    void setChar(int character);

protected:
    int character;
    int x,y;
    Square pos;
    Loc_Status status;
};

}

#endif // TILE_H
