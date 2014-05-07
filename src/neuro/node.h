#ifndef NODE_H
#define NODE_H

#include "neuro/square.h"
#include "boost/shared_ptr.hpp"

namespace Ogre {
    class OverlayContainer;
}

namespace neuro {

class Room;
class Program;

class Node : public Square
{
public:
    enum NodeFlags {
        UserOwned   = 1,
        Encrypted   = 2,
        Infected    = 4,
        HoneyPot    = 8,
        IPSpoofed   = 16,
        Crashed     = 32,
        IO          = 64,
        Scanner     = 128,
        Data        = 256,
        Spawner     = 512,
        Wetware     = 1024,
        Control     = 2048,
        Hacking     = 4096
    };
    Node(Vec2i pos,int nodeFlags );
    ~Node();
    void reveal();

    void setPortFlags(int nodeFlags);
    void addPortFlag(NodeFlags flag);
    void removePortFlag(NodeFlags flag);
    int getPortFlags();
    bool containsFlag(NodeFlags flag);
    void captured();
    void encrypt();
    void crash();
    void honeyPot();
    void unHoneyPot();
    void setParent(boost::shared_ptr<Room> parent);
    void enemyInfect();
    boost::shared_ptr<Room> getParent();
    void startRespawner(boost::shared_ptr<Program> program);
    void stopRespawner();
    bool isRespawnActive();
    void updateRespawner(float progress);
    void changeType(NodeFlags flag);
    void deleteResources();

private:
    int nodeFlags;
    Ogre::OverlayContainer* nodeIcon;
    Ogre::OverlayContainer* lockIcon;
    Ogre::OverlayContainer* honeyPotIcon;
    Ogre::OverlayContainer* infectionIcon;
    Ogre::OverlayContainer* crashIcon;
    boost::shared_ptr<Room> parent;
    boost::shared_ptr<Program> program;

    void updateGUI();
    void setupGUI();
};

}

#endif // NODE_H
