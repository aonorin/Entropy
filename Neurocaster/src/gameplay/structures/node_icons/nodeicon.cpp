#include "nodeicon.h"
#include "core/neurocaster.h"

using namespace neuro;

int num_names;

std::string get_unique_node_name(std::string name)
{
    std::stringstream ss;
    ss << name << num_names++;
    return ss.str();
}

NodeIcon::NodeIcon(Square pos)
    : NeuroManualObject(pos,get_unique_node_name("NodeIcon"))
{

}

void NodeIcon::init_manual()
{
    manual = Neurocaster::getSingletonPtr()->mSceneMgr->createManualObject(name);
//    manual->setUseIdentityView(true);

    manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

    double left = -0.5;
    double top = 0.5;
    double right = 0.5;
    double bottom = -0.5;

    manual->position(Square(left,top).relative_convert());
    manual->textureCoord(0,1);
    manual->colour(0.66,0.66,0);

    manual->position(Square(left,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(0.66,0.88,0);

    manual->position(Square(right,top).relative_convert());
    manual->textureCoord(1,1);
    manual->colour(0.88,0.66,0);

    manual->position(Square(right,bottom).relative_convert());
    manual->textureCoord(1,0);
    manual->colour(0.66,0.88,0);

    manual->end();

    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    manual->setBoundingBox(aabInf);

    manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_2);

    scene_node = Neurocaster::getSingletonPtr()->mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    scene_node->attachObject(manual);
    scene_node->setPosition(pos.convert());
}

PlayerNodeIcon::PlayerNodeIcon(Square pos)
    : NeuroManualObject(pos,get_unique_node_name("PlayerNodeIcon"))
{

}

void PlayerNodeIcon::init_manual()
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
    manual->colour(0,0.66,1);

    manual->position(Square(left,bottom).relative_convert());
    manual->textureCoord(0,0);
    manual->colour(0,0.88,1);

    manual->position(Square(right,top).relative_convert());
    manual->textureCoord(1,1);
    manual->colour(0,0.66,1);

    manual->position(Square(right,bottom).relative_convert());
    manual->textureCoord(1,0);
    manual->colour(0,0.88,1);


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
