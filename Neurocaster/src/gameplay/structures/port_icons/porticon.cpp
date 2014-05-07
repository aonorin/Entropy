#include "porticon.h"

#include "core/neurocaster.h"

using namespace neuro;

int num_ports;

std::string get_unique_port_name(std::string name)
{
    std::stringstream ss;
    ss << name << num_ports++;
    return ss.str();
}

PortIcon::PortIcon(Square pos)
    : NeuroManualObject(pos,get_unique_port_name("PortIcon"))
{

}

void PlayerPortIcon::init_manual()
{

    manual = Neurocaster::getSingletonPtr()->mSceneMgr->createManualObject(name);
//    manual->setUseIdentityView(true);

    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

    double left = -0.5;
    double top = 0.5;
    double right = 0.5;
    double bottom = -0.5;

    float r = 0;
    float g = 0.75;
    float b = 0.25;

    manual->position(Square(left,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(r,g,b);

    manual->position(Square(left,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(r,g,b);

    manual->position(Square(right,top).relative_convert());
    manual->textureCoord(1,1);
    manual->colour(r,g,b);

    manual->position(Square(right,bottom).relative_convert());
    manual->textureCoord(1,0);
    manual->colour(r,g,b);

    manual->end();

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manual->setBoundingBox(aabInf);

    manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_2);

    scene_node = Neurocaster::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    scene_node->attachObject(manual);
    scene_node->setPosition(pos.convert());

    manual->setVisible(false);
}

PlayerPortIcon::PlayerPortIcon(Square pos)
    : NeuroManualObject(pos,get_unique_port_name("PlayerPortIcon"))
{

}

void PortIcon::init_manual()
{

    manual = Neurocaster::getSingletonPtr()->mSceneMgr->createManualObject(name);
    manual->setUseIdentityView(true);

    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

    double left = -0.5;
    double top = 0.5;
    double right = 0.5;
    double bottom = -0.5;

    manual->position(Square(left,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(0.75,0.0,0.0);

    manual->position(Square(left,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(0.75,0.0,0.0);

    manual->position(Square(right,top).relative_convert());
    manual->textureCoord(1,1);
    manual->colour(0.75,0.0,0.0);

    manual->position(Square(right,bottom).relative_convert());
    manual->textureCoord(1,0);
    manual->colour(0.75,0.0,0.0);

    manual->end();

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manual->setBoundingBox(aabInf);

    manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_2);

    scene_node = Neurocaster::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    scene_node->attachObject(manual);
    scene_node->setPosition(pos.convert());
}
