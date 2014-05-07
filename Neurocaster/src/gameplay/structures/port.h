#ifndef PORT_H
#define PORT_H

#include "hackable.h"
#include "gameplay/square.h"
#include "gameplay/polyneuromanualobject.h"

namespace neuro
{

class NeuroManualObject;

class Port : public Hackable, public PolyNeuroManualObject
{
public:
    typedef boost::shared_ptr<Port> Ptr;

    enum PortFlags {
        Open        = 1,
        Closed      = 2,
        UserOwned   = 4,
        EnemyOwned  = 8,
        Trojaned    = 16,
        Infected    = 32,
        Daemoned    = 64,
        Alarmed     = 128,
        BlackICED   = 256,
        Hacking     = 512
    };
    Port(Square pos, int portFlags);

    //Flags
    void setPortFlags(int portFlags);
    void addPortFlag(PortFlags flag);
    void removePortFlag(PortFlags flag);
    int getPortFlags();
    bool containsFlag(PortFlags flag);

    void continuous_update();
    void update_turn(double turn_number);
    void update_action() {}
    void complete_action() {}

    void create_manuals();

protected:
    int portFlags;
    void async_update_gui();
    void updateGui();

    //Graphics
//    NeuroManualObject::Ptr portIcon;
//    NeuroManualObject::Ptr daemonIcon;
//    NeuroManualObject::Ptr alarmIcon;
//    NeuroManualObject::Ptr trojanIcon;
//    NeuroManualObject::Ptr blackIceIcon;
};

}

#endif // PORT_H
