#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#ifdef __APPLE__
#include "boost/shared_ptr.hpp"
#else
//#include <memory>
#endif

#include "port.h"
#include "node.h"
#include "gameplay/neuron.h"

namespace neuro
{

//typedef std::vector< std::vector< Tile::Ptr > > TileMultiArray;

class SubSystem : public NeuroManualObject
{
public:

    typedef boost::shared_ptr<SubSystem> Ptr;
    typedef std::vector<Ptr> PtrArray;
    typedef std::vector<PtrArray> PtrArray2D;

    static const unsigned int WIDTH = 5; // default width
    static const unsigned int HEIGHT = 5; // default height

    SubSystem(unsigned int width = WIDTH, unsigned int height = HEIGHT);

    void setGone(bool gone);
    bool isGone();

    void setConnected(bool connected);
    bool isConnected();

    void setPort1(Port::Ptr port1);
    void setPort2(Port::Ptr port2);
    void setNode(Node::Ptr node);

    Port::Ptr getPort1();
    Port::Ptr getPort2();
    Node::Ptr getNode();
    Tile::PtrArray2D::iterator getTilesBegin();
    Tile::PtrArray2D::iterator getTilesEnd();
    Tile::Ptr getTile(int x, int y);

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void setTile(int x, int y, Tile::Ptr tile);

    NeuroManualObject::Ptr outline;

    void addConnectedSubSystem(Square coord);
    const std::vector<Square>& getConnectedSubsystems() const;

    void init_manual();

    void set_starting();
    bool isStarting();

protected:
//    Square pos;

    bool gone;
    bool connected;
    Port::Ptr port1;
    Port::Ptr port2;
    Node::Ptr node;

    unsigned int width, height;
    Tile::PtrArray2D tiles;
    std::vector<Square> connectedSubSystems; // SubSystem::PtrArray2D coordinates for connected rooms
    std::vector<std::vector<int> >  connectedMap;

    bool starting_sub_system;
};

}

#endif // SUBSYSTEM_H
