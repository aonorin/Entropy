#include "hero.h"
#include "math.h"
#include "neuro/Global.h"
#include "boost/foreach.hpp"
#include "src/Core/terminal.h"
#include "OGRE/Ogre.h"
#include "src/Core/terminal.h"
#include "neuro/program.h"

namespace neuro{

Hero::Hero() :
    speed(0.065),
    fx(0),
    fy(0),
    health(100),
    suspicion(0),
    progress(0),
    invisible(false)
{
    this->c = HERO;
    status = EXPLORED;
    facing = DOWN;
}


Hero::Hero(int c,int x, int y) :
    speed(0.05),
    health(1),
    suspicion(0),
    progress(0),
    invisible(false)
{
    this->c = c;
    fx = x;
    fy = y;
    status = EXPLORED;
}

Hero::~Hero()
{
    for(int i=0;i<projectiles.size();i++) {
        delete projectiles.at(i);
    }
//    std::cout << "DELETING HERO" << std::endl;
    std::pair<int,boost::shared_ptr<Program> > programPair;
    BOOST_FOREACH(programPair,activePrograms) {
//        std::cout << "Hero deleting Program: "<<programPair.second->getID() << std::endl;
        programPair.second->deleteResources();
        programPair.second.reset();
    }
    programPair.second.reset();
}

void Hero::setPos(Vec2i pos)
{
    fx = pos.x;
    fy = pos.y;
}

Vec2i Hero::getPos()
{
    Vec2i pi;
    pi.x = (int)(fx+0.5);
    pi.y = (int)(fy+0.5);
    return pi;
}

float Hero::getSpeed()
{
    return speed;
}

int Hero::getX()
{
    return (int)(fx+0.5);
}

int Hero::getY()
{
    return (int)(fy+0.5);
}

void Hero::setX(int x)
{
    this->fx = x;
}

void Hero::setY(int y)
{
    this->fy = y;
}

void Hero::gravity()
{
    //    inertiaY+=speed*0.012;
    inertiaY=std::min(inertiaY,speed);
    inertiaY=std::max(inertiaY,-speed);
    //    std::cout << inertiaX << std::endl;
    //    fy+=inertiaY;
    //    fx+=inertiaX;
}

void Hero::jump()
{
    inertiaY-=0.3;
    inertiaY=std::min(inertiaY,speed);
    //    std::cout << inertiaY << std::endl;
    //    fy+=inertiaY;
}

void Hero::moveUp()
{
    fy+=speed;
    //    inertiaY+=speed*0.05;
    //    inertiaY=std::min(inertiaY,speed);
    //    inertiaY=std::max(inertiaY,-speed);
    facing = UP;
}

void Hero::moveDown()
{
    fy-=speed;
    //    inertiaY-=speed*0.05;
    //    inertiaY=std::min(inertiaY,speed);
    //    inertiaY=std::max(inertiaY,-speed);
    facing = DOWN;
}

void Hero::moveLeft()
{
    fx-=speed;
    //    inertiaX=std::min(inertiaX,speed);
    //    inertiaX=std::max(inertiaX,-speed);
    //    inertiaX-=speed*0.05;
    facing = LEFT;
}

void Hero::moveRight()
{
    fx+=speed;
    //    inertiaX=std::min(inertiaX,speed);
    //    inertiaX=std::max(inertiaX,-speed);
    //    inertiaX+=speed*0.05;
    facing = RIGHT;
}

float Hero::getFX()
{
    return fx;
}

float Hero::getFY()
{
    return fy;
}

void Hero::setFX(float fx)
{
    this->fx = fx;
}

void Hero::setFY(float fy)
{
    this->fy = fy;
}

void Hero::setFPos(Vec2f fPos)
{
    fx = fPos.x;
    fy = fPos.y;
}

Vec2f Hero::getFPos()
{
    Vec2f fPos(fx,fy);
    return fPos;
}

//BARS

float Hero::getHealth()
{
    return health;
}

void Hero::damageHealth(float damage)
{
    health-=damage;
    if(0.45*health > 0) {
        if(Terminal::getSingletonPtr()->hacking)
            Terminal::getSingletonPtr()->healthBar->setWidth(0.45*health);
    } else {
        if(Terminal::getSingletonPtr()->hacking)
            Terminal::getSingletonPtr()->healthBar->setWidth(0.01);
    }
}

void Hero::healHealth(float heal)
{
    health+=heal;
}

float Hero::getSuspicion()
{
    return suspicion;
}

void Hero::addSuspicion(float addedS)
{
    suspicion = std::min(suspicion+addedS,1.0f);
}

void Hero::reduceSuspicion(float reduceS)
{
    suspicion-=reduceS;
}

float Hero::getProgress()
{
    return progress;
}

boost::shared_ptr<Program> Hero::initProgram(Program::ProgramType program, boost::shared_ptr<Square> programSquare)
{
    bool found = false;
    //    programID++;
    int programID = Program::getProgramID();
    std::pair<int,boost::shared_ptr<Program> > programPair;
    BOOST_FOREACH(programPair,activePrograms) {
        if(
                (programSquare->getPos().x==programPair.second->getPos().x)&&
                (programSquare->getPos().y==programPair.second->getPos().y)
                ) {
            found = true;
        }
    }
    if(!found) {
        boost::shared_ptr<Program> newProgram(new Program(program,programSquare->getPos(),programID));
        activePrograms[programID] = newProgram;
        addSuspicion(newProgram->getSuspiciousness());
        return newProgram;
    } else {
        boost::shared_ptr<Program> newProgram;
        return newProgram;
    }
}

void Hero::endProgram(int id)
{
    try{
        //        delete activePrograms[id];
        activePrograms[id].reset();
        activePrograms.erase(id);
    } catch(...) {

    }
}

//Program* Hero::getProgram(int id)
boost::shared_ptr<Program> Hero::getProgram(int id)
{
    return activePrograms[id];
}

int Hero::getFacing()
{
    return facing;
}

void Hero::setFacing(int facing)
{
    this->facing = facing;
}

void Hero::attack()
{
    projectiles.push_back(new Projectile(Vec2f(fx,fy),0,(Direction) facing,Program::getProgramID()));
}

void Hero::updateProjectiles()
{
    std::vector<int> deadProjs;
    for(int i=projectiles.size()-1;i>=0;i-=1) {
        projectiles.at(i)->move();
        if(!checkMapBounds(projectiles.at(i)->getPos())) {
            deadProjs.push_back(i);
        }
    }
    for(int i=0;i<deadProjs.size();i++) {
        //        delete projectiles.at(deadProjs.at(i));
        projectiles.erase(projectiles.begin()+deadProjs.at(i));
    }
}

void Hero::setInvisible()
{
    if(Terminal::getSingletonPtr()->hacking)
        Terminal::getSingletonPtr()->heroBox->setMaterialName("InvisibleHero");
    this->invisible = true;
}

void Hero::setVisible()
{
    if(Terminal::getSingletonPtr()->hacking)
        Terminal::getSingletonPtr()->heroBox->setMaterialName("heroMat");
    this->invisible = false;
}

bool Hero::isInvisible()
{
    return invisible;
}

void Hero::invisDamage(float prg)
{
    damageHealth(0.0005);
}
}
