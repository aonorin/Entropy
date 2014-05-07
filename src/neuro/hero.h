#ifndef HERO_H
#define HERO_H

#ifdef WIN32
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif

#include "neuro/square.h"
#include "neuro/program.h"
#include "vector"
#include "boost/signal.hpp"
#include "boost/bind.hpp"

#include "neuro/projectile.h"

namespace neuro{

class Projectile;
class Program;
class Hero
{
public:
    Hero();
    Hero(int c,int x,int y);
    ~Hero();
    void move(int ch);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    //Position Date
    void setFPos(Vec2f fPos);
    void setFX(float fx);
    void setFY(float fy);
    float getFX();
    float getFY();
    Vec2f getFPos();
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    void setPos(Vec2i pos);
    Vec2i getPos();
    int id;
    float getSpeed();
    void gravity();
    void jump();

    //H/S/P Bars
    float getHealth();
    void damageHealth(float damage);
    void healHealth(float heal);
    float getSuspicion();
    void addSuspicion(float addedS);
    void reduceSuspicion(float reduceS);
    float getProgress();

    //Programs
    //    Program* initProgram(Program::ProgramType program, Square* programSquare);
    //    Program* getProgram(int id);
    boost::shared_ptr<Program> initProgram(Program::ProgramType program, boost::shared_ptr<Square> programSquare);
    boost::shared_ptr<Program> getProgram(int id);
    void endProgram(int id);
    void setFacing(int facing);
    int getFacing();
    void attack();
    void updateProjectiles();
    void setInvisible();
    void setVisible();
    bool isInvisible();
    void invisDamage(float prg);

private:
    float health;
    float suspicion;
    float progress;
    bool invisible;
    int pixelSize;
    float speed;
    float inertiaX;
    float inertiaY;
    float fx;
    float fy;
    int c;
    int facing;
    STATUS status;
    std::vector<Projectile* > projectiles;
    std::tr1::unordered_map<int,boost::shared_ptr<Program> > activePrograms;
};

}

#endif // HERO_H
