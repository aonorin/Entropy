#include "actor.h"

using namespace neuro;

Actor::Actor(Square pos, std::string name, Tile::Type type, double speed) :
    Entity(),
    NeuroManualObject(pos,name)
{

}

void Actor::continuous_update()
{

}

void Actor::update_turn(double turn_number)
{

}

void Actor::move()
{

}

void Actor::setSpeed(double speed)
{
    this->speed = speed;
}
