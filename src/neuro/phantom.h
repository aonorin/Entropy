#ifndef PHANTOM_H
#define PHANTOM_H

#include "neuro/neuroai.h"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace Ogre {
    class OverlayElement;
}

namespace neuro {

class Program;
class Map;

class Phantom : public boost::enable_shared_from_this<Phantom>, public NeuroAI
{
public:
    Phantom(Vec2f pos, int id, int status, int metaStatus,Map* map);
    ~Phantom();
    void attack();
    void updateProjectiles();
    void update();
    void scan();
    void initProgram(int programType);
    void endProgram(int programType);
    void setTargetID(int targetID);
    void chooseTarget();
    void setNodePos(Vec2i nodePos);
    Vec2i getNodePos();
    void setupPhantomGUI();


private:
    Vec2i scanPos;
    Vec2i nodePos;
    bool guiSetup;
    int scanTimer;
    int scanNum;
    int scanRate;
    Ogre::OverlayElement* scanIcon;
    Ogre::OverlayElement* targetIcon;
    Vec2i targetPos;
    Map* map;
    boost::shared_ptr<Program> program;

};

}

#endif // PHANTOM_H
