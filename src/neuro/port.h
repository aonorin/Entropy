#pragma once

#include "neuro/square.h"
#include "boost/shared_ptr.hpp"

namespace Ogre {
    class OverlayContainer;
}

namespace neuro {

class Room;

class Port : public Square
{
public:

    enum PortFlags {
        Open        = 1,
        Closed      = 2,
        UserOwned   = 4,
        EnemyOwned  = 8,
        Trojaned    = 16,
        Infected    = 32,
        Daemoned    = 64,
        Alarmed     = 128,
        BlackICED    = 256,
        Hacking     = 512
    };

    Port();
    Port(Vec2i pos,int portFlags);
    ~Port();

    //Flags
    void setPortFlags(int portFlags);
    void addPortFlag(PortFlags flag);
    void removePortFlag(PortFlags flag);
    int getPortFlags();
    bool containsFlag(PortFlags flag);

    //GUI stuff
    void setupGUI();
    void updateGUI();
    Ogre::OverlayContainer* portIcon;
    Ogre::OverlayContainer* daemonIcon;
    Ogre::OverlayContainer* alarmIcon;
    Ogre::OverlayContainer* trojanIcon;
    Ogre::OverlayContainer* blackICEIcon;
    void setActive();

    //TimedEvent Functions
    void portToEnemyPort();
    void enemyPortToPort();
    void activatePrograms();
    void setParent(boost::shared_ptr<Room> parent);
    boost::shared_ptr<Room> getParent();
    void deactivateAlarm();
    void explodeDaemon();
    void deleteResources();

private:
    int portFlags;
    bool active;
    boost::shared_ptr<Room> parent;
    bool setup;

};

}
