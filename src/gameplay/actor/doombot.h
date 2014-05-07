#ifndef DOOMBOT_H
#define DOOMBOT_H

#include "gameplay/actor/enemyactor.h"
#include "OGRE/OgreVector3.h"

namespace hypnos {

class DoomBot : public gameplay::actor::EnemyActor
{
public:
    DoomBot(Ogre::Vector3 pos);
    void move(Ogre::Vector3 nextPosition);
    static void constructDoomBot();
private:

};

}

#endif // DOOMBOT_H
