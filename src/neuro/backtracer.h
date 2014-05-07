#ifndef BACKTRACER_H
#define BACKTRACER_H

#include <vector>
#include "neuro/square.h"
namespace Ogre {
class OverlayElement;
}

namespace neuro {

class Map;
class Program;

class BackTracer
{
public:
    enum TraceState{Dormant,Tracing,Disabled,Found};
    BackTracer(Vec2i pos,Map* map,int ioID);
    ~BackTracer();
    void activate();
    void deactivate();
    void reactivate();
    bool update();

private:
    Vec2i pos;
    //    boost::shared_ptr<Map> map;
    Map* map;
    int id;
    int ioID;
    TraceState traceState;
    int counter;
    int idCounter;
    void setupGUI();
    std::vector<Ogre::OverlayElement*> tracers;
};

}


#endif // BACKTRACER_H
