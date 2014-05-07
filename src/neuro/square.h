#pragma once

//#include "iostream"

namespace neuro {

enum STATUS{UNEXPLORED=100,EXPLORED=101,OCCUPIED=102};
enum OBJECT {
    BLANK=32,FLOOR='-',HALLWAY='o',CORNER=',',TOPCORNER=',',SIDEWALL=':',TOPWALL='.',HERO='@',VIRUS='*',
    IO = 0,SCANNER=1,DATA=2,CONTROL=3,SPAWNER=4,WETWARE=5,ENEMYPORT=6,PORT=7,NODE,ENEMYNODE

};
class Vec2i {
public:
    Vec2i(int x,int y) {
        this->x = x;
        this->y = y;
    }
    Vec2i(){}
    int x;
    int y;
//    void print() {
//        std::cout << "["<<x<<","<<y<<"]"<<std::endl;
//    }
};

class Vec2f {
public:
    Vec2f(float x,float y) {
        this->x = x;
        this->y = y;
    }
    Vec2f(){}
    float x;
    float y;
//    void print() {
//        std::cout << "FLOAT["<<x<<","<<y<<"]"<<std::endl;
//    }
};

static int squareIDs = 0;

class Square
{
public:
    Square();
    Square(int c,int x,int y);
    int getOptions();
    int getChar();
    void draw();
    int getX();
    int getY();
    void getEntity();
    void removeEntity();
    void addEntity(int entity);
    void setX(int x);
    void setY(int y);
    void setPos(Vec2i pos);
    Vec2i getPos();
    void setStatus(STATUS status);
    STATUS getStatus();
    char getDrawChar();
    int id;
    void setChar(int c);

protected:
    int c;
    int x,y;
    Vec2i pos;
    int entity;
    STATUS status;
};

}
