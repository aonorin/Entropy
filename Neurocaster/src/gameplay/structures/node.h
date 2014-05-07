#ifndef NODE_H
#define NODE_H

#include "port.h"
#include "hackable.h"
#include "gameplay/square.h"
#include "gameplay/polyneuromanualobject.h"

namespace neuro
{

class Node : public Hackable, public PolyNeuroManualObject
{
public:
    typedef boost::shared_ptr<Node> Ptr;
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
    Node(Square pos, int nodeFlags);

    void reveal();

    void setPortFlags(int nodeFlags);
    void addPortFlag(NodeFlags flag);
    void removePortFlag(NodeFlags flag);
    int getPortFlags();
    bool containsFlag(NodeFlags flag);
    void changeType(NodeFlags flag);

    void captured();
    void encrypt();
    void crash();
    void honeyPot();
    void unHoneyPot();
    void enemyInfect();

    //Need to handle Respawns

    void continuous_update();
    void update_turn(double turn_number);
    void update_action() {}
    void complete_action() {}

    void create_manuals();

protected:
    int nodeFlags;
    void async_update_gui();
    void updateGui();
};

}

#endif // NODE_H
