#include "node.h"
#include "node_icons/nodeicon.h"
#include "core/neurocaster.h"

using namespace neuro;

Node::Node(Square pos, int nodeFlags) :
    Hackable(ENEMYNODE,pos),
    nodeFlags(nodeFlags)
{
}

//===================
//Bit Field Stuff
//===================
void Node::removePortFlag(NodeFlags flag)
{
    nodeFlags &= ~flag;

    if(nodeFlags & UserOwned)
    {
        character = Tile::NODE;
    }
    else
    {
        character = Tile::ENEMYNODE;
    }

    async_update_gui();
}

void Node::addPortFlag(NodeFlags flag)
{
    nodeFlags |= flag;

    if(nodeFlags & UserOwned)
    {
        character = Tile::NODE;
    }
    else
    {
        character = Tile::ENEMYNODE;
    }

    async_update_gui();
}

void Node::setPortFlags(int nodeFlags)
{
    this->nodeFlags = nodeFlags;

    if(nodeFlags & UserOwned)
    {
        character = Tile::NODE;
    }
    else
    {
        character = Tile::ENEMYNODE;
    }

    async_update_gui();
}

bool Node::containsFlag(NodeFlags flag)
{
    if(nodeFlags & flag) {
        return true;
    } else {
        return false;
    }
}

inline void Node::async_update_gui()
{
    boost::function<void ()> func = boost::bind(&Node::updateGui,this);
    Neurocaster::getSingletonPtr()->register_async_func(func);
}

void Node::updateGui()
{
    if(nodeFlags & UserOwned)
    {
        neuro_manuals["nodeIcon"]->setInvisible();
        neuro_manuals["playerNodeIcon"]->setVisible();
    }
    else
    {
        neuro_manuals["nodeIcon"]->setVisible();
        neuro_manuals["playerNodeIcon"]->setInvisible();
    }
}

void Node::changeType(NodeFlags flag)
{
    removePortFlag(IO);
    removePortFlag(Scanner);
    removePortFlag(Data);
    removePortFlag(Spawner);
    removePortFlag(Wetware);
    removePortFlag(Control);
    addPortFlag(flag);
}

//==================================
//Updatable Stuff
//==================================
void Node::continuous_update()
{

}

void Node::update_turn(double turn_number)
{

}

//==================================
//PolyNeuroManualObject Stuff
//==================================

class LockIcon : public NeuroManualObject
{
public:
    LockIcon() : NeuroManualObject(Square(),"LockIcon") {}
    void init_manual()
    {

    }
};

class HoneyPotIcon : public NeuroManualObject
{
public:
    HoneyPotIcon() : NeuroManualObject(Square(),"HoneyPotIcon") {}
    void init_manual()
    {

    }
};

class InfectionIcon : public NeuroManualObject
{
public:
    InfectionIcon() : NeuroManualObject(Square(),"InfectionIcon") {}
    void init_manual()
    {

    }
};

class CrashIcon : public NeuroManualObject
{
public:
    CrashIcon() : NeuroManualObject(Square(),"CrashIcon") {}
    void init_manual()
    {

    }
};

void Node::create_manuals()
{
    NeuroManualObject::Ptr nodeIcon(new NodeIcon(pos));
    NeuroManualObject::Ptr playerNodeIcon(new PlayerNodeIcon(pos));

//    NeuroManualObject::Ptr lockIcon = NeuroManualObject::Ptr(new LockIcon());
//    NeuroManualObject::Ptr honeyPotIcon = NeuroManualObject::Ptr(new HoneyPotIcon());
//    NeuroManualObject::Ptr infectionIcon = NeuroManualObject::Ptr(new InfectionIcon());
//    NeuroManualObject::Ptr crashIcon = NeuroManualObject::Ptr(new CrashIcon());

    neuro_manuals["nodeIcon"] = nodeIcon;
    neuro_manuals["playerNodeIcon"] = playerNodeIcon;
//    std::cout << neuro_manuals.size() << std::endl;
//    neuro_manuals["lockIcon"] = lockIcon;
//    neuro_manuals["honeyPotIcon"] = honeyPotIcon;
//    neuro_manuals["infectionIcon"] = infectionIcon;
//    neuro_manuals["crashIcon"] = crashIcon;
    init_manuals();
//    playerNodeIcon->setInvisible();
}
