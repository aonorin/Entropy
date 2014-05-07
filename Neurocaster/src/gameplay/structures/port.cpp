#include "port.h"
#include "port_icons/porticon.h"
#include "core/neurocaster.h"

using namespace neuro;

Port::Port(Square pos, int portFlags) :
    Hackable(ENEMYPORT,pos),
    portFlags(portFlags)
{

}

//===================
//Bit Field Stuff
//===================
void Port::removePortFlag(PortFlags flag)
{
    portFlags &= ~flag;

    if(portFlags & EnemyOwned)
    {
        character = Tile::ENEMYPORT;
    }
    else if(portFlags & UserOwned)
    {
        character = Tile::PORT;
    }

    async_update_gui();
}

void Port::addPortFlag(PortFlags flag)
{
    portFlags |= flag;

    if(portFlags & EnemyOwned)
    {
        character = Tile::ENEMYPORT;
    }
    else if(portFlags & UserOwned)
    {
        character = Tile::PORT;
    }

    async_update_gui();
}

void Port::setPortFlags(int portFlags)
{
    this->portFlags = portFlags;

    if(portFlags & EnemyOwned)
    {
        character = Tile::ENEMYPORT;
    }
    else if(portFlags & UserOwned)
    {
        character = Tile::PORT;
    }

    async_update_gui();
}

inline void Port::async_update_gui()
{
    boost::function<void ()> func = boost::bind(&Port::updateGui,this);
    Neurocaster::getSingletonPtr()->register_async_func(func);
}

void Port::updateGui()
{
    if(portFlags & EnemyOwned)
    {
        character = Tile::ENEMYPORT;
        neuro_manuals["portIcon"]->setVisible();
        neuro_manuals["playerPortIcon"]->setInvisible();
    }
    else if(portFlags & UserOwned)
    {
        character = Tile::PORT;
        neuro_manuals["portIcon"]->setInvisible();
        neuro_manuals["playerPortIcon"]->setVisible();
    }
}

bool Port::containsFlag(PortFlags flag)
{
    if(portFlags & flag) {
        return true;
    } else {
        return false;
    }
}

void Port::continuous_update()
{
    Hackable::continuous_update();
}

void Port::update_turn(double turn_number)
{
    Hackable::update_turn(turn_number);
}

void Port::create_manuals()
{
    NeuroManualObject::Ptr portIcon(new PortIcon(pos));
    NeuroManualObject::Ptr playerPortIcon(new PlayerPortIcon(pos));

    neuro_manuals["portIcon"] = portIcon;
    neuro_manuals["playerPortIcon"] = playerPortIcon;
    init_manuals();

//    playerPortIcon->setInvisible();
}

