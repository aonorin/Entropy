#include <sstream>

#include "hallway.h"
#include "core/Global.h"
#include "boost/function.hpp"
#include "core/neurocaster.h"

namespace neuro
{

std::string generateHallwayName(const Square& pos)
{
    std::stringstream ss;
    ss << "Hallway" << pos.x << "," << pos.y;
    return ss.str();
}

Hallway::Hallway(const Square& pos) :
    NeuroManualObject(pos, generateHallwayName(pos))
{
    async_init_manual();
}

void Hallway::init_manual()
{
    manual = Neurocaster::getSingletonPtr()->mSceneMgr->createManualObject(name);
//    manual->setUseIdentityProjection(true);
//    manual->setUseIdentityView(true);
    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

    double left = -0.5;
    double top = 0.5;
    double right = 0.5;
    double bottom = -0.5;

    float r = 0.25;
    float g = 0.3;
    float b = 0.35;

    manual->position(Square(left, top).relative_convert());
    manual->textureCoord(0, 1);
    manual->colour(r,g,b);

    manual->position(Square(left, bottom).relative_convert());
    manual->textureCoord(0, 0);
    manual->colour(r,g,b);

    manual->position(Square(right, top).relative_convert());
    manual->textureCoord(1, 1);
    manual->colour(r,g,b);

    manual->position(Square(right, bottom).relative_convert());
    manual->textureCoord(1,0);
    manual->colour(r,g,b);

    manual->end();

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manual->setBoundingBox(aabInf);

    manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_1);

    scene_node = Neurocaster::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    scene_node->attachObject(manual);
    scene_node->setPosition(pos.convert());
}

} // neuro namespace
