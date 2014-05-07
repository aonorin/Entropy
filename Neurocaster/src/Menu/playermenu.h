#ifndef PLAYERMENU_H
#define PLAYERMENU_H

#include "menu.h"
#include "src/gameplay/player.h"

namespace gui
{

class PlayerMenu : public Gui
{
public:
    PlayerMenu(const Ogre::Vector2& min, const Ogre::Vector2& max);

    virtual void show();
    virtual void update();

private:

    neuro::Player* player;
};


} // gui namespace

#endif // PLAYERMENU_H
