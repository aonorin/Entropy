#include "backtracer.h"
#include "src/Core/terminal.h"
#include <boost/lexical_cast.hpp>
#include "src/Core/Entropy.h"
#include "neuro/map.h"
#include "neuro/program.h"
#include "neuro/astar.h"
#include "OGRE/OgreOverlayElement.h"
#include "OGRE/OgreOverlayManager.h"
#include "OGRE/OgreOverlayContainer.h"

namespace neuro {

BackTracer::BackTracer(Vec2i pos, Map *map,int ioID):
    pos(pos),
    map(map),
    id(map->getAStar()->registerMover(pos,1)),
    traceState(Dormant),
    counter(0),
    ioID(ioID),
    idCounter(0)
{
}

BackTracer::~BackTracer()
{
    for(int i=0;i<tracers.size();i++) {
        Entropy::getSingletonPtr()->addQeueudDeleteOverlays(tracers.at(i));
    }
    tracers.clear();
}

bool BackTracer::update()
{
    if(traceState==Tracing) {
        counter++;
        if(counter>120) {
            map->getAStar()->setPos(id,pos);
            Vec2i tPos = map->getAStar()->move(id,ioID,1);
            if(!(tPos.x==0&&tPos.y==0)) {
                if(checkMapBounds(tPos)) {
//                    if(map->enemyWalkability[tPos.x][tPos.y]==Walkable) {
                        pos = tPos;
                        boost::function<void (void)> func = boost::bind(&BackTracer::setupGUI,this);
                        Entropy::getSingletonPtr()->addQeueudFunction(func);
//                    }
                }
            }
            counter=0;
        }
    }
    if(pos.x == map->getAStar()->getPos(ioID).x&&pos.y == map->getAStar()->getPos(ioID).y) {
        traceState = Found;
    }

    if(traceState==Found) {
        return true;
    } else {
        return false;
    }
}

void BackTracer::setupGUI()
{
    Ogre::OverlayElement* tracer = Terminal::getSingletonPtr()->_overlayMgr->createOverlayElement("Panel", "BackTracer"+boost::lexical_cast<std::string>(idCounter++));
    tracer->setMetricsMode(Ogre::GMM_PIXELS);
    tracer->setPosition(pos.x*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25, pos.y*Terminal::getSingletonPtr()->pixelSize+Terminal::getSingletonPtr()->pixelSize*0.25);
    tracer->setWidth(Terminal::getSingletonPtr()->pixelSize*0.5);
    tracer->setHeight(Terminal::getSingletonPtr()->pixelSize*0.5);
    tracer->setParameter("uv_coords","0 0 1 1");
    tracer->setMaterialName("ScanIcon");
    Terminal::getSingletonPtr()->_panel3->addChild(tracer);
    tracers.push_back(tracer);
}

void BackTracer::activate()
{
    if(traceState==Dormant) {
        traceState = Tracing;
    }
}

void BackTracer::deactivate()
{
    traceState = Disabled;
}

void BackTracer::reactivate()
{
    if(traceState=Disabled) {
        traceState = Dormant;
    }
}

}
